#ifndef SJFSCHEDULER_H
#define SJFSCHEDULER_H

#include "scheduler.h"

class SJFScheduler : public Scheduler
{
public:
    SJFScheduler(IQueueSystemListener* listener);

    void Tick() override;
};

#endif // SJFSCHEDULER_H
