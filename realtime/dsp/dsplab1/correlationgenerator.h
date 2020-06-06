#ifndef CORRELATIONGENERATOR_H
#define CORRELATIONGENERATOR_H

#include "isignalgenerator.h"

class CorrelationGenerator : public ISignalGenerator
{
private:
    ISignalGenerator* s1;
    ISignalGenerator* s2;
    double avg1;
    double avg2;

public:
    CorrelationGenerator(ISignalGenerator* s1, ISignalGenerator* s2, double avg1, double avg2);

    double getSample(double time) override;
};

#endif // CORRELATIONGENERATOR_H
