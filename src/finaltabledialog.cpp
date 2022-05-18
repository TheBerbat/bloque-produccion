#include <finaltabledialog.h>
#include "ui_finaltabledialog.h"

FinalTableDialog::FinalTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinalTableDialog)
{
    ui->setupUi(this);
}

FinalTableDialog::~FinalTableDialog()
{
    delete ui;
}
