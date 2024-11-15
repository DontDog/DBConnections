#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>

#include "connectiondialog.h"
#include "ui_connectiondialog.h"

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
    void on_actionConnectToDatabase_triggered();

    void on_actionDisconnectFromDatabase_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    ConnectionDialog *dlg;
    QSqlQueryModel *qmodel;
};
#endif // MAINWINDOW_H
