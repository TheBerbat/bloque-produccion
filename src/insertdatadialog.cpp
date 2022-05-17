#include "insertdatadialog.h"
#include "ui_insertdatadialog.h"

InsertDataDialog::InsertDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertDataDialog)
{
    ui->setupUi(this);
}

InsertDataDialog::~InsertDataDialog()
{
    delete ui;
}
