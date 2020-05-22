#include "queuesystemdraw.h"

QueueSystemDraw::QueueSystemDraw(QWidget *parent) : QWidget(parent)
{
    taskBackground = QBrush(QColor(200, 200, 255));
    taskProgressBackground = QBrush(QColor(20, 200, 50));
    taskPositionSpringFactor = 0.9;
    fillBackground = QBrush(QColor(Qt::white));
}

void QueueSystemDraw::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QRect screenRect = QRect(0, 0, width(), height());
    painter.fillRect(screenRect, fillBackground);
    painter.setRenderHint(QPainter::Antialiasing);
    for (auto qi = queues.begin(); qi != queues.end(); ++qi)
        qi.value().Draw(&painter);
}


QSize QueueSystemDraw::sizeHint() const
{
    return parentWidget()->size();
}

QSize QueueSystemDraw::minimumSizeHint() const
{
    return parentWidget()->size();
}

void QueueSystemDraw::AddQueue(QString name, QPoint position, QSize step)
{
    queues[name] = QueueDraw(this, name, position, step);
}

void QueueSystemDraw::AddTask(const Task* source, QString queue)
{
    tasks[source->getId()] = TaskDraw(this, QPointF(-100, -100), source->getPriority());
    queues[queue].AddTask(&tasks[source->getId()]);
}

void QueueSystemDraw::UpdateTask(const Task* source)
{
    tasks[source->getId()].progress = (float)source->getProgress() / source->getCost();
}

void QueueSystemDraw::MoveTask(const Task* source, QString queue)
{
    TaskDraw* task = &tasks[source->getId()];
    for (auto qi = queues.begin(); qi != queues.end(); ++qi)
    {
        if (qi.value().RemoveTask(task))
        {
            break;
        }
    }
    queues[queue].AddTask(task);
}

void QueueSystemDraw::update()
{
    for (auto qi = queues.begin(); qi != queues.end(); ++qi)
        qi.value().Update();
    repaint();
}

QueueSystemDraw::TaskDraw::TaskDraw()
{
    parent = nullptr;
}

QueueSystemDraw::TaskDraw::TaskDraw(QueueSystemDraw* parent, QPointF position, Task::Priority priority)
{
    this->position = position;
    size = QSize(32, 32);
    this->priority = priority;
    progress = 0.0;
    text = "";
    this->parent = parent;
}

void QueueSystemDraw::TaskDraw::Draw(QPainter* painter)
{
    QRectF rect = QRectF(position, size);
    painter->setBrush(parent->taskBackground);
    painter->drawRect(rect);
    painter->setBrush(parent->taskProgressBackground);
    QRectF progress_rect = QRectF(
        position.x()+1,
        position.y()+1,
        progress * (size.width()-2),
        size.height()-2
    );
    painter->drawRect(progress_rect);
    painter->drawText(rect, text);
}

QueueSystemDraw::QueueDraw::QueueDraw()
{
    name = "?";
    parent = nullptr;
}

QueueSystemDraw::QueueDraw::QueueDraw(QueueSystemDraw* parent, QString name, QPointF position, QSize step)
{
    this->name = name;
    this->position = position;
    this->step = step;
    this->parent = parent;
}

void QueueSystemDraw::QueueDraw::AddTask(TaskDraw* task)
{
    tasks.append(task);
}

bool QueueSystemDraw::QueueDraw::RemoveTask(TaskDraw* task)
{
    for (auto ti = tasks.begin(); ti != tasks.end(); ++ti)
    {
        if ((*ti) == task)
        {
            tasks.erase(ti);
            return true;
        }
    }
    return false;
}

void QueueSystemDraw::QueueDraw::Draw(QPainter* painter)
{
    for (auto ti = tasks.begin(); ti != tasks.end(); ++ti)
    {
        (*ti)->Draw(painter);
    }
    //int count = tasks.count();
}

void QueueSystemDraw::QueueDraw::Update()
{
    int i = 0;
    for (auto ti = tasks.begin(); ti != tasks.end(); ++ti)
    {
        float tx = position.x() + i * step.width();
        float ty = position.y() + i * step.height();
        float px = (*ti)->position.x();
        float py = (*ti)->position.y();
        (*ti)->position.setX(px*parent->taskPositionSpringFactor + tx*(1.0 - parent->taskPositionSpringFactor));
        (*ti)->position.setY(py*parent->taskPositionSpringFactor + ty*(1.0 - parent->taskPositionSpringFactor));
        ++i;
    }
}

