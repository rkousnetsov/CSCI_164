#ifndef LANE_HPP
#define LANE_HPP

#include <vector>
#include <utility>

/*
 * This class is reserved for a standard lane that contains:
 *  (1) A U-turn and left turn sub-lane (UL)
 *  (2) A straight pass and right turn sub-lane (SR)
 * All arrivals and departures are logged.
 * Also, the number of cars in both sub-lanes are tracked over time.
 * An ID is supplied to each lane for convenience in identification.
 */
class Lane
{
public:
    Lane(unsigned int id);
    void addCars(float time, unsigned int cars);
    unsigned int removeCars(float time, unsigned int cars, bool UL);

    unsigned int id;
    unsigned int numberOfCarsUL;
    unsigned int numberOfCarsSR;
    std::vector<std::pair<float, unsigned int>> arrivalsUL;
    std::vector<std::pair<float, unsigned int>> arrivalsSR;
    std::vector<std::pair<float, unsigned int>> departuresUL;
    std::vector<std::pair<float, unsigned int>> departuresSR;
};

#endif // LANE_HPP
