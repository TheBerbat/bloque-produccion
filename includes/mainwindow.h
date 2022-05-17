#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <selectdatabasedialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fillTable(QSqlQuery& q);

private slots:
    void connectDatabase();
    void disconnectDatabase();
    void updateTable(int idx);
    void updateItem(int r, int c);
    void insertItem(int status);

private:
    Ui::MainWindow *ui;

    SelectDatabaseDialog selector_database_window;

    bool auto_editing {false};
};
#endif // MAINWINDOW_H
