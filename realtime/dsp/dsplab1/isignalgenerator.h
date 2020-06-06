#ifndef ISIGNALGENERATOR_H
#define ISIGNALGENERATOR_H

#include "discretesignal.h"

class ISignalGenerator
{

public:
    virtual double getSample(double time) = 0;

    void fill(DiscreteSignal& signal, double t0, double dt)
    {
        for (size_t i = 0; i < signal.Size(); ++i)
            signal.SetSample(i, getSample(t0 + dt*i));
    };
};

#endif // ISIGNALGENERATOR_H
