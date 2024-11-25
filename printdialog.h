#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include <QWidget>
#include <QSqlQuery> //создание запросов
#include <QFileDialog> //диалоговое окно работы с файлами
#include <QFile> //работа с файлами
#include <QTextStream> //организация текстовых потоков
#include <QAxObject>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>

namespace Ui {
class PrintDialog;
}

class PrintDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PrintDialog(const QString &format, QWidget *parent = nullptr);
    ~PrintDialog();

private slots:
    void on_btnExport_clicked();

    void on_tbtn_clicked();

private:
    void exportToWord();
    void exportToPDF();

    Ui::PrintDialog *ui;
    const QString format;
};

#endif // PRINTDIALOG_H
