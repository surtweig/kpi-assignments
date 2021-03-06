#ifndef QUEUESYSTEMDRAW_H
#define QUEUESYSTEMDRAW_H

#include <QWidget>
#include <QPainter>
#include <QLinkedList>
#include <QList>
#include <QMap>
#include "task.h"
#include "iqueuesystemlistener.h"

class QueueSystemDraw : public QWidget, public IQueueSystemListener
{
    Q_OBJECT

public:
    explicit QueueSystemDraw(QWidget *parent = nullptr);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void paintEvent(QPaintEvent* event);

    void AddQueue(QString name, QPointF position, QSize step, bool inverse = false);
    void SetQueuePosition(QString name, QPointF position);
    QPointF GetQueuePosition(QString name);
    void AddTask(const Task* source, QString queue);
    void UpdateTask(const Task* source);
    void MoveTask(const Task* source, QString queue);
    void Clear();

public slots:
    void update();



private:
    QBrush fillBackground;
    QBrush taskBackground;
    QBrush taskProgressBackground;
    QBrush prioLowBg;
    QBrush prioHighBg;
    QPen defaultPen;
    QPen noPen;
    float taskPositionSpringFactor;

    class TaskDraw
    {
    private:
        QueueSystemDraw* parent;
        inline static QPointF prioTrianglePoly[3];
    public:
        QPointF position;
        bool horizontal;
        QSize size;
        float progress;
        Task::Priority priority;
        QString text;

        TaskDraw();
        TaskDraw(QueueSystemDraw* parent, QPointF position, Task::Priority priority, int cost, bool horizontal);
        void Draw(QPainter* painter);
    };

    class QueueDraw
    {
    private:
        QueueSystemDraw* parent;
    public:
        QLinkedList<TaskDraw*> tasks;
        QString name;
        QPointF position;
        QSize step;
        bool inverse;

        QueueDraw();
        QueueDraw(QueueSystemDraw* parent, QString name, QPointF position, QSize step, bool inverse);
        void AddTask(TaskDraw* task);
        bool RemoveTask(TaskDraw* task);
        void Draw(QPainter* painter);
        void Update();
    };

    QMap<quint32, TaskDraw> tasks;
    QMap<QString, QueueDraw> queues;

signals:

};

#endif // QUEUESYSTEMDRAW_H
