#include "lane.hpp"

#include "random.hpp"
#include <algorithm>

Lane::Lane()
{
    arrivalsUL.push_back(std::make_pair(0.0, 0));
    arrivalsSR.push_back(std::make_pair(0.0, 0));
    departuresUL.push_back(std::make_pair(0.0, 0));
    departuresSR.push_back(std::make_pair(0.0, 0));
}

void Lane::addCars(float time, unsigned int cars)
{
    unsigned int carsUL = (unsigned int) Random::integerUniform(0, cars);
    unsigned int carsSR = cars - carsUL;

    numberOfCarsUL += carsUL;
    numberOfCarsSR += carsSR;

    if(carsUL != 0)
        arrivalsUL.push_back(std::make_pair(time, carsUL));
    if(carsSR != 0)
        arrivalsSR.push_back(std::make_pair(time, carsSR));
}

void Lane::removeCars(float time, unsigned int cars, bool UL)
{
    if(UL)
    {
        unsigned int departedCars = std::min(numberOfCarsUL, cars);
        numberOfCarsUL -= departedCars;
        departuresUL.push_back(std::make_pair(time, cars));
    }
    else
    {
        unsigned int departedCars = std::min(numberOfCarsSR, cars);
        numberOfCarsSR -= departedCars;
        departuresSR.push_back(std::make_pair(time, cars));
    }
}
