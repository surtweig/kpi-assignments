#ifndef TASK_H
#define TASK_H

#include <QtGlobal>

class Task
{
public:
    enum Priority
    {
        TPLowest,
        TPLow,
        TPNormal,
        TPHigh,
        TPHighest
    };

private:
    inline static quint32 TaskIDCounter = 0;

    quint32 id;
    quint32 cost;
    quint32 progress;
    Priority priority;

public:
    Task(quint32 cost, Priority priority)
    {
        this->cost = cost;
        this->priority = priority;
        id = TaskIDCounter++;
        progress = 0;
    }

    quint32 getId() const { return id; }
    quint32 getCost() const { return cost; }
    quint32 getProgress() const { return progress; }
    Priority getPriority() const { return priority; }

    void Tick()
    {
        ++progress;
        if (progress > cost)
            progress = cost;
    }

    bool Complete() const { return progress == cost; }
};

#endif // TASK_H
