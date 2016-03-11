#ifndef SOURCELANE_HPP
#define SOURCELANE_HPP

#include "lane.hpp"
#include <functional>

class SourceLane: public Lane
{
public:
    SourceLane(std::function<float()> arrivalLambda);
    float setNextArrival(float time);

    float nextArrival;
    std::function<float()> arrivalFunction;
};

#endif // SOURCELANE_HPP
