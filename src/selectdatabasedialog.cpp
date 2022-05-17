#include "selectdatabasedialog.h"
#include "ui_selectdatabasedialog.h"

SelectDatabaseDialog::SelectDatabaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDatabaseDialog)
{
    ui->setupUi(this);
}

SelectDatabaseDialog::~SelectDatabaseDialog()
{
    delete ui;
}
