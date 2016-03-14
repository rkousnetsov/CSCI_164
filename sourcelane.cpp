#include "sourcelane.hpp"

/*
 * Constructor; intializes the member variables.
 */
SourceLane::SourceLane(unsigned int id, std::function<float()> arrivalLambda)
    :Lane(id)
{
    arrivalFunction = arrivalLambda;
    setNextArrival(0.0);
}

/*
 * Sets the time of next arrival according to the parameter time and the arrival function.
 */
float SourceLane::setNextArrival(float time)
{
    nextArrival = time + arrivalFunction();
    return nextArrival;
}
