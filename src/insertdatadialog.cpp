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

QString InsertDataDialog::getTarget() const
{
    return ui->targetEdit->text();
}
int InsertDataDialog::getPeriod() const
{
    return ui->periodEdit->value();
}
int InsertDataDialog::getSize() const
{
    return ui->sizeEdit->value();
}
