#include "dialog2.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "ui_dialog2.h"

Dialog2::Dialog2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog2)
{
    ui->setupUi(this);
        setWindowTitle("AI Chat");
}

Dialog2::~Dialog2()
{
    delete ui;
}
QString strall;
QString discuss;
QString ask;
QString answer;
void Dialog2::on_pushButton_clicked()
{
    ui->textBrowser->setPlainText("正在等待ChatGPT回答您的问题，请稍等......");
    ask=ui->textEdit->toPlainText();
    discuss=discuss+"Q:"+ask+"A:";
    QString test2;"ESP8266控制电路数据上传："+ask;
    qDebug() << "ESP8266控制电路数据上传:"+ui->textEdit->toPlainText();;
    // if(ask.length()>880){
    //   QMessageBox::warning(this, "Error", "长度超出限制，当前共"+QString::number(ask.length())+"字节\n请删除部分字符使其控制在880字节以内。");
    //}


    int num=0;

    QNetworkAccessManager manager;

    // 创建POST请求
    QUrl url("aiurl"); // 更换为你要访问的API地址
    QNetworkRequest request(url);
    QString accessToken = "token";
    QByteArray authHeader = "Bearer " + accessToken.toUtf8();

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", authHeader);
    // 准备POST数据
    QByteArray postData;
    postData.append("session_id=123&content=[{\"role\":\"system\",\"content\":\"You are Flowerpot AI, a large language model trained by YANG\\nCarefully heed the user's instructions.\\nRespond using Markdown.\\nCurrent Date: 2024/3/18 14:00:23\\n\"},{\"role\":\"user\",\"content\":\""+ask.toUtf8()+"\"}]&max_context_length=5&params={\"model\":\"gpt-3.5-turbo\",\"temperature\":1,\"max_tokens\":2048,\"presence_penalty\":0,\"frequency_penalty\":0,\"max_context_length\":5,\"voiceShortName\":\"zh-CN-XiaoxiaoNeural\",\"rate\":1,\"pitch\":1}"); // 根据API要求，构建合适的POST参数

    // 发送POST请求
    QNetworkReply *reply = manager.post(request, postData);

    // 等待请求完成
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QString str1="You:\n"+ask+"\n\n";

    // 获取返回值
    if(reply->error() == QNetworkReply::NoError) {
        QString responsex = reply->readAll();
       QString response= responsex.remove(responsex.length() - 6, 6);
        QString str2="Chat Robot:\n"+response+"\n\n";
        strall=strall+str1+str2;
        ui->textBrowser->setPlainText(strall);
        QString test1="AI数据应答："+response;
        qDebug() << test1;
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    // 清理
    reply->deleteLater();
}

