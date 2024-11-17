#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSqlQueryModel>

#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "adddialog.h"
#include "ui_adddialog.h"
#include "modifydialog.h"
#include "ui_modifydialog.h"

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

    void on_actionUpdateDatabaseState_triggered();

    void on_btnAddRecord_clicked();

    void on_btnUpdateRecord_clicked();

    void on_btnDeleteRecord_clicked();

    void on_btnEditRecord_clicked();

    void on_btnFilterData_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void CustomMenuReq(QPoint pos);

    void ModRecAction();

    void DelRecAction();

private:
    Ui::MainWindow *ui;
    ConnectionDialog *dlg;
    AddDialog *add;
    ModifyDialog *mdlg;
    QSqlQueryModel *qmodel;

signals:
    void sendID(int);

};
#endif // MAINWINDOW_H
