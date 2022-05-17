#include <mainwindow.h>
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>

#include <selectdatabasedialog.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selector_database_window(this)
{
    ui->setupUi(this);

    QObject::connect(ui->actionConectar, &QAction::triggered,
                     this, &MainWindow::connectDatabase);

    ui->actionDesconectar->setEnabled(false);
    QObject::connect(ui->actionDesconectar, &QAction::triggered,
                     this, &MainWindow::disconnectDatabase);

    ui->selectorTable->setEnabled(false);
    QObject::connect(ui->selectorTable, &QComboBox::currentIndexChanged,
                     this, &MainWindow::updateTable);


    QObject::connect(ui->tableWidget, &QTableWidget::cellChanged,
                     this, &MainWindow::updateItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectDatabase()
{
    qDebug() << "Connecting database";
    selector_database_window.exec();
    if (selector_database_window.database_valid())
    {
        ui->actionConectar->setEnabled(false);
        ui->actionDesconectar->setEnabled(true);

        ui->selectorTable->setEnabled(true);
    }
}

void MainWindow::disconnectDatabase()
{
    qDebug() << "Disconnecting database";
    selector_database_window.disconnectDB();
    ui->actionConectar->setEnabled(true);
    ui->actionDesconectar->setEnabled(false);

    ui->selectorTable->setEnabled(false);
}

void MainWindow::updateTable(int idx)
{
    if (idx == 0) {
        QSqlQuery q("select ID_PEDIDO, ID_CLIENTE, FECHA_LANZAMIENTO, CANTIDAD from pedidos",
                     selector_database_window.getDB());
        ui->tableWidget->horizontalHeaderItem(1)->setText("Cliente");
        this->fillTable(q);
    } else {
        QSqlQuery q("select ID_PEDIDO, ID_CLIENTE, FECHA_LANZAMIENTO, CANTIDAD from pedidos",
                     selector_database_window.getDB());
        ui->tableWidget->horizontalHeaderItem(1)->setText("Proveedor");
        this->fillTable(q);
    }
}

void MainWindow::updateItem(int r, int c)
{
    if (auto_editing)
        return;

    QString id { ui->tableWidget->item(r, 0)->text()};
    QString id_consumidor {ui->tableWidget->item(r, 1)->text()};
    QString fecha_lanzamiento {ui->tableWidget->item(r, 2)->text()};
    QString cantidad {ui->tableWidget->item(r, 3)->text()};

    QSqlQuery q(selector_database_window.getDB());
    q.prepare("update pedidos set ID_CLIENTE=:id_cli, FECHA_LANZAMIENTO=:fech_lanz, CANTIDAD=:cant where ID_PEDIDO=:id_pedi");

        q.bindValue(":id_cli", id_consumidor);
        q.bindValue(":fech_lanz", fecha_lanzamiento);
        q.bindValue(":cant", cantidad);
        q.bindValue(":id_pedi", id);

    q.exec();
}


void MainWindow::fillTable(QSqlQuery& q)
{
    // CLEAR TABLE
    ui->tableWidget->setRowCount(0);

    auto_editing = true;
    while(q.next())
    {
        int row {ui->tableWidget->rowCount()};
        ui->tableWidget->insertRow(row);

        QTableWidgetItem* id_pedido = new QTableWidgetItem(q.value(0).toString());
        QTableWidgetItem* id_consumidor = new QTableWidgetItem(q.value(1).toString());
        QTableWidgetItem* fecha_lanzamiento = new QTableWidgetItem(q.value(2).toString());
        QTableWidgetItem* cantidad = new QTableWidgetItem(q.value(3).toString());

        id_pedido->setFlags(id_consumidor->flags() ^ Qt::ItemIsEditable);
        //id_consumidor->setFlags(id_consumidor->flags() ^ Qt::ItemIsEditable);
        //fecha_lanzamiento->setFlags(fecha_lanzamiento->flags() ^ Qt::ItemIsEditable);
        //cantidad->setFlags(cantidad->flags() ^ Qt::ItemIsEditable);

        id_pedido->setTextAlignment(Qt::AlignCenter);
        id_consumidor->setTextAlignment(Qt::AlignCenter);
        fecha_lanzamiento->setTextAlignment(Qt::AlignCenter);
        cantidad->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(row, 0, id_pedido);
        ui->tableWidget->setItem(row, 1, id_consumidor);
        ui->tableWidget->setItem(row, 2, fecha_lanzamiento);
        ui->tableWidget->setItem(row, 3, cantidad);
    }
    auto_editing = false;
}




