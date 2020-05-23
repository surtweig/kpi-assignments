#include "experiment.h"

Experiment::Experiment(QueueSystemDraw* systemDraw, quint32 seed,
                       float taskRate, float taskCostScale, float taskCostPareto, quint32 taskCostMax)
{
    this->systemDraw = systemDraw;
    scheduler = nullptr;
    random = QRandomGenerator(seed);
    this->taskRate = taskRate;
    this->taskCostScale = taskCostScale;
    this->taskCostMax = taskCostMax;
    this->taskCostPareto = taskCostPareto;
    nextTaskTimestamp = 0;
}

float Experiment::Pareto(float scale, float shape)
{
    float U = random.generateDouble();
    if (U > __FLT_EPSILON__)
        return scale / (powf(U, 1.0/shape));
    return 0;
}

float Experiment::PoissonTime(float rate)
{
    float U = random.generateDouble();
    if (U > __FLT_EPSILON__)
        return -logf(U)/rate;
    return 0;
}

Task::Priority Experiment::RandomPriority()
{
    float U = 2.0*random.generateDouble() - 1.0;

    if (abs(U) <= NormalPriorityProb)
        return Task::Priority::TPNormal;

    if (abs(U) <= NormalPriorityProb + LowOrHighPriorityProb)
    {
        if (U >= 0)
            return Task::Priority::TPHigh;
        else
            return Task::Priority::TPLow;
    }

    if (U >= 0)
        return Task::Priority::TPHighest;
    else
        return Task::Priority::TPLowest;
}

void Experiment::onUpdate()
{
    if (scheduler)
    {
        generateTask();
        scheduler->Tick();
        systemDraw->update();
    }
}

void Experiment::generateTask()
{
    quint64 ct = QDateTime::currentMSecsSinceEpoch();
    if (ct >= nextTaskTimestamp || nextTaskTimestamp == 0)
    {
        Task task = Task(
                    qMin(1 + (quint32)Pareto(taskCostScale, taskCostPareto), taskCostMax),
                    RandomPriority()
                    );
        scheduler->AcceptTask(task);
        nextTaskTimestamp = ct + qMin((quint64)PoissonTime(taskRate), (quint64)3000);
    }
}
