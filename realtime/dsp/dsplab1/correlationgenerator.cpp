#include "correlationgenerator.h"

CorrelationGenerator::CorrelationGenerator(ISignalGenerator* s1, ISignalGenerator* s2, double avg1, double avg2)
    : s1(s1), s2(s2), avg1(avg1), avg2(avg2)
{

}

double CorrelationGenerator::getSample(double time)
{

}
