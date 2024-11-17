#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::on_btnAddRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("INSERT INTO product (name, cat_ID) VALUES (:name, :cat_ID)");
    query->bindValue(":name", ui->lineEditRecordName->text());
    query->bindValue(":cat_ID", ui->lineEditRecordCategory->text());
    query->exec();
    close(); //закрытие диалогового окна
}

