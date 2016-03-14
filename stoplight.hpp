#ifndef STOPLIGHT_HPP
#define STOPLIGHT_HPP

#include <functional>

/*
 * This class is reserved for a stoplight, which controls movement in the intersection network.
 * Each stoplight can change states (from red to green and green to red).
 *  (1) The time for the green to red transition is given by a member function (changingFunction)
 *  (2) The time of the red to green transition is controlled by another stoplight (when one stoplight
 *      undergoes a green to red transition, it will also trigger a red to green transition in a neighboring
 *      stoplight; this keeps the stoplights synchronized).
 *  (3) Upon changing from green to red, the departureFunction can be used to return a number of cars that may pass
 *      in a sub-lane according to the time spent in the green state (determined by changingFunction).
 */
class Stoplight
{
public:
    Stoplight(bool initialRed, std::function<float()> changingLambda,
              std::function<unsigned int(float)> departureLambda);
    float changeLight(float time);

    bool red;
    float nextChange;
    std::function<float()> changingFunction;
    std::function<unsigned int(float)> departureFunction;
};

#endif // STOPLIGHT_HPP
