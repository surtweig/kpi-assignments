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

void DiscreteSignal::Correlation(DiscreteSignal& signal1, DiscreteSignal& signal2, DiscreteSignal& ac)
{
    assert(signal1.Size() == signal2.Size());

    double avg1 = signal1.Average();
    double avg2 = signal2.Average();

    for (size_t i = 0; i < ac.Size(); ++i)
    {
        double c = 0;
        for (size_t j = 0; j < signal1.Size(); ++j)
        {
            if (j >= i)
                c += (signal1.GetSample(j) - avg1)*(signal2.GetSample(j-i) - avg2);
        }
        ac.SetSample(i, c);
    }
}

void DiscreteSignal::Autocorrelation(DiscreteSignal& signal, DiscreteSignal& ac)
{
    /*
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
    */
    Correlation(signal, signal, ac);
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
            /*
            double samplePos = 20.0*M_2_PI*(double(j)/double(N) - 0.5);
            double sinc = 1;
            if (abs(samplePos) > 1e-6)
                sinc = sin(samplePos)/samplePos;
                */
            double x = signal.GetSample(j);
            double arg = i * j * M_2_PI / N;
            dftRe += x * cos(arg);
            dftIm -= x * sin(arg);
        }
        dftAmp.SetSample(i, hypot(dftRe, dftIm));
        dftPhase.SetSample(i, atan2(dftIm, dftRe));
    }
}



void fft_rec(std::complex<double> *x, int N) {
    // Check if it is splitted enough
    if (N <= 1) {
        return;
    }

    // Split even and odd
    std::complex<double> odd[N/2];
    std::complex<double> even[N/2];
    for (int i = 0; i < N / 2; i++) {
        even[i] = x[i*2];
        odd[i] = x[i*2+1];
    }

    // Split on tasks
    fft_rec(even, N/2);
    fft_rec(odd, N/2);


    // Calculate DFT
    for (int k = 0; k < N / 2; k++) {
        std::complex<double> t = exp(std::complex<double>(0, -2 * M_PI * k / N)) * odd[k];
        x[k] = even[k] + t;
        x[N / 2 + k] = even[k] - t;
    }
}

void fft(double *x_in,
    std::complex<double> *x_out,
    int N) {

    // Make copy of array and apply window
    for (int i = 0; i < N; i++) {
        x_out[i] = std::complex<double>(x_in[i], 0);
        x_out[i] *= 1; // Window
    }

    // Start recursion
    fft_rec(x_out, N);
}

QVector<complex<double>> DiscreteSignal::fft2(DiscreteSignal& signal, size_t count, size_t step, size_t offset)
{
    if (count == 0)
    {
        count = signal.Size();
        assert(count > 0 && (count & (count-1)) == 0);
    }

    QVector<complex<double>> transform = QVector<complex<double>>(count);
    if (count == 1)
    {
        /*
        double samplePos = 20.0*M_2_PI*(double(offset)/double(signal.Size()) - 0.5);
        double sinc = 1;
        if (abs(samplePos) > 1e-6)
            sinc = sin(samplePos)/samplePos;
            */
        transform[0] = complex<double>(signal.GetSample(offset), 0);
    }
    else
    {
        size_t halfcount = count / 2;

        QVector<complex<double>> t1 = fft2(signal, halfcount, step*2, offset);
        QVector<complex<double>> t2 = fft2(signal, halfcount, step*2, offset + step);

        for (size_t i = 0; i < halfcount; ++i)
        {
            complex<double> t = t1[i];
            //complex<double> twiddle = polar<double>(1, -(double)i*M_2_PI/(double)count) * t2[i];
            complex<double> twiddle = exp(std::complex<double>(0, -2 * M_PI * i / count)) * t2[i];
            transform[i] = t + twiddle;
            transform[i+halfcount] = t - twiddle;
        }
    }
    return transform;
}

void DiscreteSignal::r2ditfft(DiscreteSignal& signal, QVector<double> &transformRe, QVector<double> &transformIm,
                              size_t count, size_t step, size_t soffset, size_t toffset)
{
    if (count == 0)
    {
        count = signal.Size();
        assert(count > 0 && (count & (count-1)) == 0);
    }

    if (count == 1)
    {
        transformRe[toffset] = signal.GetSample(soffset);
        transformIm[toffset] = 0;
    }
    else
    {
        size_t halfcount = count / 2;
        r2ditfft(signal, transformRe, transformIm, halfcount, step*2, soffset, toffset);
        r2ditfft(signal, transformRe, transformIm, halfcount, step*2, soffset + step, toffset + halfcount);

        for (size_t i = 0; i < halfcount; ++i)
        {
            double tRe = transformRe[toffset+i];
            double tIm = transformIm[toffset+i];
            double t2Re = transformRe[toffset+i+halfcount];
            double t2Im = transformIm[toffset+i+halfcount];
            double eArg = -i*M_2_PI/count;
            double ecos = cos(eArg);
            double esin = sin(eArg);
            // twiddle = (ecos + i*esin) * (t2Re + i*t2Im) =
            // = ecos*t2Re - esin*t2Im + i*(esin*t2Re + ecos*t2Im)
            double twiddleRe = ecos*t2Re - esin*t2Im;
            double twiddleIm = esin*t2Re + ecos*t2Im;
            transformRe[toffset+i] = tRe + twiddleRe;
            transformIm[toffset+i] = tIm + twiddleIm;
            transformRe[toffset+i+halfcount] = tRe - twiddleRe;
            transformIm[toffset+i+halfcount] = tIm - twiddleIm;
        }
    }
}

void DiscreteSignal::FFT(DiscreteSignal& signal, DiscreteSignal& dftAmp, DiscreteSignal& dftPhase)
{
/*
    QVector<double> transformRe = QVector<double>(signal.Size());
    QVector<double> transformIm = QVector<double>(signal.Size());

    for (size_t i = 0; i < signal.Size(); ++i)
    {
        transformRe[i] = 0;
        transformIm[i] = 0;
    }

    r2ditfft(signal, transformRe, transformIm);
    for (size_t i = 0; i < signal.Size(); ++i)
    {
        dftAmp.SetSample(i, hypot(transformRe[i], transformIm[i]));
        dftPhase.SetSample(i, atan2(transformIm[i], transformRe[i]));
    }
*/


    QVector<complex<double>> transform = fft2(signal);

    //complex<double>* transform = (complex<double>*)malloc(signal.Size()*sizeof(complex<double>));
    //double* sig = signal.samples.data();
    //fft(sig, transform, signal.Size());
    for (size_t i = 0; i < signal.Size(); ++i)
    {
        dftAmp.SetSample(i, abs<double>(transform[i]));
        dftPhase.SetSample(i, arg<double>(transform[i]));
    }
    //free(transform);
}
