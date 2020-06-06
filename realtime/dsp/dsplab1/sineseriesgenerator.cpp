#include "sineseriesgenerator.h"

SineSeriesGenerator::SineSeriesGenerator(uint seed, uint harmonicsNumber, double maxAmplitude)
{
    amp = QVector<double>(harmonicsNumber);
    phase = QVector<double>(harmonicsNumber);
    freq = QVector<double>(harmonicsNumber);

    QRandomGenerator rand(seed);

    for (uint i = 0; i < harmonicsNumber; ++i)
    {
        //amp[i] = maxAmplitude/(i+1);// * rand.generateDouble();
        //phase[i] = 0;//M_2_PI * (double)i/harmonicsNumber;//M_2_PI * rand.generateDouble();
        //freq[i] = (i+1)*M_PI;
        amp[i] = maxAmplitude * rand.generateDouble();
        phase[i] = M_2_PI * rand.generateDouble();
        freq[i] = (i+1)*M_PI;
    }
}

double SineSeriesGenerator::getSample(double time)
{
    double series = 0;
    for (uint i = 0; i < amp.size(); ++i)
        series += amp[i] * sin( freq[i]*time + phase[i] );

    return series;
}
