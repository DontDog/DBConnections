#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class ModifyDialog;
}

class ModifyDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyDialog(QWidget *parent = nullptr);
    ~ModifyDialog();

private slots:
    void on_btnEditRecord_clicked();

    void sendingID(int);

private:
    Ui::ModifyDialog *ui;
    int tempID;
};

#endif // MODIFYDIALOG_H
