#include <mainwindow.h>
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

#include <selectdatabasedialog.h>
#include <insertdatadialog.h>
#include <algorithm/Batch2Batch.hpp>
#include <algorithm/Batch2BatchWithMin.hpp>
#include <algorithm/EOQ.hpp>
#include <algorithm/FixedPeriod.hpp>
#include <algorithm/FixedPeriodOptimal.hpp>
#include <algorithm/MinimumTotalCost.hpp>
#include <algorithm/MinimumUnitaryCost.hpp>
#include <algorithm/SilverMeal.hpp>

#include <aboutdialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selector_database_window(this)
    , table_window(this)
{
    ui->setupUi(this);

    disconnectDatabase();
    changedBatchMethod(0);

    QObject::connect(ui->actionConectar, &QAction::triggered,
                     this, &MainWindow::connectDatabase);

    QObject::connect(ui->actionDesconectar, &QAction::triggered,
                     this, &MainWindow::disconnectDatabase);

    QObject::connect(ui->selectorTable, &QComboBox::currentIndexChanged,
                     this, &MainWindow::updateTable);


    QObject::connect(ui->tableWidget, &QTableWidget::cellChanged,
                     this, &MainWindow::updateItem);
    QObject::connect(ui->tableWidget, &QTableWidget::cellClicked,
                     this, &MainWindow::clickDeleteItem);

    QObject::connect(ui->insertButton, &QPushButton::clicked,
                     this, &MainWindow::insertItem);

    QObject::connect(ui->calculateButton, &QPushButton::clicked,
                     this, &MainWindow::calculate);

    QObject::connect(ui->algorithmSelector, &QComboBox::currentIndexChanged,
                     this, &MainWindow::changedBatchMethod);

    QObject::connect(ui->actionInformacion, &QAction::triggered,
                     this, &MainWindow::showInformation);
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
        ui->insertButton->setEnabled(true);
        ui->calculateButton->setEnabled(true);
    }
    updateTable(ui->selectorTable->currentIndex());
}

void MainWindow::disconnectDatabase()
{
    qDebug() << "Disconnecting database";
    selector_database_window.disconnectDB();
    ui->actionConectar->setEnabled(true);
    ui->actionDesconectar->setEnabled(false);

    ui->selectorTable->setEnabled(false);
    ui->insertButton->setEnabled(false);
    ui->calculateButton->setEnabled(false);
    QSqlQuery q;
    fillTable(q);
}

