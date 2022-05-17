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

private slots:
    void connectDatabase();
    void disconnectDatabase();

private:
    Ui::MainWindow *ui;

    SelectDatabaseDialog selector_database_window;
};
#endif // MAINWINDOW_H
