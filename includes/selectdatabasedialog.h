#ifndef SELECTDATABASEDIALOG_H
#define SELECTDATABASEDIALOG_H

#include <QDialog>

#include <QSqlDatabase>

namespace Ui {
class SelectDatabaseDialog;
}

class SelectDatabaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDatabaseDialog(QWidget *parent = nullptr);
    ~SelectDatabaseDialog();

    QString filenameDB() const;
    void setFilenameDB(const QString& filenameDB);
    void selectDBWindow();

    static bool isValidFileName(const QString &filename);
    bool connectDB();
    void disconnectDB();
    bool database_valid() const;
    QSqlDatabase getDB();

private slots:
    void filenameUpdated(const QString &new_filename);
    void connect_close();

private:
    Ui::SelectDatabaseDialog *ui;
    QSqlDatabase m_db;
};

#endif // SELECTDATABASEDIALOG_H
