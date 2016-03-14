#ifndef INTERSECTIONNETWORK_HPP
#define INTERSECTIONNETWORK_HPP

#include "lane.hpp"
#include "sourcelane.hpp"
#include "stoplight.hpp"
#include "random.hpp"
#include <array>

/*
 * This class is reserved for the intersection network, which coordinates all the lanes and stoplights
 * in order to simulate a 2x2 square network interconnected 4-way intersections.
 * (1) The simulation will run from the start time to the end time.
 * (2) Arrivals of cars from outside of the network will be determined by a collection of arrival functions (called
 * lambdas because they may use lambda expressions). Currently, these arrival functions do not change over time
 * (the underlying assumption being that a homogenous Poisson process is being modeled).
 * (3) Departures of cars will be determined by a departure function that is given to all stoplights.
 * (4) Changes in stoplight state will be determined by a changing function that is given to all stoplights
 * as well as a neighbor system that keeps stoplights in an intersection synchronized.
 */
class IntersectionNetwork
{
public:
    IntersectionNetwork(float startTime, float endTime,
                        std::array<std::function<float()>, 8> arrivalLambdas,
                        std::function<unsigned int(float)> departureLambda,
                        std::function<float()> changingLambda);
    void runSimulation(bool showOutput);

    float time;
    float endTime;
    std::array<SourceLane *, 8> sourceLanes;
    std::array<Lane *, 8> sinkLanes;
    std::array<Lane *, 8> intermediateLanes;
    std::array<Stoplight *, 32> stoplights;
    std::array<Lane *, 32> lightToInputLaneMap;
    std::array<std::pair<Lane *, Lane *>, 32> lightToOutputLaneMap;
    std::array<float, 40> eventTimes;

    unsigned int minEventIndex();
    unsigned int getNextLightIndex(unsigned int currentLightIndex);
    void printLaneName(unsigned int id);
    void printDelimitingLine(char delimiter, unsigned int numberOfDelimiters);
};

#endif // INTERSECTIONNETWORK_HPP
