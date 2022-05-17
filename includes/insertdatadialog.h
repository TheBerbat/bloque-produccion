#ifndef INSERTDATADIALOG_H
#define INSERTDATADIALOG_H

#include <QDialog>

namespace Ui {
class InsertDataDialog;
}

class InsertDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertDataDialog(QWidget *parent = nullptr);
    ~InsertDataDialog();

    QString getTarget() const;
    int getPeriod() const;
    int getSize() const;

private:
    Ui::InsertDataDialog *ui;
};

#endif // INSERTDATADIALOG_H
