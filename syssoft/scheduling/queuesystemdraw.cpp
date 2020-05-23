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

void QueueSystemDraw::AddQueue(QString name, QPointF position, QSize step, bool inverse)
{
    queues[name] = QueueDraw(this, name, position, step, inverse);
}

void QueueSystemDraw::SetQueuePosition(QString name, QPointF position)
{
    queues[name].position = position;
}

QPointF QueueSystemDraw::GetQueuePosition(QString name)
{
    return queues[name].position;
}

void QueueSystemDraw::AddTask(const Task* source, QString queue)
{
    bool hor = queues[queue].step.height() != 0;
    tasks[source->getId()] = TaskDraw(this, QPointF(-100, -100), source->getPriority(), source->getCost(), hor);
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

void QueueSystemDraw::Clear()
{
    tasks.clear();
    queues.clear();
    repaint();
}

void QueueSystemDraw::update()
{
    repaint();
    for (auto qi = queues.begin(); qi != queues.end(); ++qi)
        qi.value().Update();
}

QueueSystemDraw::TaskDraw::TaskDraw()
{
    parent = nullptr;
}

QueueSystemDraw::TaskDraw::TaskDraw(QueueSystemDraw* parent, QPointF position, Task::Priority priority, int cost, bool horizontal)
{
    this->position = position;
    this->horizontal = horizontal;
    if (horizontal)
        size = QSize(cost, 16);
    else
        size = QSize(16, cost);

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
    QRectF progress_rect;
    if (horizontal)
    {
        progress_rect = QRectF(
            position.x()+1,
            position.y()+1,
            progress * (size.width()-2),
            size.height()-2
        );
    }
    else
    {
        int h = progress * (size.height()-2);
        progress_rect = QRectF(
            position.x()+1,
            position.y()-1+size.height()-h,
            size.width()-2,
            h
        );
    }
    painter->drawRect(progress_rect);
    painter->drawText(rect, text);
}

QueueSystemDraw::QueueDraw::QueueDraw()
{
    name = "?";
    parent = nullptr;
}

QueueSystemDraw::QueueDraw::QueueDraw(QueueSystemDraw* parent, QString name, QPointF position, QSize step, bool inverse)
{
    this->name = name;
    this->position = position;
    this->step = step;
    this->parent = parent;
    this->inverse = inverse;
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
    int tasksWidth = 0;
    int tasksHeight = 0;
    int tasksMaxWidth = 0;
    int tasksMaxHeight = 0;
    for (auto ti = tasks.begin(); ti != tasks.end(); ++ti)
    {        
        TaskDraw* t = *ti;
        t->Draw(painter);
        tasksWidth += t->size.width();
        tasksHeight += t->size.height();
        tasksMaxWidth = qMax(t->size.width(), tasksMaxWidth);
        tasksMaxHeight = qMax(t->size.height(), tasksMaxHeight);
    }

    if (step.width() > 0)
    {
        //QPointF firstTaskPos = tasks.first()->position;
        //QPointF lastTaskPos = tasks.last()->position;
        int count = tasks.count();
        int lineWidth = qMax(16, tasksWidth + (count-1)*step.width());

        painter->drawLine(position.x(), position.y() + 5,
                          position.x(), position.y() + 10);

        painter->drawLine(position.x(), position.y() + 10,
                          position.x() + lineWidth, position.y() + 10);

        painter->drawLine(position.x() + lineWidth, position.y() + 10,
                          position.x() + lineWidth, position.y() + 5);

        QFontMetrics fm(painter->font());
        int textWidth=fm.horizontalAdvance(name);
        painter->drawText(QPointF(position.x() + lineWidth*0.5 - textWidth*0.5, position.y() + 15 + fm.height()), name);
    }

}

void QueueSystemDraw::QueueDraw::Update()
{
    int i = 0;
    int xAccum = 0;
    int yAccum = 0;
    int count = tasks.count();
    QLinkedListIterator<TaskDraw*> ti(tasks);
    bool iterate;
    if (inverse)
    {
        ti.toBack();
        iterate = ti.hasPrevious();
    }
    else
    {
        ti.toFront();
        iterate = ti.hasNext();
    }
    //for (auto ti = tasks.begin(); ti != tasks.end(); ++ti)
    while (iterate)
    {
        TaskDraw* t;// = *ti;
        if (inverse)
        {
            t = ti.previous();
            iterate = ti.hasPrevious();
        }
        else
        {
            t = ti.next();
            iterate = ti.hasNext();
        }
        float tx = position.x() + i * step.width() + xAccum;
        float ty = position.y() + i * step.height() + yAccum - t->size.height();
        float px = t->position.x();
        float py = t->position.y();
        t->position.setX(px*parent->taskPositionSpringFactor + tx*(1.0 - parent->taskPositionSpringFactor));
        t->position.setY(py*parent->taskPositionSpringFactor + ty*(1.0 - parent->taskPositionSpringFactor));
        if (step.width() > 0)
            xAccum += t->size.width();
        else
            yAccum += t->size.height();
        ++i;
    }
}

