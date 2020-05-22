#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include <QLinkedList>
#include <QMap>
#include <QString>
#include <QtGlobal>
#include <QList>
#include "iqueuesystemlistener.h"

class Scheduler
{
public:
    inline static const QString IncomingQueueName = "Incoming";
    inline static const QString CompleteQueueName = "Complete";
    inline static const QString CPUQueueName = "CPU";

protected:
    QList<Task> tasks;
    QMap<QString, QLinkedList<Task*>> queues;
    IQueueSystemListener* listener;
    QLinkedList<Task*>* incomingQueue;
    QLinkedList<Task*>* completeQueue;
    QLinkedList<Task*>* cpuQueue;

    bool moveTask(Task* task, QString qFrom, QString qTo);

public:
    Scheduler(IQueueSystemListener* listener);

    virtual void AcceptTask(Task);
    virtual void Tick();
    int GetQueueSize(QString queue);

    ~Scheduler();
};

#endif // SCHEDULER_H
