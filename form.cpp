#include "form.h"
#include "ui_form.h"
#include <QMessageBox>
#include <QtMqtt/QtMqtt>
QString st;
QString upid;
form::form(const QString& id, const QString& ves, const QString& newves, const QString& log,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::form),
 client(new QMqttClient(this))
{
    ui->setupUi(this);
    setWindowTitle("更新检查");
    st=id;
    upid = id.mid(2);
    //设备更新
    if(id!=""){
    client->setHostname("mqtt");
    client->setPort(123);
    client->setClientId("clientid");
    client->connectToHost();

    QObject::connect(client, &QMqttClient::connected, [this]() {
        qDebug() << "Connected to MQTT broker!";
        // 订阅主题
        QMqttTopicFilter filter("Flowerpot");
        client->subscribe(filter);
    });

     ui->label_6->setText("设备ID："+id);
 ui->label_8->setText("固件版本："+ves);
 ui->label_3->setText(ves);
  ui->label_5->setText(newves);
 ui->textBrowser->setText(log);
     if(ves==""){
      ui->label_7->setText("设备状态：离线");
     }else{
           ui->label_7->setText("设备状态：在线");
}
    }
    //程序更新
    else{
ui->label_6->setText("万物互联IOT设备PC控制软件更新");
if(newves!=""){
ui->label_7->setText("服务器状态：在线");
}else{
ui->label_7->setText("服务器状态：离线");
}

 ui->label_8->setText("服务器最新版本："+newves);
 ui->label->setText("当前版本："+ves);
 ui->label_3->setText(ves);
 ui->label_5->setText(newves);
  ui->textBrowser->setText(log);
    }
}
form::~form()
{
    delete ui;
}
#include <QDesktopServices>
#include <QUrl>
void form::on_pushButton_clicked()
{
    if(st!=""){
    QMqttTopicName topic("Flowerpot");

           QString str = "#IDTO"+upid+"&Updata";
     QByteArray      byteArray = str.toUtf8();



    client->publish(topic, byteArray, 0, false);
    }else{
    QMessageBox::information(this, "密码提示", "即将打开链接，密钥：2xzk");
     QUrl url("updataurl"); // 替换为您要打开的网页URL
    if (QDesktopServices::openUrl(url)) {


    } else {
// 处理无法打开链接的错误

    }
    }

}

void  form::closeEvent(QCloseEvent *event)
{
    if(st!=""){
    // 断开 MQTT 连接
    client->disconnectFromHost();

    // 调用父类的关闭事件处理函数
    QDialog::closeEvent(event);
    qDebug() << "offfff";
    }
}
