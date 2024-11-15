#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    void disconnectFromDatabase();
    ~ConnectionDialog();
    QMessageBox*msg;


private slots:
    void on_btnConnectToDatabase_clicked();

private:
    Ui::ConnectionDialog *ui;
    QSqlDatabase db;
    void showSuccessMessage(const QString &message);
    void showErrorMessage(const QString &message);
};

#endif // CONNECTIONDIALOG_H
