#include "modifydialog.h"
#include "ui_modifydialog.h"

ModifyDialog::ModifyDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyDialog)
{
    ui->setupUi(this);
    QSqlQuery *queryCombo = new QSqlQuery();

    queryCombo->exec("SELECT name FROM category");
    while (queryCombo->next())
    {
        ui->comboBox->addItem(queryCombo->value(0).toString());
    }
}

ModifyDialog::~ModifyDialog()
{
    delete ui;
}

void ModifyDialog::on_btnEditRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("UPDATE product SET name = :name, cat_ID = :category, ImagePath = :image WHERE ID = :ID");
    query->bindValue(":ID", tempID);
    query->bindValue(":name",ui->lineEditRecordName->text());
    query->bindValue(":category",ui->comboBox->currentIndex()+1);
    query->bindValue(":image", Img);

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
        ui->comboBox->setCurrentIndex(query->value(1).toInt()-1);
    }
}

void ModifyDialog::on_toolButton_clicked()
{
    Img = QFileDialog::getOpenFileName(0, "Открыть файл", Img, "*.jpg");
    ui->label_5->setPixmap(Img);
}

