#include "stoplight.hpp"

#include <limits>

Stoplight::Stoplight(bool initialRed, std::function<float()> changingLambda, std::function<unsigned int(float)> departureLambda)
{
    red = initialRed;
    changingFunction = changingLambda;
    departureFunction = departureLambda;
    if(red)
        timeToRed = std::numeric_limits<float>::infinity();
    else
        timeToRed = changingFunction();
}

float Stoplight::changeLight(float time)
{
    red = !red;
    if(red)
        timeToRed = std::numeric_limits<float>::infinity();
    else
        timeToRed = time + changingFunction();
    return timeToRed;
}
