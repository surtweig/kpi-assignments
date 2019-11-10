#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void deinit();

    void centerAndResize();
    void refreshDbView();

private slots:
    void on_AddRecordButton_clicked();

private:
    Ui::MainWindow *ui;
    Database* db;
};
#endif // MAINWINDOW_H
