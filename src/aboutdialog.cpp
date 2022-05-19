#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->label_email->setOpenExternalLinks(true);
    ui->label_repo->setOpenExternalLinks(true);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
