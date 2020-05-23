#include "fifoexp.h"

FIFOExp::FIFOExp(QueueSystemDraw* systemDraw, quint32 seed,
                 float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax):
    Experiment(systemDraw, seed, taskRate, taskCostScale, taskCostPareto, taskCostMax)
{

}

void FIFOExp::setupScheduler()
{
    scheduler = new Scheduler(systemDraw);
}

void FIFOExp::setupDraw()
{
    systemDraw->AddQueue(Scheduler::IncomingQueueName, QPoint(50, 120), QSize(2, 0));
    systemDraw->AddQueue(Scheduler::CPUQueueName, QPoint(50, 300), QSize(2, 0));
    systemDraw->AddQueue(Scheduler::CompleteQueueName, QPoint(50, 480), QSize(2, 0), true);
}
