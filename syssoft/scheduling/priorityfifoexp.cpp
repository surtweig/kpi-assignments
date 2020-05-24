#include "priorityfifoexp.h"

PriorityFIFOExp::PriorityFIFOExp(QueueSystemDraw* systemDraw, quint32 seed,
                                 float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax)
    : Experiment(systemDraw, seed, taskRate, taskCostScale, taskCostPareto, taskCostMax)
{

}

void PriorityFIFOExp::setupScheduler()
{
    priorityFIFOScheduler = new PriorityFIFOScheduler(systemDraw);
    scheduler = priorityFIFOScheduler;
}

void PriorityFIFOExp::setupDraw()
{
    //systemDraw->AddQueue(Scheduler::IncomingQueueName, QPoint(50, 140), QSize(2, 0));
    systemDraw->AddQueue(Scheduler::CPUQueueName, QPoint(500, 420), QSize(2, 0));
    systemDraw->AddQueue(Scheduler::CompleteQueueName, QPoint(500, 650), QSize(-2, 0), true);

    for (int i = Task::Priority::TPLowest; i <= Task::Priority::TPHighest; ++i)
    {
        Task::Priority prio = static_cast<Task::Priority>(i);
        systemDraw->AddQueue(PriorityFIFOScheduler::PriorityNames[prio], QPointF(700, 100 + i*160), QSize(2, 0));
    }
}
