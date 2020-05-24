#ifndef SJFEXP_H
#define SJFEXP_H

#include "experiment.h"
#include "sjfscheduler.h"

class SJFExp : public Experiment
{
public:
    SJFExp(QueueSystemDraw* systemDraw, quint32 seed,
           float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax);

    void setupScheduler() override;
    void setupDraw() override;
};

#endif // SJFEXP_H
