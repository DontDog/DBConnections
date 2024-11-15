#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}


void ConnectionDialog::on_btnConnectToDatabase_clicked()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");

    // Формируем строку подключения
    QString serverName = ui->lineEditServerName->text();    // Поле для ввода имени сервера
    QString databaseName = ui->lineEditDatabaseName->text(); // Поле для ввода имени базы данных
    QString username = ui->lineEditUsername->text();     // Поле для ввода имени пользователя
    QString password = ui->lineEditPassword->text();      // Поле для ввода пароля

    database.setDatabaseName(QString("DRIVER={SQL Server};SERVER=%1;DATABASE=%2;")
        .arg(serverName)
        .arg(databaseName));

    // Устанавливаем логин и пароль
    database.setUserName(username);
    database.setPassword(password);

    // Создаём объект для вывода сообщения
    QMessageBox msgBox;

    // Подключение к базе данных
    if (database.open())
    {
        msgBox.setText("Соединение установлено");
        close();
    }
    else
    {
        msgBox.setText("Соединение НЕ установлено:\n" + database.lastError().text()); // Добавляем текст ошибки
    }

    // Показываем сообщение
    msgBox.exec();
}

