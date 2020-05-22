#ifndef IQUEUESYSTEMLISTENER_H
#define IQUEUESYSTEMLISTENER_H

#include "task.h"

class IQueueSystemListener
{
public:
    virtual void AddTask(const Task* source, QString queue) = 0;
    virtual void UpdateTask(const Task* source) = 0;
    virtual void MoveTask(const Task* source, QString queue) = 0;
};

#endif // IQUEUESYSTEMLISTENER_H
