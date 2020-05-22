#include "scheduler.h"
#include <QMutableLinkedListIterator>

Scheduler::Scheduler(IQueueSystemListener* listener)
{
    this->listener = listener;

    queues[IncomingQueueName] = QLinkedList<Task*>();
    incomingQueue = &queues[IncomingQueueName];

    queues[CompleteQueueName] = QLinkedList<Task*>();
    completeQueue = &queues[CompleteQueueName];

    queues[CPUQueueName] = QLinkedList<Task*>();
    cpuQueue = &queues[CPUQueueName];
}

bool Scheduler::moveTask(Task* task, QString qFrom, QString qTo)
{
    QLinkedList<Task*>* fromQueue = &queues[qFrom];
    QLinkedList<Task*>* toQueue = &queues[qTo];

    if (fromQueue->contains(task))
    {
        fromQueue->removeOne(task);
        toQueue->push_back(task);
        listener->MoveTask(task, qTo);
        return true;
    }
    return false;
}

void Scheduler::AcceptTask(Task newTask)
{
    tasks.push_back(newTask);
    incomingQueue->push_back(&tasks.last());
    listener->AddTask(&tasks.last(), IncomingQueueName);
}

void Scheduler::Tick()
{
    if (cpuQueue->count() == 0 && incomingQueue->count() > 0)
    {
        moveTask(incomingQueue->first(), IncomingQueueName, CPUQueueName);
    }

    QMutableLinkedListIterator<Task*> ti(*cpuQueue);
    //for (auto ti = cpuQueue->begin(); ti != cpuQueue->end(); ++ti)
    while (ti.hasNext())
    {
        Task* t = ti.next();
        t->Tick();
        listener->UpdateTask(t);
        if (t->Complete())
            moveTask(t, CPUQueueName, CompleteQueueName);
    }

}

int Scheduler::GetQueueSize(QString queue)
{
    return queues[queue].count();
}

Scheduler::~Scheduler()
{

}
