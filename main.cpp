#include "random.hpp"
#include "lane.hpp"
#include "sourcelane.hpp"
#include "stoplight.hpp"

#include <cstdio>
#include <cmath>
#include <array>

float changingLambda()
{
    return 10.0;
}

unsigned int departureLambda(float greenTime)
{
    return (unsigned int) floor(Random::weibull(greenTime, 3.0));
}

float arrivalLambda()
{
    return Random::exponential(0.2);
}

unsigned int minIndex(std::vector<float> a)
{
    unsigned int minIndex = 0;
    float minValue = a[0];
    for(int x = 1; x < a.size(); ++x)
    {
        if(a[x] < minValue)
        {
            minIndex = x;
            minValue = a[x];
        }
    }
    return minIndex;
}

unsigned int getNextLightIndex(unsigned int currentIndex)
{
    unsigned int nextIndex = currentIndex + 1;
    if(nextIndex % 8 != 0)
        return nextIndex;
    else
        return nextIndex - 8;
}

void printLaneName(unsigned int id)
{
    if(id < 8)
        printf("Source lane %d\n", id);
    else if (id < 16)
        printf("Sink lane %d\n", id - 8);
    else
        printf("Intermediate lane %d\n", id - 16);
}

int main()
{
    std::array<SourceLane *, 8> sourceLanes;
    std::array<Lane *, 8> sinkLanes;
    std::array<Lane *, 8> intermediateLanes;
    for(int x = 0; x < 8; ++x)
    {
        sourceLanes[x] = new SourceLane(arrivalLambda);
        sinkLanes[x] = new Lane();
        intermediateLanes[x] = new Lane();
        sourceLanes[x]->id = x;
        sinkLanes[x]->id = x + 8;
        intermediateLanes[x]->id = x + 16;
    }

    std::array<Stoplight *, 32> stoplights;
    for(int x = 0; x < 32; ++x)
        stoplights[x] = new Stoplight((x % 4) == 1 ? false : true, changingLambda, departureLambda);

    std::array<Lane *, 32> lightToInputLaneMap = {
        sourceLanes[0], sourceLanes[0],
        sourceLanes[1], sourceLanes[1],
        intermediateLanes[0], intermediateLanes[0],
        intermediateLanes[1], intermediateLanes[1],
        sourceLanes[2], sourceLanes[2],
        intermediateLanes[2], intermediateLanes[2],
        intermediateLanes[3], intermediateLanes[3],
        sourceLanes[3], sourceLanes[3],
        intermediateLanes[4], intermediateLanes[4],
        sourceLanes[4], sourceLanes[4],
        sourceLanes[5], sourceLanes[5],
        intermediateLanes[5], intermediateLanes[5],
        intermediateLanes[6], intermediateLanes[6],
        intermediateLanes[7], intermediateLanes[7],
        sourceLanes[6], sourceLanes[6],
        sourceLanes[7], sourceLanes[7],
    };

    std::array<std::pair<Lane *, Lane *>, 32> lightToOutputLaneMap = {
        std::make_pair(sinkLanes[0], intermediateLanes[1]),
        std::make_pair(intermediateLanes[4], sinkLanes[1]),
        std::make_pair(sinkLanes[1], sinkLanes[0]),
        std::make_pair(intermediateLanes[2], intermediateLanes[4]),
        std::make_pair(intermediateLanes[4], sinkLanes[1]),
        std::make_pair(sinkLanes[0], intermediateLanes[2]),
        std::make_pair(intermediateLanes[2], intermediateLanes[4]),
        std::make_pair(sinkLanes[1], sinkLanes[0]),

        std::make_pair(sinkLanes[2], sinkLanes[3]),
        std::make_pair(intermediateLanes[6], intermediateLanes[1]),
        std::make_pair(intermediateLanes[1], sinkLanes[2]),
        std::make_pair(sinkLanes[3], intermediateLanes[6]),
        std::make_pair(intermediateLanes[6], intermediateLanes[1]),
        std::make_pair(sinkLanes[2], sinkLanes[4]),
        std::make_pair(sinkLanes[3], intermediateLanes[6]),
        std::make_pair(intermediateLanes[1], sinkLanes[2]),

        std::make_pair(intermediateLanes[0], intermediateLanes[7]),
        std::make_pair(sinkLanes[4], sinkLanes[5]),
        std::make_pair(sinkLanes[5], intermediateLanes[0]),
        std::make_pair(intermediateLanes[7], sinkLanes[4]),
        std::make_pair(sinkLanes[4], sinkLanes[5]),
        std::make_pair(intermediateLanes[0], intermediateLanes[7]),
        std::make_pair(intermediateLanes[7], sinkLanes[4]),
        std::make_pair(sinkLanes[5], intermediateLanes[0]),

        std::make_pair(intermediateLanes[3], sinkLanes[6]),
        std::make_pair(sinkLanes[7], intermediateLanes[5]),
        std::make_pair(intermediateLanes[5], intermediateLanes[3]),
        std::make_pair(sinkLanes[6], sinkLanes[7]),
        std::make_pair(sinkLanes[7], intermediateLanes[5]),
        std::make_pair(intermediateLanes[3], sinkLanes[7]),
        std::make_pair(sinkLanes[6], sinkLanes[7]),
        std::make_pair(intermediateLanes[5], intermediateLanes[3]),
    };

    std::vector<float> eventTimes;
    for(int x = 0; x < sourceLanes.size(); ++x)
        eventTimes.push_back(sourceLanes[x]->nextArrival);
    for(int x = 0; x < stoplights.size(); ++x)
        eventTimes.push_back(stoplights[x]->timeToRed);

    float time = 0;
    float endTime = 45;
    while (time < endTime)
    {
        unsigned int eventIndex = minIndex(eventTimes);
        time = eventTimes[eventIndex];
        if(time > endTime)
        {
            printf("Time %f: End time of %f exceeded -> simulation terminated\n", time, endTime);
            break;
        }

        if(eventIndex < 8)
        {
            SourceLane * lane = sourceLanes[eventIndex];
            lane->addCars(time, 1);
            eventTimes[eventIndex] = lane->setNextArrival(time);
            printf("Time %f: car arrived at source lane %d\n", time, eventIndex);
        }
        else
        {
            unsigned int currentLightIndex = eventIndex - 8;
            unsigned int nextLightIndex = getNextLightIndex(currentLightIndex);
            Stoplight * currentLight = stoplights[currentLightIndex];
            Stoplight * nextLight = stoplights[nextLightIndex];
            eventTimes[currentLightIndex + 8] = currentLight->changeLight(time);
            eventTimes[nextLightIndex + 8] = nextLight->changeLight(time);
            printf("Time %f: stoplight %d becomes red, stoplight %d becomes green\n", time, currentLightIndex, nextLightIndex);

            Lane * inputLane = lightToInputLaneMap[currentLightIndex];
            Lane * outputLane0 = std::get<0>(lightToOutputLaneMap[currentLightIndex]);
            Lane * outputLane1 = std::get<1>(lightToOutputLaneMap[currentLightIndex]);
            unsigned int departedCars = currentLight->departureFunction(currentLight->changingFunction());
            inputLane->removeCars(time, departedCars, (currentLightIndex % 2) == 0);
            printf("Time %f: %d cars departed from ", time, departedCars);
            printLaneName(inputLane->id);

            unsigned int carsInput0 = Random::integerUniform(0, departedCars);
            unsigned int carsInput1 = departedCars - carsInput0;
            outputLane0->addCars(time, carsInput0);
            outputLane1->addCars(time, carsInput1);
            printf("Time %f: %d cars entered ", time, carsInput0);
            printLaneName(outputLane0->id);
            printf("Time %f: %d cars entered ", time, carsInput1);
            printLaneName(outputLane1->id);
        }
    }
    return 0;
}
