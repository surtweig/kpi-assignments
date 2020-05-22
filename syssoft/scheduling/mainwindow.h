#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "queuesystemdraw.h"
#include "scheduler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void UpdateSim();

private slots:
    void on_CreateTaskBtn_clicked();

private:
    Ui::MainWindow *ui;
    Scheduler* scheduler;
    QueueSystemDraw* qsd;
    QTimer* drawTimer;

    void initSim();
    void generateRandomTask();
};
#endif // MAINWINDOW_H
