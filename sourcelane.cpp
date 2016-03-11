#include "sourcelane.hpp"

SourceLane::SourceLane(std::function<float()> arrivalLambda)
{
    arrivalFunction = arrivalLambda;
    setNextArrival(0.0);
}

float SourceLane::setNextArrival(float time)
{
    nextArrival = time + arrivalFunction();
    return nextArrival;
}
