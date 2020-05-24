#include "sjfscheduler.h"

SJFScheduler::SJFScheduler(IQueueSystemListener* listener)
    : Scheduler(listener)
{

}

void SJFScheduler::Tick()
{
    if (cpuQueue->count() == 0 && incomingQueue->count() > 0)
    {
        Task* shortestJob = incomingQueue->first();
        for (auto ti = incomingQueue->begin(); ti != incomingQueue->end(); ++ti)
        {
            if ((*ti)->getCost() < shortestJob->getCost())
                shortestJob = *ti;
        }
        moveTask(shortestJob, IncomingQueueName, CPUQueueName);
    }

    updateCPUQueue();
}
