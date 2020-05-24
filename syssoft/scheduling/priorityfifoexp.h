#ifndef PRIORITYFIFOEXP_H
#define PRIORITYFIFOEXP_H

#include "experiment.h"
#include "priorityfifoscheduler.h"

class PriorityFIFOExp : public Experiment
{
protected:
    PriorityFIFOScheduler* priorityFIFOScheduler;
public:
    PriorityFIFOExp(QueueSystemDraw* systemDraw, quint32 seed,
                    float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax);

    void setupScheduler() override;
    void setupDraw() override;
};

#endif // PRIORITYFIFOEXP_H
