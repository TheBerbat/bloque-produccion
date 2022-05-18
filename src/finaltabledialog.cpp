#include <finaltabledialog.h>
#include "ui_finaltabledialog.h"

#include <cstdio>

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

void FinalTableDialog::setRow(std::vector<std::size_t> values, int row)
{
    int column {0};
    for(auto it : values)
        this->setItem(it, row, column++);
}

void FinalTableDialog::setItem(std::size_t value, int row, int column)
{
    QTableWidgetItem* n_item = new QTableWidgetItem(QString::number(value));
    n_item->setFlags(n_item->flags() ^ Qt::ItemIsEditable);
    n_item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, column, n_item);
}

void FinalTableDialog::setMRP(MRP_Algorithm::BaseMRP* mrp)
{
    // CLEAR TABLE
    ui->tableWidget->setColumnCount(mrp->get_ppl().size());

    setRow(mrp->get_needs()       , 0);
    setRow(mrp->get_availability(), 1);
    setRow(mrp->get_receptions()  , 2);
    setRow(mrp->get_net_needs()   , 3);
    setRow(mrp->get_ppl()         , 4);

    std::printf("%.2lfe\n", mrp->get_emision_costs());
    std::printf("%.2lfe\n", mrp->get_hold_costs());
    std::printf("%.2lfe\n", mrp->get_total_costs());
    std::fflush(stdout);
}

