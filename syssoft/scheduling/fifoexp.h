#ifndef FIFOEXP_H
#define FIFOEXP_H

#include "experiment.h"

class FIFOExp : public Experiment
{
public:
    FIFOExp(QueueSystemDraw* systemDraw, quint32 seed,
            float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax);

    void setupScheduler() override;
    void setupDraw() override;

};

#endif // FIFOEXP_H
