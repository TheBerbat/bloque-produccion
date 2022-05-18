#ifndef FINALTABLEDIALOG_H
#define FINALTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class FinalTableDialog;
}

class FinalTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FinalTableDialog(QWidget *parent = nullptr);
    ~FinalTableDialog();

private:
    Ui::FinalTableDialog *ui;
};

#endif // FINALTABLEDIALOG_H
