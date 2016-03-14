#include "stoplight.hpp"

#include <limits>

/*
 * Constructor; intializes all member variables.
 */
Stoplight::Stoplight(bool initialRed, std::function<float()> changingLambda,
                     std::function<unsigned int(float)> departureLambda)
{
    red = initialRed;
    changingFunction = changingLambda;
    departureFunction = departureLambda;
    if(red)
        nextChange = std::numeric_limits<float>::infinity();
    else
        nextChange = changingFunction();
}

/*
 * Changes the state of this light.
 * For a green to red transition, the time of the next change is controlled by another stoplight,
 * so the nextChange variables is set to an arbitrarily large constant.
 * Otherwise a red to green transition has taken place, so update the nextChange variable based on
 * the changingFunction.
 */
float Stoplight::changeLight(float time)
{
    red = !red;
    if(red)
        nextChange = std::numeric_limits<float>::infinity();
    else
        nextChange = time + changingFunction();
    return nextChange;
}
