#include "dialog.h"
#include "ui_dialog.h"
#include <QtMqtt/QtMqtt>
#include <QMessageBox>
QStringList parts;
QByteArray byteArray;
QString formattedVersion;
QString newves;
QString ID;
QString remainderx;
Dialog::Dialog(const QString& text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    client(new QMqttClient(this))
{
    ui->setupUi(this);
    setWindowTitle("设备管理");
    ID=text;
    ui->label_6->setText("设备ID："+text);
    trimmedText = text.mid(2); // 从索引2（即"fp"之后）开始截取文本

    client->setHostname("url");
    client->setPort(123);
    client->setClientId("clientid");
    client->connectToHost();

    QObject::connect(client, &QMqttClient::connected, [this]() {
        qDebug() << "Connected to MQTT broker!";
        // 订阅主题
        QMqttTopicFilter filter("Flowerpot");
        client->subscribe(filter);
    });

    QObject::connect(client, &QMqttClient::messageReceived, [this](const QByteArray& message, const QMqttTopicName& topic) {
        QString messageContent = message;
        QString desiredPrefix = "#ID" + trimmedText;
        formattedVersion ="";
        if (messageContent.startsWith(desiredPrefix)) {

            ui->label_7->setText("设备状态：在线");
            QString messageString = QString::fromUtf8(message);
            qDebug() << "Received message:" << message << "from topic:" << topic.name();
          parts = messageString.split('#');

            for (const QString& part : parts) {
                qDebug() << "Split part:" << part;
            }
            QString firmwareVersion = parts[8];
            int versionNumber = firmwareVersion.toInt(); // 将字符串转换为整数

            int x = versionNumber / 100; // 获取百位数
            int y = (versionNumber / 10) % 10; // 获取十位数
            int z = versionNumber % 10; // 获取个位数

            formattedVersion = QString("V%1.%2.%3").arg(x).arg(y).arg(z); // 创建格式化后的版本号字符串
            ui->label_8->setText("固件版本："+formattedVersion);
            ui->label_3->setText(parts[2]+"%");
            ui->label_4->setText(parts[4]+"%");
            ui->label_5->setText(parts[3]+"%");
            if(parts[6]=="1"){
                ui->pushButton->setStyleSheet("background-color: red;");
            }else{
                ui->pushButton->setStyleSheet("background-color: white;");
            }
            if(parts[7]=="1"){
                ui->pushButton_2->setStyleSheet("background-color: red;");
            }else{
                ui->pushButton_2->setStyleSheet("background-color: white;");
            }
            if(parts[5]=="1"){
                ui->pushButton_3->setStyleSheet("background-color: red;");
            }else{
                ui->pushButton_3->setStyleSheet("background-color: white;");
            }
            ui->checkBox->blockSignals(true);
            if (parts[9] == "1") {
                ui->checkBox->setCheckState(Qt::Checked);  // 设置为选中状态
            } else {
                ui->checkBox->setCheckState(Qt::Unchecked);  // 设置为未选中状态
            }
            ui->checkBox->blockSignals(false);
        } else {
            // 忽略其他的消息
        }
    });
    //更新检查服务器
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("updata url");
    QNetworkRequest request(url);

    // Set unsafe SSL configuration
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(config);

    QNetworkReply *reply = manager->get(request);

    // Connect to the finished signal
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Read the JSON data
            QByteArray data = reply->readAll();

            // Display the JSON data in a message box
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(data).toUtf8());
            QJsonObject jsonObj = jsonDoc.object();

            // 获取数据
            QJsonObject jObj2 = jsonDoc["data"].toObject();
            QString str = jObj2 ["content"].toString();
            qDebug() << str;
            newves = str.left(6);                  // 提取前6个字符组成newves

            remainderx = str.mid(6);                // 提取剩下的字符
            remainderx.replace("#", "\n");
            qDebug() << "update1111111";
            qDebug() << newves;
            qDebug() << formattedVersion;
            if (newves != formattedVersion) {
              //  QMessageBox::warning(this, "Error", "已发布新版本\"" + info + "\"。\n点击\"设置\"下载最新版本。");
                // ui->pushButton_4->setText("发现新版本");
                  ui->pushButton_6->setStyleSheet("background-color: red;");
              ui->pushButton_6->setText("发现新版本！");
qDebug() << "update";
            }
            else {
                //  ui->pushButton_4->setText("暂无更新");
qDebug() << "no";
            }
        }
        else {
            // Display an error message
            QMessageBox::warning(this, "Error", reply->errorString());
        }

        // Clean up
        reply->deleteLater();
        manager->deleteLater();
    });
}

