#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dlg(nullptr)
    , add(nullptr)
{
    ui->setupUi(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,
     SIGNAL(customContextMenuRequested(QPoint)),
     SLOT(CustomMenuReq(QPoint)));
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

void MainWindow::on_actionUpdateDatabaseState_triggered()
{
    qmodel = new QSqlTableModel();
    qmodel->setQuery("SELECT * FROM product");
    ui->tableView->setModel(qmodel);
}


void MainWindow::on_btnAddRecord_clicked()
{
    if(add == nullptr)
        add = new AddDialog();
    add->show();
}


void MainWindow::on_btnUpdateRecord_clicked()
{
    qmodel = new QSqlTableModel();
    qmodel->setQuery("SELECT * FROM product");
    ui->tableView->setModel(qmodel);
}


void MainWindow::on_btnDeleteRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("DELETE FROM product WHERE ID = :ID");
    query->bindValue(":ID", ui->lineEditRecordID->text());
    query->exec();
    //очищаем поля ввода
    ui->lineEditRecordID->setText("");
    ui->lineEditRecordName->setText("");
    ui->lineEditRecordCategory->setText("");
    //вызов обраотчика кнопки «Обновить»
    MainWindow::on_btnUpdateRecord_clicked();
}


void MainWindow::on_btnEditRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("UPDATE product SET name = :name, "
    "category = :category WHERE ID = :ID");
    query->bindValue(":ID", ui->lineEditRecordID->text());
    query->bindValue(":name", ui->lineEditRecordName->text());
    query->bindValue(":category", ui->lineEditRecordCategory->text());
    //query->bindValue(":image", Img);
    query->exec();

    ui->lineEditRecordID->setText("");
    ui->lineEditRecordName->setText("");
    ui->lineEditRecordCategory->setText("");

    //ui->label_4->setText("");
    MainWindow::on_btnUpdateRecord_clicked();

}


void MainWindow::on_btnFilterData_clicked()
{

}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int temp_ID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT name, cat_ID FROM product WHERE ID =:ID");
    query->bindValue(":ID",temp_ID);
    ui->lineEditRecordID->setText(QString::number(temp_ID));
    if (query->exec()) {
        query->next();
        ui->lineEditRecordName->setText(query->value(0).toString());
        ui->lineEditRecordCategory->setText(query->value(1).toString());
    }
}

void MainWindow::CustomMenuReq(QPoint pos) {
    QModelIndex index = ui->tableView->indexAt(pos);

    if (index.isValid()) {
        QMenu *menu = new QMenu(this);
        QAction *ModRec = new QAction("Изменить", this);
        QAction *DelRec = new QAction("Удалить", this);

        connect(ModRec, &QAction::triggered, this, &MainWindow::ModRecAction);
        connect(DelRec, &QAction::triggered, this, &MainWindow::DelRecAction);

        menu->addAction(ModRec);
        menu->addAction(DelRec);

        menu->exec(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::ModRecAction() {
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        int temp_ID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
        mdlg = new ModifyDialog();
        connect(this, SIGNAL(sendID(int)), mdlg, SLOT(sendingID(int)));
        emit sendID(temp_ID);
        mdlg->show();
        disconnect(this,SIGNAL(sendID(int)),mdlg,
        SLOT(sendingID(int)));
    }
}

void MainWindow::DelRecAction() {
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        int temp_ID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
        QSqlQuery *query = new QSqlQuery();
        query->prepare("DELETE FROM product WHERE ID = :ID");
        query->bindValue(":ID", temp_ID);
        query->exec();
        MainWindow::on_btnUpdateRecord_clicked();
    }
}

