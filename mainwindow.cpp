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

