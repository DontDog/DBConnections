#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dlg(nullptr)
    , add(nullptr)
{
    ui->setupUi(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(CustomMenuReq(QPoint)));

    // Пример создания QShortcut для фильтрации
    QShortcut *filterShortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(filterShortcut, &QShortcut::activated, this, &MainWindow::on_btnFilterData_clicked);

    // Пример создания QShortcut для обновления данных
    QShortcut *updateShortcut = new QShortcut(QKeySequence("Ctrl+U"), this);
    connect(updateShortcut, &QShortcut::activated, this, &MainWindow::on_btnUpdateRecord_clicked);

    // Пример создания QShortcut для добавления данных
    QShortcut *addShortcut = new QShortcut(QKeySequence("Ctrl+A"), this);
    connect(addShortcut, &QShortcut::activated, this, &MainWindow::on_btnAddRecord_clicked);

    // Пример создания QShortcut для удаления данных
    QShortcut *deletShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    connect(deletShortcut, &QShortcut::activated, this, &MainWindow::on_btnDeleteRecord_clicked);

    // Пример создания QShortcut для удаления данных
    QShortcut *editShortcut = new QShortcut(QKeySequence("Ctrl+E"), this);
    connect(editShortcut, &QShortcut::activated, this, &MainWindow::on_btnEditRecord_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionConnectToDatabase_triggered()
{
    if(dlg == nullptr)
        dlg = new ConnectionDialog();
    dlg->show();

    QSqlQuery *queryCombo = new QSqlQuery();
    queryCombo->exec("SELECT name FROM category");
    while (queryCombo->next())
    {
        ui->comboBox->addItem(queryCombo->value(0).toString());
    }
}


void MainWindow::on_actionDisconnectFromDatabase_triggered()
{
   if(dlg == nullptr)
        dlg = new ConnectionDialog();
    dlg->disconnectFromDatabase();
}

void MainWindow::on_actionUpdateDatabaseState_triggered()
{
    qmodel = new QSqlTableModel();
    qmodel->setQuery("SELECT a.ID, a.name, b.name, a.ImagePath FROM product a INNER JOIN category b ON a.cat_ID = b.id;");
    ui->tableView->setModel(qmodel);
}


void MainWindow::on_btnAddRecord_clicked()
{
    if(add == nullptr)
        add = new AddDialog();
    add->show();
}


void MainWindow::on_btnUpdateRecord_clicked()
{
    qmodel = new QSqlTableModel();
    qmodel->setQuery("SELECT a.ID, a.name, b.name, a.ImagePath, a.prodDate FROM product a INNER JOIN category b ON a.cat_ID = b.id;");
    ui->tableView->setModel(qmodel);
}


void MainWindow::on_btnDeleteRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("DELETE FROM product WHERE ID = :ID");
    query->bindValue(":ID", ui->lineEditRecordID->text());
    query->exec();
    //очищаем поля ввода
    ui->lineEditRecordID->setText("");
    ui->lineEditRecordName->setText("");
    ui->comboBox->setCurrentIndex(query->value(1).toInt()-1);
    //вызов обраотчика кнопки «Обновить»
    MainWindow::on_btnUpdateRecord_clicked();
}


void MainWindow::on_btnEditRecord_clicked()
{
    QSqlQuery *query = new QSqlQuery();
    query->prepare("UPDATE product SET name = :name, "
    "cat_ID = :cat_ID, ImagePath = :image WHERE ID = :ID");
    query->bindValue(":ID", ui->lineEditRecordID->text());
    query->bindValue(":name", ui->lineEditRecordName->text());
    query->bindValue(":cat_ID",ui->comboBox->currentIndex()+1);
    query->bindValue(":image", Img);
    query->exec();

    ui->lineEditRecordID->setText("");
    ui->lineEditRecordName->setText("");
    ui->comboBox->setCurrentIndex(query->value(1).toInt()-1);
    ui->label_4->setText("");
    MainWindow::on_btnUpdateRecord_clicked();

}


void MainWindow::on_btnFilterData_clicked()
{
    ui->lineEditRecordID->setText("");
    QString name = ui->lineEditRecordName->text();
    int cat_ID = ui->comboBox->currentIndex();

    QSqlTableModel *filteredModel = new QSqlTableModel(this);
    filteredModel->setTable("product"); // Указываем таблицу
    QString filter;

    if (!name.isEmpty()) {
        filter += QString("name LIKE '%%1%'").arg(name); // Используем LIKE для частичного совпадения
    }
    if (!cat_ID) {
        if (!filter.isEmpty()) {
            filter += " AND "; // Добавляем AND, если уже есть фильтры
        }
        filter += QString("cat_ID = '%1'").arg(cat_ID); // Предполагается, что cat_ID — это числовой тип
    }

    // Устанавливаем фильтр модели
    filteredModel->setFilter(filter);

    // Загружаем данные в модель
    if (!filteredModel->select()) {
        qDebug() << "Ошибка при загрузке данных:" << filteredModel->lastError().text();
    }

    // Устанавливаем модель на таблицу
    ui->tableView->setModel(filteredModel);
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int temp_ID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT name, cat_ID, ImagePath FROM product WHERE ID =:ID");
    query->bindValue(":ID",temp_ID);
    ui->lineEditRecordID->setText(QString::number(temp_ID));
    if (query->exec()) {
        query->next();
        ui->lineEditRecordName->setText(query->value(0).toString());
        ui->comboBox->setCurrentIndex(query->value(1).toInt()-1);

        Img = query->value(2).toString();
        ui->label_4->setPixmap(Img);

    }
}

void MainWindow::CustomMenuReq(QPoint pos) {
    QModelIndex index = ui->tableView->indexAt(pos);

    if (index.isValid()) {
        QMenu *menu = new QMenu(this);
        QAction *ModRec = new QAction("Изменить", this);
        QAction *DelRec = new QAction("Удалить", this);

        connect(ModRec, &QAction::triggered, this, &MainWindow::ModRecAction);
        connect(DelRec, &QAction::triggered, this, &MainWindow::DelRecAction);

        menu->addAction(ModRec);
        menu->addAction(DelRec);

        menu->exec(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::ModRecAction() {
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        int temp_ID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
        mdlg = new ModifyDialog();
        connect(this, SIGNAL(sendID(int)), mdlg, SLOT(sendingID(int)));
        emit sendID(temp_ID);
        mdlg->show();
        disconnect(this,SIGNAL(sendID(int)),mdlg,
        SLOT(sendingID(int)));
    }
}

void MainWindow::DelRecAction() {
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        int temp_ID = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
        QSqlQuery *query = new QSqlQuery();
        query->prepare("DELETE FROM product WHERE ID = :ID");
        query->bindValue(":ID", temp_ID);
        query->exec();
        MainWindow::on_btnUpdateRecord_clicked();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F && event->modifiers() == Qt::ControlModifier) {
        on_btnFilterData_clicked();
    } else if (event->key() == Qt::Key_U && event->modifiers() == Qt::ControlModifier) {
        on_btnUpdateRecord_clicked();
    } else if (event->key() == Qt::Key_A && event->modifiers() == Qt::ControlModifier) {
        on_btnAddRecord_clicked();
    } else if (event->key() == Qt::Key_D && event->modifiers() == Qt::ControlModifier) {
        on_btnDeleteRecord_clicked();
    } else if (event->key() == Qt::Key_E && event->modifiers() == Qt::ControlModifier) {
        on_btnEditRecord_clicked();
    } else {
        QMainWindow::keyPressEvent(event); // Вызов базового класса для дальнейшей обработки
    }
}


void MainWindow::on_btnExportToWord_clicked()
{
    pdlg = new PrintDialog("Word");
    pdlg->show();
}

void MainWindow::on_btnExportToPDF_clicked()
{
    pdlg = new PrintDialog("PDF");
    pdlg->show();
}

void MainWindow::on_toolButton_clicked()
{
    Img = QFileDialog::getOpenFileName(0, "Открыть файл", Img, "*.jpg");
    ui->label_4->setPixmap(Img);

}


void MainWindow::on_action_triggered()
{
    pg = new PrintGraf();
    pg->show();
}

