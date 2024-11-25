#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);

    QSqlQuery *queryCombo = new QSqlQuery();
    queryCombo->exec("SELECT name FROM category");
    while (queryCombo->next())
    {
        ui->comboBox->addItem(queryCombo->value(0).toString());
    }

    ui->dateEdit->setDate(QDate::currentDate());
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::on_btnAddRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("INSERT INTO product (name, cat_ID, ImagePath, prodDate) VALUES (:name, :cat_ID, :image, :date)");
    query->bindValue(":name", ui->lineEditRecordName->text());
    query->bindValue(":cat_ID",ui->comboBox->currentIndex()+1);
    query->bindValue(":image", Img);
    query->bindValue(":date", ui->dateEdit->text());
    query->exec();
    close(); //закрытие диалогового окна
}

void AddDialog::on_toolButton_clicked()
{
    Img = QFileDialog::getOpenFileName(0, "Открыть файл", Img, "*.jpg");
    ui->label_5->setPixmap(Img);
}
