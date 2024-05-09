#include <QInputDialog>
#include "mainwindow.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <functional>
#include <QCloseEvent>

#include <QSettings>
QString newves33;
QString ves33="V1.0.0";
QString remainderx33;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
setWindowTitle("万物互联IOT桌面端");
    // 恢复上次保存的按钮信息
    restoreButtonSettings();
    //更新检查连接
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("api1url");
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
            newves33 = str.left(6);                  // 提取前6个字符组成newves

            remainderx33 = str.mid(6);                // 提取剩下的字符
            remainderx33.replace("#", "\n");
            qDebug() << "update1111111";
            qDebug() << newves33;
            qDebug() << ves33;
            if (newves33 != ves33) {
                //  QMessageBox::warning(this, "Error", "已发布新版本\"" + info + "\"。\n点击\"设置\"下载最新版本。");
                 ui->pushButton->setText("发现新版本");
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
    everyday();
    everyEinglish();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    form* formDialog = new form("",ves33,newves33,remainderx33,this); // 使用其他名称替换 'form'
    formDialog->show();

}

void MainWindow::onNewButtonClicked()
{
    // 在新按钮被点击时执行的操作
    qDebug() << "New Button Clicked";
}

void MainWindow::on_pushButton_2_clicked()
{
    // 创建一个新的 addButton


    // 创建输入对话框
    bool ok;
    QString buttonText = QInputDialog::getText(this, "添加设备", "请输入设备ID：", QLineEdit::Normal, "", &ok);

    // 如果用户点击了“OK”按钮，则设置按钮的文本名称为输入的内容
    if (ok && !buttonText.isEmpty()) {

        if (buttonText.contains("fp")) {
            QPushButton* newButton = new QPushButton("Add Button", ui->groupBox);

            // 获取已存在的布局对象
            QVBoxLayout* layout = ui->verticalLayout;

            // 添加新的 addButton 到布局
            layout->addWidget(newButton);
        newButton->setText("Flowerpot\nID:"+buttonText);
            // 连接新按钮的 clicked 信号到 lambda 函数
            connect(newButton, &QPushButton::clicked, [=]() {
                // 获取当前按钮的索引
                int buttonIndex = layout->indexOf(newButton) + 1;
                // 显示当前按钮的索引

                // 如果 addButton 文本名称包含 "fp"，则弹出相应的窗口
                if (buttonText.contains("fp")) {
                    QString fpText = buttonText.mid(buttonText.indexOf("fp"));
                    Dialog* dialog = new Dialog(fpText, this);
                    dialog->show();
                }

                // 可以在这里添加其他点击按钮时的操作
            });
        }else{
            QMessageBox::information(this, "设备ID错误", "设备ID错误，检查你输入的设备ID是否正确！" );

        }

    }


}

void MainWindow::saveButtonSettings()
{
    // 获取已存在的布局对象
    QVBoxLayout* layout = ui->verticalLayout;

    // 创建 QSettings 对象，用于读写注册表信息
    QSettings settings("IOT", "IOT");

    // 清空注册表中的旧按钮信息
    settings.remove("Buttons");

    // 保存每个按钮的文本和索引
    for (int i = 0; i < layout->count(); i++) {
        QPushButton* button = qobject_cast<QPushButton*>(layout->itemAt(i)->widget());
        if (button) {
            QString buttonText = button->text();
            settings.setValue(QString("Buttons/button%1Text").arg(i), buttonText);
        }
    }
}

void MainWindow::restoreButtonSettings()
{
    // 获取已存在的布局对象
    QVBoxLayout* layout = ui->verticalLayout;

    // 创建 QSettings 对象，用于读写注册表信息
    QSettings settings("IOT", "IOT");

    // 遍历按钮索引，读取注册表中的按钮信息，并添加按钮到布局
    int buttonIndex = 0;
    while (true) {
        QString buttonText = settings.value(QString("Buttons/button%1Text").arg(buttonIndex)).toString();
        if (buttonText.isEmpty()) {
            break; // 没有更多的按钮信息可读取，退出循环
        }

        QPushButton* newButton = new QPushButton(buttonText, ui->groupBox);
        layout->addWidget(newButton);

        // 连接按钮的 clicked 信号到槽函数
        connect(newButton, &QPushButton::clicked, [=]() {
            // 显示按钮索引
         //   QMessageBox::information(this, "Button Index", "This is button number " + QString::number(buttonIndex));
            if (buttonText.contains("fp")) {
                QString fpText = buttonText.mid(buttonText.indexOf("fp"));
                Dialog* dialog = new Dialog(fpText, this);
                dialog->show();
            }
            // 可以在这里添加按钮点击时的操作
        });

        buttonIndex++;
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    saveButtonSettings();
    event->accept();
}
void MainWindow::everyday()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("updataurl");
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
            QString city = jsonObj["city"].toString();
            QJsonObject jObj2 = jsonDoc["info"].toObject();
            QString high = jObj2 ["high"].toString();
                QString low = jObj2 ["low"].toString();
                   QString date = jObj2 ["date"].toString();
                    QString week = jObj2 ["week"].toString();
            QString type = jObj2 ["type"].toString();
            QJsonObject jObj23 = jObj2["air"].toObject();
            QString aqiname = jObj23 ["aqi_name"].toString();
            QString tip =jObj2["tip"].toString();
            QString info = city+" "+date+" "+week+" "+type+"\n温度:"+low+"-"+high+" 空气质量:"+aqiname;
                  ui->label_2->setWordWrap(true);
                         ui->label_2->setText("每日天气：\n\n"+info);
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

void MainWindow::everyEinglish()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("api2url");
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

            QJsonObject jObj2 = jsonDoc["data"].toObject();
            QString key1Val = jObj2["zh"].toString();
            QString key1Val2 = jObj2["en"].toString();
 ui->label_3->setWordWrap(true);
            ui->label_3->setText("每日英语：\n\n"+key1Val2);
          //  QMessageBox::information(this, "JSON Data", key1Val2);

        } else {
            // Display an error message
            QMessageBox::warning(this, "Error", reply->errorString());
        }

        // Clean up
        reply->deleteLater();
        manager->deleteLater();
    });
}

void MainWindow::on_pushButton_3_clicked()
{
    // 获取信号的发送者
    QVBoxLayout* layout = ui->verticalLayout;
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

