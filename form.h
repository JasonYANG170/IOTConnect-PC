#ifndef FORM_H
#define FORM_H

#include <QMqttClient>
#include <QMqttTopicName>
#include <QDialog>

namespace Ui {
class form;
}

class form : public QDialog
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event) override;
public:
    explicit form(const QString& id, const QString& ves, const QString& newves, const QString& log,QWidget *parent = nullptr);
    ~form();

private slots:
    void on_pushButton_clicked();

private:
    Ui::form *ui;
     QMqttClient* client;
};

#endif // FORM_H
