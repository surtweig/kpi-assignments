#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fifoexp.h"
#include "priorityfifoexp.h"
#include "sjfexp.h"

#define RND(Low, High) qrand() % ((High + 1) - Low) + Low

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsd = new QueueSystemDraw(ui->DrawWidget);
    //scheduler = new Scheduler(qsd);
    initSim();
    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(UpdateSim()));
    drawTimer->start(15);

    simRunning = false;
    taskGeneratorSeed = 0;
    taskRate = 0.0025;
    taskCostScale = 10;
    taskCostPareto = 1;
    taskCostMax = 100;

    experiment = new FIFOExp(qsd, taskGeneratorSeed, taskRate, taskCostScale, taskCostPareto, taskCostMax);
    experiment->setupScheduler();
    experiment->setupDraw();
}

void MainWindow::UpdateSim()
{
    //const int completeQueueMaxVisible = 10;

    //scheduler->Tick();
    qsd->update();
    if (experiment != nullptr && simRunning)
        experiment->onUpdate();

    /*
    int complete = scheduler->GetQueueSize(Scheduler::CompleteQueueName);
    if (complete > completeQueueMaxVisible)
    {
        qsd->SetQueuePosition(Scheduler::CompleteQueueName, QPointF(20 - (complete-completeQueueMaxVisible)*18, 360));
    }
    */
}

void MainWindow::initSim()
{
    //qsd->AddQueue(Scheduler::IncomingQueueName, QPoint(20, 120), QSize(2, 0));
    //qsd->AddQueue(Scheduler::CPUQueueName, QPoint(200, 240), QSize(2, 0));
    //qsd->AddQueue(Scheduler::CompleteQueueName, QPoint(20, 360), QSize(2, 0));
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
    /*
    Task task = Task(RND(10, 100), Task::Priority::TPNormal);
    scheduler->AcceptTask(task);
    */
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

void MainWindow::on_StartStopBtn_clicked()
{
    simRunning = !simRunning;
    if (simRunning)
        ui->StartStopBtn->setText("Stop");
    else
        ui->StartStopBtn->setText("Start");
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    simRunning = false;
    ui->StartStopBtn->setText("Start");
    qsd->Clear();

    if (experiment)
        delete experiment;

    if (arg1 == "FIFO")
    {
        experiment = new FIFOExp(qsd, taskGeneratorSeed, taskRate, taskCostScale, taskCostPareto, taskCostMax);
    }
    else if (arg1 == "Priority FIFO")
    {
        experiment = new PriorityFIFOExp(qsd, taskGeneratorSeed, taskRate, taskCostScale, taskCostPareto, taskCostMax);
    }
    else if (arg1 == "Shortest job first")
    {
        experiment = new SJFExp(qsd, taskGeneratorSeed, taskRate, taskCostScale, taskCostPareto, taskCostMax);
    }
    experiment->setupScheduler();
    experiment->setupDraw();
}
