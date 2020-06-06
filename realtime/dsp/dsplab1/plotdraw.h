#ifndef PLOTDRAW_H
#define PLOTDRAW_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include "discretesignal.h"

class PlotDraw : public QWidget
{    
    Q_OBJECT

public:
    struct SignalTransform
    {
        float offset;
        float scale;

        SignalTransform() : offset(0), scale(1) {}
    };

    enum SignalDrawType
    {
        Invisible,
        Line,
        VerticalBars,
        Dots
    };

    struct SignalStyle
    {
        SignalDrawType drawType;
        QPen pen;

        SignalStyle() : drawType(SignalDrawType::Line), pen() {}
    };

    struct SignalItem
    {
        DiscreteSignal* discreteSignal;
        SignalStyle style;
        SignalTransform transform;
    };

    explicit PlotDraw(QWidget *parent = nullptr);
    ~PlotDraw();

    /*
    void SetBuffer(quint32 samplesCount, quint32 signalsCount);
    void ClearBuffer();
    void AddFrame(float*);
    void SetSample(float sample, quint32 signalIndex);
    float GetSample(quint32 sampleIndex, quint32 signalIndex);
    void NextFrame();
    */

    quint32 AddSignal(DiscreteSignal* signal);
    void RemoveSignal(DiscreteSignal* signal);
    void RemoveSignal(quint32 signalIndex);

    void SetView(quint32 firstIndex, quint32 lastIndex);
    void SetView(quint32 firstIndex, quint32 lastIndex,
                 float low, float high);

    void SetBackground(QColor color);
    void SetSignalStyle(quint32 signalIndex, SignalDrawType drawType, QColor color, float thickness);
    void SetSignalTransform(quint32 signalIndex, float offset, float scale);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void paintEvent(QPaintEvent* event);

private:
    /*
    float* buffer;
    quint32 samplesMaxCount;
    quint32 samplesCurrentCount;
    quint32 signalsCount;
    size_t frameRingIndex;
    size_t frameSize;
    */
    QList<SignalItem> items;
    QBrush bg;
    quint32 viewFirst;
    quint32 viewLast;
    float viewLow;
    float viewHigh;
    QPointF* drawPointsBuffer;

    //quint32 sampleOffset(quint32 sampleIndex, quint32 signalIndex);
    QPointF sampleToScreen(quint32 sampleIndex, SignalItem &item);

    void drawSignalLine(QPainter &painter, SignalItem &item);
    void drawSignalVerticalBars(QPainter &painter, SignalItem &item);
    void drawSignalDots(QPainter &painter, SignalItem &item);

signals:

};

#endif // PLOTDRAW_H
