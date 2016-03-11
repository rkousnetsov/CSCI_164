#ifndef LANE_HPP
#define LANE_HPP

#include <vector>
#include <utility>

class Lane
{
public:
    Lane();
    void addCars(float time, unsigned int cars);
    void removeCars(float time, unsigned int cars, bool UL);
    void setNextArrival(float time);

    unsigned int numberOfCarsUL;
    unsigned int numberOfCarsSR;
    unsigned int id;

    std::vector<std::pair<float, unsigned int>> arrivalsUL;
    std::vector<std::pair<float, unsigned int>> arrivalsSR;

    std::vector<std::pair<float, unsigned int>> departuresUL;
    std::vector<std::pair<float, unsigned int>> departuresSR;
};

#endif // LANE_HPP
