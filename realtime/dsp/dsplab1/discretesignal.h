#ifndef DISCRETESIGNAL_H
#define DISCRETESIGNAL_H

#include <cstdio>
#include <QVector>

class DiscreteSignal
{
private:
    QVector<double> samples;
public:
    DiscreteSignal(size_t size);
    double Average();
    double Variance();
    double Variance(double avg);
    double Max();
    double Min();
    size_t Size();
    void SetSample(size_t index, double value);
    double GetSample(size_t index);

    static void Autocorrelation(DiscreteSignal& signal, DiscreteSignal& ac);
};

#endif // DISCRETESIGNAL_H
