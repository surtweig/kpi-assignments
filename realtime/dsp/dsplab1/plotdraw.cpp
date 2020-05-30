#include "plotdraw.h"

PlotDraw::PlotDraw(QWidget *parent) : QWidget(parent)
{

}

void PlotDraw::SetBuffer(quint32 samplesCount, quint32 signalsCount)
{

}

void PlotDraw::ClearBuffer()
{

}

void PlotDraw::AddFrame(float*)
{

}

void PlotDraw::AddSample(float sample, quint32 signalIndex)
{

}

void PlotDraw::SetView(quint32 firstIndex, quint32 lastIndex)
{

}

void PlotDraw::SetView(quint32 firstIndex, quint32 lastIndex,
             float low, float high)
{

}

void PlotDraw::SetBackground(QColor color)
{

}

void PlotDraw::SetSignalStyle(QColor color, float thickness)
{

}

void PlotDraw::paintEvent(QPaintEvent* event)
{

}
