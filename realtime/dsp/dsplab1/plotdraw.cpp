#include "plotdraw.h"

PlotDraw::PlotDraw(QWidget *parent) : QWidget(parent)
{
    //buffer = nullptr;
    items = QList<SignalItem>();
    drawPointsBuffer = nullptr;
    bg = QBrush();
    viewFirst = 0;
    viewLast = 0;
    viewLow = 0;
    viewHigh = 1;
    axisPen = QPen(QColor(128, 128, 128));
    /*
    samplesCurrentCount = 0;
    samplesMaxCount = 0;
    signalsCount = 0;
    frameRingIndex = 0;
    */
}

PlotDraw::~PlotDraw()
{
    //free(buffer);
    free(drawPointsBuffer);
}

/*
void PlotDraw::SetBuffer(quint32 samplesCount, quint32 signalsCount)
{
    if (buffer)
        free(buffer);

    if (drawPointsBuffer)
        free(drawPointsBuffer);

    this->samplesCurrentCount = 0;
    this->samplesMaxCount = samplesCount;
    this->signalsCount = signalsCount;
    frameSize = signalsCount*sizeof(float);
    buffer = (float*)malloc(samplesCount*frameSize);
    frameRingIndex = 0;

    drawPointsBuffer = (QPointF*)malloc((samplesCount+1)*sizeof(QPointF));

    signalStyles.resize(signalsCount);
    signalTransforms.resize(signalsCount);
}

void PlotDraw::ClearBuffer()
{
    this->samplesCurrentCount = 0;
    frameRingIndex = 0;
}

void PlotDraw::AddFrame(float* signalsSamples)
{
    //float* framePtr = (float*)((size_t)buffer + frameRingIndex*frameSize);
    memcpy(&buffer[frameRingIndex*signalsCount], signalsSamples, signalsCount);
    NextFrame();
}

void PlotDraw::NextFrame()
{
    ++frameRingIndex;
    if (frameRingIndex >= samplesMaxCount)
        frameRingIndex = 0;

    if (samplesCurrentCount < samplesMaxCount)
        ++samplesCurrentCount;
}

void PlotDraw::SetSample(float sample, quint32 signalIndex)
{
    buffer[frameRingIndex*signalsCount + signalIndex] = sample;
}

float PlotDraw::GetSample(quint32 sampleIndex, quint32 signalIndex)
{
    return buffer[sampleOffset(sampleIndex, signalIndex)];
}

quint32 PlotDraw::sampleOffset(quint32 sampleIndex, quint32 signalIndex)
{
    assert(sampleIndex < samplesCurrentCount);

    quint32 firstSampleIndex = 0;
    if (samplesCurrentCount == samplesMaxCount)
        firstSampleIndex = (frameRingIndex + 1) % samplesMaxCount;
    quint32 sampleRingIndex = (firstSampleIndex + sampleIndex) % samplesMaxCount;
    return sampleRingIndex*signalsCount + signalIndex;
}
*/

quint32 PlotDraw::AddSignal(DiscreteSignal* signal)
{
    SignalItem item = SignalItem();
    item.discreteSignal = signal;
    items.push_back(item);

    return items.size()-1;
}

void PlotDraw::RemoveSignal(DiscreteSignal* signal)
{
    for (int i = 0; i < items.size(); ++i)
        if (items[i].discreteSignal == signal)
        {
            items.removeAt(i);
            break;
        }
}

void PlotDraw::RemoveSignal(quint32 signalIndex)
{
    if (signalIndex < items.size())
        items.removeAt(signalIndex);
}

void PlotDraw::SetView(quint32 firstIndex, quint32 lastIndex)
{
    this->viewFirst = firstIndex;
    this->viewLast = lastIndex;
    if (drawPointsBuffer)
        free(drawPointsBuffer);
    drawPointsBuffer = (QPointF*)malloc((lastIndex-firstIndex+1)*sizeof(QPointF));
}

void PlotDraw::SetView(quint32 firstIndex, quint32 lastIndex,
             float low, float high)
{
    SetView(firstIndex, lastIndex);
    viewLow = low;
    viewHigh = high;
}

void PlotDraw::SetBackground(QColor color)
{
    bg = QBrush(color);
}

void PlotDraw::SetSignalStyle(quint32 signalIndex, SignalDrawType drawType, QColor color, float thickness)
{
    items[signalIndex].style.pen = QPen(color);
    items[signalIndex].style.pen.setWidthF(thickness);
    items[signalIndex].style.drawType = drawType;
}

void PlotDraw::SetSignalTransform(quint32 signalIndex, float offset, float scale)
{
    items[signalIndex].transform.offset = offset;
    items[signalIndex].transform.scale = scale;
}

QSize PlotDraw::sizeHint() const
{
    return parentWidget()->size();
}

QSize PlotDraw::minimumSizeHint() const
{
    return parentWidget()->size();
}

void PlotDraw::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect bgRect = QRect(0, 0, width(), height());
    painter.fillRect(bgRect, bg);
    painter.setRenderHint(QPainter::Antialiasing);

    float zy = - viewLow / (viewHigh-viewLow);
    int pzy = (1-zy)*height();
    painter.setPen(axisPen);
    painter.drawLine(0, pzy, width(), pzy);

    for (int i = 0; i < items.size(); ++i)
    {
        switch (items[i].style.drawType)
        {
            case SignalDrawType::Line:
                drawSignalLine(painter, items[i]);
                break;

            default:
                break;
        }
    }
}

QPointF PlotDraw::sampleToScreen(quint32 sampleIndex, SignalItem &item)
{

    float sample = item.transform.offset +
                   item.transform.scale *
                   item.discreteSignal->GetSample(sampleIndex);//GetSample(sampleIndex, item);

    float x = ((float)sampleIndex - (float)viewFirst)/(float)(viewLast-viewFirst);
    float y = (sample - viewLow)/(viewHigh-viewLow);

    return QPointF(x*width(), (1-y)*height());
}

void PlotDraw::drawSignalLine(QPainter &painter, SignalItem &item)
{
    QPointF prevPoint(0, 0);
    painter.setPen(item.style.pen);
    quint32 pointsCount = 0;
    for (quint32 i = viewFirst; i < viewLast; ++i)
    {
        if (i >= item.discreteSignal->Size())
            break;

        QPointF point = sampleToScreen(i, item);
        drawPointsBuffer[pointsCount++] = point;
        if (i > 0)
            painter.drawLine(prevPoint, point);
        prevPoint = point;
    }
    painter.drawPolyline(drawPointsBuffer, pointsCount);
}

void PlotDraw::drawSignalVerticalBars(QPainter &painter, SignalItem &item)
{

}

void PlotDraw::drawSignalDots(QPainter &painter, SignalItem &item)
{

}
