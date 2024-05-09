
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dialog.h"
#include "ui_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewButtonClicked();
    void on_pushButton_clicked();
    void restoreButtonSettings();
    void on_pushButton_2_clicked();
    void saveButtonSettings();
    void closeEvent(QCloseEvent*);
     void everyday();
     void everyEinglish();
     void on_pushButton_3_clicked();

 private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
