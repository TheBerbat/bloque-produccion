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

void FinalTableDialog::setRow(std::vector<std::size_t> values, int row, int zero_offset)
{
    int column {0};
    for (int i {} ; i<zero_offset ; ++i)
        this->setItem(0, row, column++);
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
    int size {static_cast<int>(mrp->get_ppl().size())};
    int ship_period {static_cast<int>(mrp->get_ship_period())};
    ui->tableWidget->setColumnCount(size + ship_period);

    setRow(mrp->get_needs()       , 0, ship_period);
    setRow(mrp->get_availability(), 1, ship_period);
    setRow(mrp->get_receptions()  , 2, ship_period);
    setRow(mrp->get_net_needs()   , 3, ship_period);
    setRow(mrp->get_ppl()         , 4, ship_period);
    setRow(mrp->get_lppl()        , 5);

    ui->emisionCost->setText(QString::number(mrp->get_emision_costs(), 'f', 2) + "€");
    ui->emisionCost->setReadOnly(true);
    ui->holdCost->setText(QString::number(mrp->get_hold_costs(), 'f', 2) + "€");
    ui->holdCost->setReadOnly(true);
    ui->totalCost->setText(QString::number(mrp->get_total_costs(), 'f', 2) + "€");
    ui->totalCost->setReadOnly(true);

    // Update header labels and hide negative columns if is all 0s
    for (int i {0} ; i<size+ship_period ; ++i)
    {
        const QString text {QString::number(i-ship_period+1)};
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(text));
        ui->tableWidget->setColumnHidden(i, false);
    }
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    for (int column {0} ; column<ship_period ; ++column)
    {
        bool hide {true};
        for (int row {0} ; row<6 ; ++row)
            if (ui->tableWidget->item(row, column)->text() != "0")
                hide = false;
        if (hide)
            ui->tableWidget->setColumnHidden(column, true);
        else
            break;
    }
}