void MainWindow::updateTable(int idx)
{
    if (idx == 0) {
        QSqlQuery q("select ID_PEDIDO, ID_CLIENTE, FECHA_LANZAMIENTO, CANTIDAD from pedidos",
                     selector_database_window.getDB());
        ui->tableWidget->horizontalHeaderItem(1)->setText("Cliente");
        this->fillTable(q);
    } else {
        QSqlQuery q("select ID_RECEPCION, ID_PROVEEDOR, FECHA, CANTIDAD from recepciones_programadas",
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

    bool isNum;
    int fecha_lanzamiento_num = fecha_lanzamiento.toInt(&isNum);
    if (!isNum || fecha_lanzamiento_num<=0 || fecha_lanzamiento_num>10)
    {
        QMessageBox messageBox;
        messageBox.critical(this,"Periodo no válido","No cumple con las condiciones de entrada. Debe ser un número entre 1 y 10 (ambos incluidos)");
        messageBox.setFixedSize(500,200);

        updateTable(ui->selectorTable->currentIndex());
        return;
    }
    int cantidad_num = cantidad.toInt(&isNum);
    if (!isNum || cantidad_num<0)
    {
        QMessageBox messageBox;
        messageBox.critical(this,"Cantidad de pedido no válida","No cumple con las condiciones de entrada. Debe ser un número positivo (incluyendo 0)");
        messageBox.setFixedSize(500,200);

        updateTable(ui->selectorTable->currentIndex());
        return;
    }


    QSqlQuery q(selector_database_window.getDB());
    if (ui->selectorTable->currentIndex() == 0)
    {
        q.prepare("update pedidos set ID_CLIENTE=:id_cli, FECHA_LANZAMIENTO=:fech_lanz, CANTIDAD=:cant where ID_PEDIDO=:id_pedi");

        q.bindValue(":id_cli", id_consumidor);
        q.bindValue(":fech_lanz", fecha_lanzamiento);
        q.bindValue(":cant", cantidad);
        q.bindValue(":id_pedi", id);
    }
    else if (ui->selectorTable->currentIndex() == 1)
    {
        q.prepare("update recepciones_programadas set ID_PROVEEDOR=:id_provee, FECHA=:fech_lanz, CANTIDAD=:cant where ID_RECEPCION=:id_recep");

        q.bindValue(":id_provee", id_consumidor);
        q.bindValue(":fech_lanz", fecha_lanzamiento);
        q.bindValue(":cant", cantidad);
        q.bindValue(":id_recep", id);
    }

    q.exec();
    updateTable(ui->selectorTable->currentIndex());
}

void MainWindow::insertItem(int status)
{
    qDebug()<<"Inserting new item in database";
    InsertDataDialog w(this);
    if (!w.exec())
        return;

    QSqlQuery q(selector_database_window.getDB());
    if (ui->selectorTable->currentIndex() == 0)
    {
        q.prepare("insert into pedidos (ID_CLIENTE, FECHA_LANZAMIENTO, CANTIDAD) values (:id_cli, :fech_lanz, :cant)");

        q.bindValue(":id_cli", w.getTarget());
        q.bindValue(":fech_lanz", w.getPeriod());
        q.bindValue(":cant", w.getSize());
    }
    else if (ui->selectorTable->currentIndex() == 1)
    {
        q.prepare("insert into recepciones_programadas (ID_PROVEEDOR, FECHA, CANTIDAD) values (:id_provee, :fech_lanz, :cant)");

        q.bindValue(":id_provee", w.getTarget());
        q.bindValue(":fech_lanz", w.getPeriod());
        q.bindValue(":cant", w.getSize());
    }

    q.exec();
    updateTable(ui->selectorTable->currentIndex());
}

void MainWindow::clickDeleteItem(int r, int c)
{
    if (!ui->deleteButton->isChecked())
        return;
    qDebug() << "Deleting row" << r;

    QString id { ui->tableWidget->item(r, 0)->text()};

    QSqlQuery q(selector_database_window.getDB());
    if (ui->selectorTable->currentIndex() == 0)
    {
        q.prepare("delete from pedidos where ID_PEDIDO=:id_pedi");

        q.bindValue(":id_pedi", id);
    }
    else if (ui->selectorTable->currentIndex() == 1)
    {
        q.prepare("delete from recepciones_programadas where ID_RECEPCION=:id_recepc");

        q.bindValue(":id_recepc", id);
    }

    q.exec();
    updateTable(ui->selectorTable->currentIndex());
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
    ui->tableWidget->setColumnHidden(0, true);
    auto_editing = false;
}

void MainWindow::changedBatchMethod(int state)
{
    switch (state)
    {
    case 1: // BATCH2BATCH with minimum
        ui->optionalValue->setVisible(true);
        ui->optionalValue->setMinimum(0);
        ui->optionalValue->setValue(0);
        ui->optionalLabel->setVisible(true);
        ui->optionalLabel->setText("Lote minimo");
        break;

    case 3: // Fixed Period
        ui->optionalValue->setVisible(true);
        ui->optionalValue->setMinimum(1);
        ui->optionalValue->setValue(1);
        ui->optionalLabel->setVisible(true);
        ui->optionalLabel->setText("Periodo fijo");
        break;

    case 0: // BATCH2BATCH
    case 2: // EOQ
    case 4: // Fixed Period Optimal
    case 5: // MinimumTotalCost
    case 6: // MinimumUnitaryCost
    case 7: // SilverMeal
        ui->optionalValue->setVisible(false);
        ui->optionalLabel->setVisible(false);
        break;

    default:
        qDebug() << "ERROR PLANNING, case not programmed";
        return;
    }
}

void MainWindow::calculate()
{
    // GET DATA FROM WINDOW
    //
    const int operationID { ui->algorithmSelector->currentIndex() };
    const std::size_t ss { static_cast<std::size_t>(ui->securityStock->value()) };
    const double hold_cost { ui->holdCost->value() };
    const double emision_cost { ui->emisionCost->value() };
    const std::size_t batch_min { static_cast<std::size_t>(ui->optionalValue->value()) };
    const std::size_t fixed_period { static_cast<std::size_t>(ui->optionalValue->value()) };
    const std::size_t period_ship { static_cast<std::size_t>(ui->shipTime->value()) };
    const std::size_t initial_availability { static_cast<std::size_t>(ui->initialAvailability->value()) };


    // GET PLANNING HORIZON FROM DATABASE
    //
    QSqlQuery q1("SELECT FECHA_LANZAMIENTO FROM pedidos ORDER BY FECHA_LANZAMIENTO DESC LIMIT 1", selector_database_window.getDB());

    std::size_t planning_horizon {1};
    if(q1.next())
        planning_horizon = q1.value(0).toULongLong();
    else
        qDebug()<< "WARNING: get top database. No elements in pedidos";

    QSqlQuery q2("SELECT FECHA FROM recepciones_programadas ORDER BY FECHA DESC LIMIT 1", selector_database_window.getDB());
    if(q2.next())
    {
        const std::size_t tmp { q2.value(0).toULongLong() };
        planning_horizon = std::max(tmp, planning_horizon);
    }
    else
        qDebug()<< "Error get top database. No elements in recepciones_programadas";

    // CREATION OF ALGORITHM PLANNER
    //
    std::unique_ptr<MRP_Algorithm::BaseMRP> p;

    switch (operationID)
    {
    case 0ull: // BATCH2BATCH
        p = std::make_unique<MRP_Algorithm::Batch2Batch>(planning_horizon, ss, emision_cost, hold_cost);
        break;

    case 1ull: // BATCH2BATCH with minimum
        p = std::make_unique<MRP_Algorithm::Batch2BatchWithMin>(batch_min, planning_horizon, ss, emision_cost, hold_cost);
        break;

    case 2ull: // EOQ
        p = std::make_unique<MRP_Algorithm::EOQ>(planning_horizon, ss, emision_cost, hold_cost);
        break;

    case 3ull: // Fixed Period
        p = std::make_unique<MRP_Algorithm::FixedPeriod>(fixed_period, planning_horizon, ss, emision_cost, hold_cost);
        break;

    case 4ull: // Fixed Period Optimal
        p = std::make_unique<MRP_Algorithm::FixedPeriodOptimal>(planning_horizon, ss, emision_cost, hold_cost);
        break;

    case 5ull: // MinimumTotalCost
        p = std::make_unique<MRP_Algorithm::MinimumTotalCost>(planning_horizon, ss, emision_cost, hold_cost);
        break;

    case 6ull: // MinimumUnitaryCost
        p = std::make_unique<MRP_Algorithm::MinimumUnitaryCost>(planning_horizon, ss, emision_cost, hold_cost);
        break;

    case 7ull: // SilverMeal
        p = std::make_unique<MRP_Algorithm::SilverMeal>(planning_horizon, ss, emision_cost, hold_cost);
        break;

    default:
        qDebug() << "ERROR PLANNING, case not programmed";
        return;
    }

    QSqlQuery q_orders("SELECT FECHA_LANZAMIENTO,SUM(CANTIDAD) FROM pedidos GROUP BY FECHA_LANZAMIENTO", selector_database_window.getDB());
    while(q_orders.next())
    {
        const std::size_t period {q_orders.value(0).toULongLong()};
        const std::size_t count {q_orders.value(1).toULongLong()};
        p->insert_need(period-1, count);
    }

    QSqlQuery q_receptions("SELECT FECHA,SUM(CANTIDAD) FROM recepciones_programadas GROUP BY FECHA", selector_database_window.getDB());
    while(q_receptions.next())
    {
        const std::size_t period {q_receptions.value(0).toULongLong()};
        const std::size_t count {q_receptions.value(1).toULongLong()};
        p->insert_receptions(period-1, count);
    }

    p->set_ship_period(period_ship);
    p->set_initial_availability(initial_availability);
    p->calculate();
    table_window.setMRP(p.get());
    table_window.show();

}

void MainWindow::showInformation()
{
    AboutDialog w(this);
    w.exec();
}


