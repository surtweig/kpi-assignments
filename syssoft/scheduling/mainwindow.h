#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "queuesystemdraw.h"
#include "scheduler.h"
#include "experiment.h"

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
    void on_StartStopBtn_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    //Scheduler* scheduler;
    bool simRunning;
    QueueSystemDraw* qsd;
    QTimer* drawTimer;
    Experiment* experiment;
    void initSim();
    void generateRandomTask();
};
#endif // MAINWINDOW_H
