#include "queuesystemdraw.h"

#define MAKE_QPOINTF_TRIANGLE(poly, x1, x2, basey, height) \
    poly[0] = QPointF(x1, basey); \
    poly[1] = QPointF(x2, basey); \
    poly[2] = QPointF((x1+x2)*0.5, basey-height);

QueueSystemDraw::QueueSystemDraw(QWidget *parent) : QWidget(parent)
{
    taskBackground = QBrush(QColor(200, 200, 255));
    taskProgressBackground = QBrush(QColor(20, 200, 50));
    prioLowBg = QBrush(QColor(50, 50, 200));
    prioHighBg = QBrush(QColor(200, 100, 0));
    taskPositionSpringFactor = 0.9;
    fillBackground = QBrush(QColor(Qt::white));
    defaultPen = QPen();
    noPen = QPen(Qt::PenStyle::NoPen);
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
    float px = position.x();
    float py = position.y();
    float w = size.width();
    float h = size.height();
    if (horizontal)
    {
        progress_rect = QRectF(
            px+1,
            py+1,
            progress * (w-2),
            h-2
        );
    }
    else
    {
        int ph = progress * (h-2);
        progress_rect = QRectF(
            px+1,
            py-1+h-ph,
            w-2,
            ph
        );
    }
    painter->drawRect(progress_rect);
    painter->drawText(rect, text);

    if (horizontal)
    {
    }
    else
    {
        painter->setPen(parent->noPen);
        QPointF poly[3];
        if (priority == Task::Priority::TPLow || priority == Task::Priority::TPLowest)
        {
            painter->setBrush(parent->prioLowBg);
            MAKE_QPOINTF_TRIANGLE(poly, px+4, px+w-4, py-4-w*0.5, -w*0.5)
            painter->drawPolygon(poly, 3);
            if (priority == Task::Priority::TPLowest)
            {
                MAKE_QPOINTF_TRIANGLE(poly, px+4, px+w-4, py-4-w, -w*0.5)
                painter->drawPolygon(poly, 3);
            }
        }
        else if (priority == Task::Priority::TPHigh || priority == Task::Priority::TPHighest)
        {
            painter->setBrush(parent->prioHighBg);
            MAKE_QPOINTF_TRIANGLE(poly, px+4, px+w-4, py-4, w*0.5)
            painter->drawPolygon(poly, 3);
            if (priority == Task::Priority::TPHighest)
            {
                MAKE_QPOINTF_TRIANGLE(poly, px+4, px+w-4, py-4-w*0.5, w*0.5)
                painter->drawPolygon(poly, 3);
            }
        }

        painter->setPen(parent->defaultPen);
    }
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
    int stepWSign = 0;
    if (step.width() > 0)
        stepWSign = 1;
    else
        stepWSign = -1;

    int stepHSign = 0;
    if (step.height() > 0)
        stepHSign = 1;
    else
        stepHSign = -1;

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
        if (step.width() != 0)
            xAccum += t->size.width()*stepWSign;
        else
            yAccum += t->size.height()*stepHSign;
        ++i;
    }
}

