#ifndef PRIORITYFIFOSCHEDULER_H
#define PRIORITYFIFOSCHEDULER_H

#include "scheduler.h"

class PriorityFIFOScheduler : public Scheduler
{
private:
    QMap<Task::Priority, QLinkedList<Task*>*> priorityQueues;

public:
    inline static const QMap<Task::Priority, QString> PriorityNames = {
        {Task::Priority::TPLowest, "Lowest priority"},
        {Task::Priority::TPLow, "Low priority"},
        {Task::Priority::TPNormal, "Normal priority"},
        {Task::Priority::TPHigh, "High priority"},
        {Task::Priority::TPHighest, "Highest priority"},
    };

    PriorityFIFOScheduler(IQueueSystemListener* listener);

    void AcceptTask(Task&) override;
    void Tick() override;

};

#endif // PRIORITYFIFOSCHEDULER_H
