#ifndef SOURCELANE_HPP
#define SOURCELANE_HPP

#include "lane.hpp"
#include <functional>

/*
 * This class is reserved for a source lane, which is a special type of lane where cars can arrive from outside
 * of the intersection network according to some arrival function.
 * This class extends the standard lane in order to include an arrival function member and a time of next
 * arrival (calculated from the arrival function).
 */
class SourceLane: public Lane
{
public:
    SourceLane(unsigned int id, std::function<float()> arrivalLambda);
    float setNextArrival(float time);

    float nextArrival;
    std::function<float()> arrivalFunction;
};

#endif // SOURCELANE_HPP