Dialog::~Dialog()
{
    delete ui;
}



void Dialog::on_pushButton_clicked()
{
    if (client->state() == QMqttClient::Connected) {
        QMqttTopicName topic("Flowerpot");
        if (parts[6]=="1") {
            QString str = "#IDTO" + trimmedText+"&OFFHOT";
            byteArray = str.toUtf8();
        }else{
            QString str = "#IDTO" + trimmedText+"&ONHOT";
            byteArray = str.toUtf8();
        }


        client->publish(topic, byteArray, 0, false);
        // 发送消息后做其他处理
    } else {
        QMessageBox::warning(this, "Error", "MQTT client is not connected!");
    }
}


void Dialog::on_pushButton_2_clicked()
{
    if (client->state() == QMqttClient::Connected) {
        QMqttTopicName topic("Flowerpot");
        if (parts[7]=="1") {
            QString str = "#IDTO" + trimmedText+"&OFFLED";
            byteArray = str.toUtf8();
        }else{
            QString str = "#IDTO" + trimmedText+"&ONLED";
            byteArray = str.toUtf8();
        }


        client->publish(topic, byteArray, 0, false);
        // 发送消息后做其他处理
    } else {
        QMessageBox::warning(this, "Error", "MQTT client is not connected!");
    }
}


void Dialog::on_pushButton_3_clicked()
{
    if (client->state() == QMqttClient::Connected) {
        QMqttTopicName topic("Flowerpot");
        if (parts[5]=="1") {
            QString str = "#IDTO" + trimmedText+"&OFFPUMP";
            byteArray = str.toUtf8();
        }else{
            QString str = "#IDTO" + trimmedText+"&ONPUMP";
            byteArray = str.toUtf8();
        }


        client->publish(topic, byteArray, 0, false);
        // 发送消息后做其他处理
    } else {
        QMessageBox::warning(this, "Error", "MQTT client is not connected!");
    }
}


void Dialog::on_pushButton_4_clicked()
{
    // 实例化第二个 Dialog2 对象
    Dialog2 dialog2;

    // 显示第二个 Dialog2
    dialog2.exec();
}

void Dialog::on_pushButton_5_clicked()
{

}

void Dialog::on_pushButton_6_clicked()
{
    form* formDialog = new form(ID, formattedVersion,newves,remainderx,this); // 使用其他名称替换 'form'
    formDialog->show();
}


void Dialog::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        qDebug() << "复选框被选中";
        // 根据选中状态执行特定操作
        if (client->state() == QMqttClient::Connected) {
            QMqttTopicName topic("Flowerpot");
            QString str = "#IDTO" + trimmedText+"&ONAUTO";
            byteArray = str.toUtf8();
            client->publish(topic, byteArray, 0, false);
            // 发送消息后做其他处理
        } else {
            QMessageBox::warning(this, "Error", "MQTT client is not connected!");
        }
    } else if (arg1 == Qt::Unchecked) {
        qDebug() << "复选框未被选中";
        // 根据未选中状态执行特定操作
        if (client->state() == QMqttClient::Connected) {
            QMqttTopicName topic("Flowerpot");
            QString str = "#IDTO" + trimmedText+"&OFFAUTO";
            byteArray = str.toUtf8();
            client->publish(topic, byteArray, 0, false);
            // 发送消息后做其他处理
        } else {
            QMessageBox::warning(this, "Error", "MQTT client is not connected!");
        }
    }
}

void Dialog::closeEvent(QCloseEvent *event)
{
    // 断开 MQTT 连接
    client->disconnectFromHost();

    // 调用父类的关闭事件处理函数
    QDialog::closeEvent(event);
}
