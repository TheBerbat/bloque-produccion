#include <selectdatabasedialog.h>
#include "ui_selectdatabasedialog.h"

#include <QFileDialog>
#include <QSqlDatabase>

SelectDatabaseDialog::SelectDatabaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDatabaseDialog),
    m_db(QSqlDatabase::addDatabase("QSQLITE", "current_db"))
{
    ui->setupUi(this);

    QObject::connect(ui->windowPath, &QPushButton::clicked,
                     this, &SelectDatabaseDialog::selectDBWindow);

    QObject::connect(ui->acceptButton, &QPushButton::clicked,
                     this, &SelectDatabaseDialog::connect_close);

    QObject::connect(ui->selectedPath, &QLineEdit::textChanged,
                     this, &SelectDatabaseDialog::filenameUpdated);

    QObject::connect(ui->cancelButton, &QPushButton::clicked,
                     this, &SelectDatabaseDialog::close);

    ui->acceptButton->setEnabled(false);
}

SelectDatabaseDialog::~SelectDatabaseDialog()
{
    delete ui;
}

QString SelectDatabaseDialog::filenameDB() const
{
    return ui->selectedPath->text();
}

void SelectDatabaseDialog::setFilenameDB(const QString& filenameDB)
{
    ui->selectedPath->setText(filenameDB);
}

void SelectDatabaseDialog::selectDBWindow()
{
    QFileDialog fd(this, tr("Open XML File 1"), filenameDB(), tr("SQlite Files (*.sqlite)"));
    fd.setFileMode(QFileDialog::ExistingFile);
    if(fd.exec())
    {
        QStringList fileNames = fd.selectedFiles();
        setFilenameDB(fileNames[0]);
    }
}

bool SelectDatabaseDialog::isValidFileName(const QString &filename)
{
    QFileInfo file(filename);
    return file.exists() && file.completeSuffix() == "sqlite";
}

void SelectDatabaseDialog::filenameUpdated(const QString &new_filename)
{
    if(isValidFileName(new_filename))
    {
        ui->selectedPath->setStyleSheet("border: 1px solid black");
        ui->acceptButton->setEnabled(true);
    }
    else
    {
        ui->selectedPath->setStyleSheet("border: 1px solid red");
        ui->acceptButton->setEnabled(false);
    }
}
void SelectDatabaseDialog::connect_close()
{
    qInfo() << "Clossing windows and connect";
    if (this->connectDB())
        qInfo() << "Successfully connected";
    else
        qInfo() << "Error connecting database";
    this->close();
}

bool SelectDatabaseDialog::connectDB()
{
    // Check if database file is selected correctly
    if ( !isValidFileName(filenameDB()) ) {
        qInfo() << "Database cannot be open";
        return false;
    }

    // Restart connection
    m_db.close();
    m_db.setDatabaseName(filenameDB());
    m_db.open();

    return database_valid();
}

void SelectDatabaseDialog::disconnectDB()
{
    m_db.close();
    m_db.setDatabaseName("");
}

bool SelectDatabaseDialog::database_valid() const
{
    return m_db.isValid() && m_db.isOpen();
}

QSqlDatabase SelectDatabaseDialog::getDB()
{
    return m_db;
}

