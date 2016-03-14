#include "lane.hpp"

#include "random.hpp"
#include <algorithm>

/*
 * Constructor; initializes all variables.
 */
Lane::Lane(unsigned int id)
{
    this->id = id;
    numberOfCarsUL = 0;
    numberOfCarsSR = 0;
    arrivalsUL.push_back(std::make_pair(0.0, 0));
    arrivalsSR.push_back(std::make_pair(0.0, 0));
    departuresUL.push_back(std::make_pair(0.0, 0));
    departuresSR.push_back(std::make_pair(0.0, 0));
}

/*
 * Adds cars to this lane.
 * Arriving cars are partioned between the two sub-lanes using a uniform integer distribution.
 * The time of arrival and number of cars arriving in each sub-lane is logged.
 * The number of cars in each sub-lane is updated accordingly.
 */
void Lane::addCars(float time, unsigned int cars)
{
    unsigned int carsUL = (unsigned int) Random::integerUniform(0, cars);
    unsigned int carsSR = cars - carsUL;

    if(carsUL != 0)
    {
        numberOfCarsUL += carsUL;
        arrivalsUL.push_back(std::make_pair(time, carsUL));
    }
    if(carsSR != 0)
    {
        numberOfCarsSR += carsSR;
        arrivalsSR.push_back(std::make_pair(time, carsSR));
    }
}

/*
 * Removes cars from a sub-lane.
 * The time of departure and number of cars departing in each sub-lane is logged.
 * The number of cars in each sub-lane is updated accordingly.
 */
unsigned int Lane::removeCars(float time, unsigned int cars, bool UL)
{
    unsigned int departedCars;
    if(UL)
    {
        departedCars = std::min(numberOfCarsUL, cars);
        if (departedCars != 0)
        {
            numberOfCarsUL -= departedCars;
            departuresUL.push_back(std::make_pair(time, departedCars));
        }
    }
    else
    {
        departedCars = std::min(numberOfCarsSR, cars);
        if (departedCars != 0)
        {
            numberOfCarsSR -= departedCars;
            departuresSR.push_back(std::make_pair(time, departedCars));
        }
    }
    return departedCars;
}
