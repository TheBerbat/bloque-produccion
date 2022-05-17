#include <mainwindow.h>
#include "ui_mainwindow.h"

#include <QSqlDatabase>

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
    }
}

void MainWindow::disconnectDatabase()
{
    qDebug() << "Disconnecting database";
    selector_database_window.disconnectDB();
    ui->actionConectar->setEnabled(true);
    ui->actionDesconectar->setEnabled(false);
}




