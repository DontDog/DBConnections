#include "printdialog.h"
#include "ui_printdialog.h"
#include <QDesktopServices>
#include <QUrl>

PrintDialog::PrintDialog(const QString &_format, QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::PrintDialog)
    ,format(_format)
{
    ui->setupUi(this);
}

PrintDialog::~PrintDialog()
{
    delete ui;
}

void PrintDialog::on_btnExport_clicked()
{
    if(this->format == "Word"){
        PrintDialog::exportToWord();
    }
    else {
        PrintDialog::exportToPDF();
    }
    close();
}


void PrintDialog::on_tbtn_clicked()
{
    if(this->format == "Word"){
        ui->lineEditFilePath->setText(QFileDialog::getOpenFileName(0,"Выберете файл",".\\","*.html"));

    }
    else {
        ui->lineEditFilePath->setText(QFileDialog::getOpenFileName(0,"Выберете файл",".\\","*.pdf"));
    }
}

void PrintDialog::exportToWord(){
    QFile *file = new QFile();
    file->setFileName(ui->lineEditFilePath->text());
    file->open(QIODevice::WriteOnly);
    QTextStream in(file);
    in << "<html><head></head><body><center>"
    +QString("Отчёта");
    in << "<table border=1><tr>";
    in << "<td>"+QString("ID")+"</td>";
    in << "<td>"+QString("Наименование")+"</td>";
    in << "<td>"+QString("Категория")+"</td></tr>";
    QSqlQuery *query = new QSqlQuery();
    query->exec("SELECT * FROM product");
    query->next();
    while (query->next())
    {

        in << "<tr>";
        in << "<td>"+QString(query->value(0).toString())+"</td>";
        in << "<td>"+QString(query->value(1).toString())+"</td>";
        in << "<td>"+QString(query->value(2).toString())+"</td>";
        in << "</tr>";
    }
    in << "</table>";
    in << "</center></body></html>";
    file->close();

    QAxObject *word = new QAxObject("Word.Application",this);
    word->setProperty("DisplayAlerts",false);
    word->setProperty("Visible", true);
    QAxObject *doc = word->querySubObject("Documents");
    doc->dynamicCall("Open(QVariant)", ui->lineEditFilePath->text());
}

void PrintDialog::exportToPDF(){
    QString str;
    str.append("<html><head></head><body><center>" + QString("Отчет"));
    str.append("<table border=1><tr>");
    str.append("<td>"+QString("ID")+"</td>");
    str.append("<td>"+QString("Наименование")+"</td>");
    str.append("<td>"+QString("Категория")+"</td></tr>");
    QSqlQuery *query = new QSqlQuery();
    query->exec("SELECT a.ID, a.name, b.name, a.ImagePath FROM product a INNER JOIN category b ON a.cat_ID = b.id;");
    query->next();
    while(query->next())
    {
        str.append("<tr>");
        str.append("<td>" + QString(query->value(0).toString()) + "</td>");
        str.append("<td>" + QString(query->value(1).toString()) + "</td>");
        str.append("<td>" + QString(query->value(2).toString()) + "</td></tr>");
    }
    str.append("</table>");
    str.append("</center></body></html>");

    QPrinter printer;
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(ui->lineEditFilePath->text());
    QTextDocument doc;
    doc.setHtml(str);
    doc.print(&printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::current().absoluteFilePath(ui->lineEditFilePath->text())));
    close();
}
