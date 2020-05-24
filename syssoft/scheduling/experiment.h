#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QMap>
#include <QDateTime>
#include <QRandomGenerator>
#include <QtGlobal>
#include <QtMath>

#include "scheduler.h"
#include "queuesystemdraw.h"

class Experiment
{
private:
    const float NormalPriorityProb = 0.5;
    const float LowOrHighPriorityProb = 0.4;

    QMap<quint32, quint64> taskStartTimestamp;
    QMap<quint32, quint64> taskFinishTimestamp;
    float taskRate;
    float taskCostScale;
    float taskCostPareto;
    quint32 taskCostMax;
    quint64 nextTaskTimestamp;

protected:
    QueueSystemDraw* systemDraw;
    Scheduler* scheduler;
    QRandomGenerator random;

    void generateTask();
    float Pareto(float scale, float shape);
    float PoissonTime(float rate);
    Task::Priority RandomPriority();

public:
    Experiment(QueueSystemDraw* systemDraw, quint32 seed,
               float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax);

    virtual void setupScheduler() = 0;
    virtual void setupDraw() = 0;
    virtual void onUpdate();
    Scheduler* getScheduler() { return scheduler; }
};

#endif // EXPERIMENT_H
