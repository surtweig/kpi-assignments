#ifndef PLOTDRAW_H
#define PLOTDRAW_H

#include <QWidget>
#include <QPainter>
#include <QList>

class PlotDraw : public QWidget
{
    Q_OBJECT
public:
    explicit PlotDraw(QWidget *parent = nullptr);

    void SetBuffer(quint32 samplesCount, quint32 signalsCount);
    void ClearBuffer();
    void AddFrame(float*);
    void AddSample(float sample, quint32 signalIndex);

    void SetView(quint32 firstIndex, quint32 lastIndex);
    void SetView(quint32 firstIndex, quint32 lastIndex,
                 float low, float high);

    void SetBackground(QColor color);
    void SetSignalStyle(QColor color, float thickness);

    void paintEvent(QPaintEvent* event);

private:
    float* buffer;
    QBrush bg;
    QList<QPen> signalStyles;
    quint32 viewFirst;
    quint32 viewLast;
    float viewLow;
    float viewHigh;

signals:

};

#endif // PLOTDRAW_H
