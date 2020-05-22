#include "mainwindow.h"
#include "ui_mainwindow.h"
#define RND(Low, High) qrand() % ((High + 1) - Low) + Low

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsd = new QueueSystemDraw(ui->DrawWidget);
    scheduler = new Scheduler(qsd);
    initSim();
    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(UpdateSim()));
    drawTimer->start(15);
}

void MainWindow::UpdateSim()
{
    scheduler->Tick();
    qsd->update();
}

void MainWindow::initSim()
{
    qsd->AddQueue(Scheduler::IncomingQueueName, QPoint(20, 20), QSize(0, 36));
    qsd->AddQueue(Scheduler::CPUQueueName, QPoint(200, 200), QSize(0, 36));
    qsd->AddQueue(Scheduler::CompleteQueueName, QPoint(400, 20), QSize(0, 8));
    /*
    qsd->AddQueue("queue1", QPoint(20, 20), QSize(0, 36));
    qsd->AddQueue("queue2", QPoint(200, 20), QSize(0, 36));

    for (int i = 0; i < 10; ++i)
    {
        Task t = Task(100, Task::Priority::TPNormal);
        qsd->AddTask(t, "queue1"); //QPoint(qsd->width()/2, qsd->height()/2)
    }
    for (int i = 0; i < 10; ++i)
    {
        Task t = Task(100, Task::Priority::TPNormal);
        qsd->AddTask(t, "queue2"); //QPoint(qsd->width()/2, qsd->height()/2)
    }
    */
}

void MainWindow::generateRandomTask()
{
    Task task = Task(RND(1, 100), Task::Priority::TPNormal);
    scheduler->AcceptTask(task);
}

MainWindow::~MainWindow()
{
    delete drawTimer;
    delete qsd;
    delete ui;
}


void MainWindow::on_CreateTaskBtn_clicked()
{
    generateRandomTask();
}
