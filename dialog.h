#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include "form.h" // 添加对 form.h 的引用
#include "ui_form.h"
#include <QMqttClient>
#include <QMqttTopicName>
#include "dialog2.h"
#include "ui_dialog2.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(const QString& text, QWidget *parent = nullptr);
    ~Dialog();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::Dialog *ui;
    QMqttClient* client;
    QString trimmedText;
};

#endif // DIALOG_H
