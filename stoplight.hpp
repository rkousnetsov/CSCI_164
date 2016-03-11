#ifndef STOPLIGHT_HPP
#define STOPLIGHT_HPP

#include <functional>

class Stoplight
{
public:
    Stoplight(bool initialRed, std::function<float()> changingLambda,
              std::function<unsigned int(float)> departureLambda);
    float changeLight(float time);

    bool red;
    float timeToRed;
    std::function<float()> changingFunction;
    std::function<unsigned int(float)> departureFunction;
};

#endif // STOPLIGHT_HPP
