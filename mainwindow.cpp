#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dlg(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionConnectToDatabase_triggered()
{
    if(dlg == nullptr)
        dlg = new ConnectionDialog();
    dlg->show();
}


void MainWindow::on_actionDisconnectFromDatabase_triggered()
{
   if(dlg == nullptr)
        dlg = new ConnectionDialog();
    dlg->disconnectFromDatabase();
}


void MainWindow::on_pushButton_clicked()
{
    qmodel = new QSqlTableModel();
    qmodel->setQuery("SELECT * FROM product");
    ui->tableView->setModel(qmodel);
}


void MainWindow::on_actionUpdateDatabaseState_triggered()
{
    qmodel = new QSqlTableModel();
    qmodel->setQuery("SELECT * FROM product");
    ui->tableView->setModel(qmodel);
}

