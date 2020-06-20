#ifndef DISCRETESIGNAL_H
#define DISCRETESIGNAL_H

#include <cstdio>
#include <QVector>
#include <complex>

using namespace std;


class DiscreteSignal
{
private:
    QVector<double> samples;

    static void r2ditfft(DiscreteSignal& signal, QVector<double> &transformRe, QVector<double> &transformIm,
                         size_t count = 0, size_t step = 1, size_t soffset = 0, size_t toffset = 0);

    static QVector<complex<double>> fft2(DiscreteSignal& signal, size_t count = 0, size_t step = 1, size_t offset = 0);

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
    static void Correlation(DiscreteSignal& signal1, DiscreteSignal& signal2, DiscreteSignal& ac);
    static void DFT(DiscreteSignal& signal, DiscreteSignal& dftAmp, DiscreteSignal& dftPhase);
    static void FFT(DiscreteSignal& signal, DiscreteSignal& dftAmp, DiscreteSignal& dftPhase);
};

#endif // DISCRETESIGNAL_H

