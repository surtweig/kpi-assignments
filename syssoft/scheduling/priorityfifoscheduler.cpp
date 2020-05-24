#include "priorityfifoscheduler.h"

PriorityFIFOScheduler::PriorityFIFOScheduler(IQueueSystemListener* listener)
    : Scheduler(listener)
{
    for (auto pi = PriorityNames.begin(); pi != PriorityNames.end(); ++pi)
    {
        queues[pi.value()] = QLinkedList<Task*>();
        priorityQueues[pi.key()] = &queues[pi.value()];
    }
}

void PriorityFIFOScheduler::AcceptTask(Task& task)
{
    tasks.push_back(task);
    priorityQueues[task.getPriority()]->push_back(&tasks.last());
    listener->AddTask(&tasks.last(), PriorityNames[task.getPriority()]);
}

void PriorityFIFOScheduler::Tick()
{
    if (cpuQueue->count() == 0)
    {
        for (int i = Task::Priority::TPHighest; i >= Task::Priority::TPLowest; --i)
        {
            Task::Priority prio = static_cast<Task::Priority>(i);
            if (priorityQueues[prio]->count() > 0)
            {
                moveTask(priorityQueues[prio]->first(), PriorityNames[prio], CPUQueueName);
                break;
            }
        }
    }

    updateCPUQueue();
}
