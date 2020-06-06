#include "discretesignal.h"
#include "math.h"

DiscreteSignal::DiscreteSignal(size_t size)
{
    samples = QVector<double>(size);
    for (int i = 0; i < samples.size(); ++i)
        samples[i] = 0;
}

double DiscreteSignal::Average()
{
    double accum = 0;
    double kahan = 0;
    for (int i = 0; i < samples.size(); ++i)
    {
        double y = samples[i] - kahan;
        double t = accum + y;
        kahan = (t - accum) - y;
        accum = t;
    }
    if (samples.size() > 0)
        return accum / (double)samples.size();
    return 0;
}

double DiscreteSignal::Variance()
{
    return Variance(Average());
}

double DiscreteSignal::Variance(double avg)
{
    double accum = 0;
    double kahan = 0;
    for (int i = 0; i < samples.size(); ++i)
    {
        double d = samples[i] - avg;
        double y = d*d - kahan;
        double t = accum + y;
        kahan = (t - accum) - y;
        accum = t;
    }
    if (samples.size() > 0)
        return accum / (double)samples.size();
    return 0;
}

double DiscreteSignal::Max()
{
    if (samples.size() > 0)
    {
        double max = samples[0];
        for (int i = 0; i < samples.size(); ++i)
        {
            if (samples[i] > max)
                max = samples[i];
        }
        return max;
    }
    return 0;
}

double DiscreteSignal::Min()
{
    if (samples.size() > 0)
    {
        double min = samples[0];
        for (int i = 0; i < samples.size(); ++i)
        {
            if (samples[i] < min)
                min = samples[i];
        }
        return min;
    }
    return 0;
}

size_t DiscreteSignal::Size()
{
    return samples.size();
}

void DiscreteSignal::SetSample(size_t index, double value)
{
    assert(index < samples.size());
    samples[index] = value;
}

double DiscreteSignal::GetSample(size_t index)
{
    assert(index < samples.size());
    return samples[index];
}

void DiscreteSignal::Autocorrelation(DiscreteSignal& signal, DiscreteSignal& ac)
{
    double avg = signal.Average();

    for (size_t i = 0; i < ac.Size(); ++i)
    {
        double c = 0;
        for (size_t j = 0; j < signal.Size(); ++j)
        {
            if (j >= i)
                c += (signal.GetSample(j) - avg)*(signal.GetSample(j-i) - avg);
        }
        ac.SetSample(i, c);
    }
}

void DiscreteSignal::DFT(DiscreteSignal& signal, DiscreteSignal& dftAmp, DiscreteSignal& dftPhase)
{
    size_t N = signal.Size();
    assert(dftAmp.Size() == N);
    assert(dftPhase.Size() == N);

    for (size_t i = 0; i < N; ++i)
    {
        double dftRe = 0;
        double dftIm = 0;
        for (size_t j = 0; j < N; ++j)
        {
            double x = signal.GetSample(j);
            double arg = i * j * M_2_PI / N;
            dftRe += x * cos(arg);
            dftIm -= x * sin(arg);
        }
        dftAmp.SetSample(i, hypot(dftRe, dftIm));
        dftPhase.SetSample(i, atan2(dftIm, dftRe));
    }
}

void DiscreteSignal::FFT(DiscreteSignal& signal, DiscreteSignal& dftAmp, DiscreteSignal& dftPhase)
{

}
