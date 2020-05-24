#include "sjfexp.h"

SJFExp::SJFExp(QueueSystemDraw* systemDraw, quint32 seed,
               float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax)
    : Experiment(systemDraw, seed, taskRate, taskCostScale, taskCostPareto, taskCostMax)
{

}

void SJFExp::setupScheduler()
{
    scheduler = new SJFScheduler(systemDraw);
}

void SJFExp::setupDraw()
{
    systemDraw->AddQueue(Scheduler::IncomingQueueName, QPoint(50, 140), QSize(2, 0));
    systemDraw->AddQueue(Scheduler::CPUQueueName, QPoint(50, 320), QSize(2, 0));
    systemDraw->AddQueue(Scheduler::CompleteQueueName, QPoint(50, 500), QSize(2, 0), true);
}
