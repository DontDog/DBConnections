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

// Функция для отображения успешного сообщения
void ConnectionDialog::showSuccessMessage(const QString &message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Успех");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information); // Иконка для успешного сообщения
    msgBox.setStandardButtons(QMessageBox::Ok); // Кнопка "OK"
    msgBox.exec(); // Отображение сообщения
}

// Функция для отображения сообщения об ошибке
void ConnectionDialog::showErrorMessage(const QString &message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Ошибка");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Critical); // Иконка для сообщения об ошибке
    msgBox.setStandardButtons(QMessageBox::Ok); // Кнопка "OK"
    msgBox.exec(); // Отображение сообщения
}

void ConnectionDialog::on_btnConnectToDatabase_clicked()
{
    db = QSqlDatabase::addDatabase("QODBC");

    // Формируем строку подключения
    QString serverName = ui->lineEditServerName->text();    // Поле для ввода имени сервера
    QString databaseName = ui->lineEditDatabaseName->text(); // Поле для ввода имени базы данных
    QString username = ui->lineEditUsername->text();     // Поле для ввода имени пользователя
    QString password = ui->lineEditPassword->text();      // Поле для ввода пароля

    db.setDatabaseName(QString("DRIVER={SQL Server};SERVER=%1;DATABASE=%2;")
        .arg(serverName).arg(databaseName));

    // Устанавливаем логин и пароль
    db.setUserName(username);
    db.setPassword(password);

    // Подключение к базе данных
    if (db.open())
    {
        showSuccessMessage("Соединение установлено");
        close();
    }
    else
    {
        showErrorMessage("Соединение НЕ установлено:\n" + db.lastError().text()); // Добавляем текст ошибки
    }

}

void ConnectionDialog::disconnectFromDatabase() {
    if (db.isOpen()) {
        db.close(); // Закрываем соединение
        showSuccessMessage("Соединение с базой данных разорвано.");
    } else {
        showErrorMessage("Нет открытого соединения с базой данных.");
    }
}
