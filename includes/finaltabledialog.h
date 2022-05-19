#ifndef FINALTABLEDIALOG_H
#define FINALTABLEDIALOG_H

#include <QDialog>
#include <algorithm/BaseMRP.hpp>

namespace Ui {
class FinalTableDialog;
}

class FinalTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FinalTableDialog(QWidget *parent = nullptr);
    ~FinalTableDialog();

    void setMRP(MRP_Algorithm::BaseMRP* mrp);
private:
    void setRow(std::vector<std::size_t> values, int row, int zero_offset=0);
    void setItem(std::size_t value, int row, int column);

private:
    Ui::FinalTableDialog *ui;
};

#endif // FINALTABLEDIALOG_H
