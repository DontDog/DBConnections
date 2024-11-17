#include "modifydialog.h"
#include "ui_modifydialog.h"

ModifyDialog::ModifyDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyDialog)
{
    ui->setupUi(this);
}

ModifyDialog::~ModifyDialog()
{
    delete ui;
}

void ModifyDialog::on_btnEditRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("UPDATE product SET name = :name, cat_ID = :cat_ID WHERE ID = :ID");
    query->bindValue(":ID", tempID);
    query->bindValue(":name",ui->lineEditRecordName->text());
    query->bindValue(":cat_ID",ui->lineEditRecordCategory->text());
    if(query->exec())
    {
        close();
    }

}

void ModifyDialog::sendingID(int _tempID)
{
    tempID = _tempID;
    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT name, cat_ID FROM product WHERE ID = :ID");
    query->bindValue(0, tempID);
    if (query->exec())
    {
        query->next();
        ui->lineEditRecordName->setText(query->value(0).toString());
        ui->lineEditRecordCategory->setText(query->value(1).toString());
    }
}


