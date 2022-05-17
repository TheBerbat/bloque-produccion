#ifndef SELECTDATABASEDIALOG_H
#define SELECTDATABASEDIALOG_H

#include <QDialog>

namespace Ui {
class SelectDatabaseDialog;
}

class SelectDatabaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDatabaseDialog(QWidget *parent = nullptr);
    ~SelectDatabaseDialog();

private:
    Ui::SelectDatabaseDialog *ui;
};

#endif // SELECTDATABASEDIALOG_H
