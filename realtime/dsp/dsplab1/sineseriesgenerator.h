#ifndef SINESERIESGENERATOR_H
#define SINESERIESGENERATOR_H

#include <QtMath>
#include <QRandomGenerator>
#include <QVector>
#include "isignalgenerator.h"

class SineSeriesGenerator : public ISignalGenerator
{
private:
    QVector<double> amp;
    QVector<double> phase;
    QVector<double> freq;

public:
    SineSeriesGenerator(uint seed, uint harmonicsNumber, double maxAmplitude);

    double getSample(double time) override;
};

#endif // SINESERIESGENERATOR_H
