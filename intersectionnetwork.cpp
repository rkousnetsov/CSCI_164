#include "intersectionnetwork.hpp"
#include <cstdio>

/*
 * Constructor; initializes all member variables.
 */
IntersectionNetwork::IntersectionNetwork(float startTime, float endTime,
                                         std::array<std::function<float()>, 8> arrivalLambdas,
                                         std::function<unsigned int(float)> departureLambda,
                                         std::function<float()> changingLambda)
{
    /* Initialize start and end time */
    this->time = startTime;
    this->endTime = endTime;

    /* Initialize all lanes; source lanes also receive arrival functions */
    for(int x = 0; x < 8; ++x)
    {
        sourceLanes[x] = new SourceLane(x, arrivalLambdas[x]);
        sinkLanes[x] = new Lane(x + 8);
        intermediateLanes[x] = new Lane(x + 16);
    }

    /* Initialize all stoplights; all stoplights receive the same changing function and departure function
     * In this case, the stoplights controlling the SR sublanes for north-south movement are set to green;
     * all other stoplights are set to red (this is just an arbitrary choice). */
    for(int x = 0; x < 32; ++x)
        stoplights[x] = new Stoplight((x % 4) == 1 ? false : true, changingLambda, departureLambda);

    /* Set up all event times to monitor the time for the next arrival into a source lane or a stoplight
     * change, which are the two classes of events that can trigger a state change */
    for(int x = 0; x < 8; ++x)
        eventTimes[x] = sourceLanes[x]->nextArrival;
    for(int x = 8; x < 40; ++x)
        eventTimes[x] = stoplights[x - 8]->nextChange;

    /* Set up a map from stoplights to input lanes (which stoplight controls movement for which lane). */
    lightToInputLaneMap =
    {
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

    /* Set up a map from stoplights to output lanes (which lanes receive arrivals from cars controlled by
     * by a particular stoplight). */
    lightToOutputLaneMap =
    {
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
}

/*
 * Runs the intersection simulation and prints debug output if requested.
 */
void IntersectionNetwork::runSimulation(bool showOutput)
{
    /* Show debug output maring the start of simulation */
    if(showOutput)
    {
        printDelimitingLine('-', 79);
        printDelimitingLine('+', 79);
        printf("Time %f: Start of Simulation\n", time);
        printDelimitingLine('+', 79);
        printDelimitingLine('-', 79);
    }

    while (time < endTime)
    {
        /* Get time of next event */
        unsigned int eventIndex = minEventIndex();
        time = eventTimes[eventIndex];

        /* Stop simulation if end time is exceeded */
        if(time > endTime)
            break;

        /* Arrival into source lane 0-7 is marked by event indices 0-7 */
        if(eventIndex < 8)
        {
            SourceLane * lane = sourceLanes[eventIndex];
            lane->addCars(time, 1);
            eventTimes[eventIndex] = lane->setNextArrival(time);
            if(showOutput)
                printf("Time %f: Car arrived at Source lane %d\n", time, eventIndex);
        }
        else /* Stoplight change occured */
        {
            /* Find which stoplight has changed state and which neighboring stoplight's state is to be changed.
             * Change the states and update the event times. */
            unsigned int currentLightIndex = eventIndex - 8;
            unsigned int nextLightIndex = getNextLightIndex(currentLightIndex);
            Stoplight * currentLight = stoplights[currentLightIndex];
            Stoplight * nextLight = stoplights[nextLightIndex];
            eventTimes[currentLightIndex + 8] = currentLight->changeLight(time);
            eventTimes[nextLightIndex + 8] = nextLight->changeLight(time);

            /* Remove cars from the input sub-lane whose stoplight changed from green to red */
            Lane * inputLane = lightToInputLaneMap[currentLightIndex];
            unsigned int maxDepartedCars = currentLight->departureFunction(currentLight->changingFunction());
            unsigned int departedCars = inputLane->removeCars(time, maxDepartedCars, (currentLightIndex % 2) == 0);
            /* Even indexed stoplights control UL sub-lanes; odd indexed stoplights control SR sub-lanes */

            /* Add cars to the output sub-lanes for the stoplight whose state changed from green to red */
            Lane * outputLane0 = std::get<0>(lightToOutputLaneMap[currentLightIndex]);
            Lane * outputLane1 = std::get<1>(lightToOutputLaneMap[currentLightIndex]);
            unsigned int carsInput0 = Random::integerUniform(0, departedCars);
            unsigned int carsInput1 = departedCars - carsInput0;
            outputLane0->addCars(time, carsInput0);
            outputLane1->addCars(time, carsInput1);
            /* Departing cars are partioned between output lanes using a uniform integer distribution */

            /* Show debug ouput detailing car movement between lanes and stoplight state changes */
            if(showOutput)
            {
                printDelimitingLine('-', 79);
                printf("Time %f: Stoplight %d becomes red, Stoplight %d becomes green\n",
                       time, currentLightIndex, nextLightIndex);
                printf("Time %f: %d cars departed from ", time, departedCars);
                printLaneName(inputLane->id);
                printf("Time %f: %d cars entered ", time, carsInput0);
                printLaneName(outputLane0->id);
                printf("Time %f: %d cars entered ", time, carsInput1);
                printLaneName(outputLane1->id);
                printDelimitingLine('-', 79);
            }
        }
    }

    /* Show debug output marking the end of the simulation */
    if(showOutput)
    {
        printDelimitingLine('-', 79);
        printDelimitingLine('+', 79);
        printf("Time %f: End of simulation\n", endTime);
        printDelimitingLine('+', 79);
        printDelimitingLine('-', 79);
    }
}

/* Find the index of the next event (having the lowest time) */
unsigned int IntersectionNetwork::minEventIndex()
{
    unsigned int minIndex = 0;
    float minValue = eventTimes[0];
    for(unsigned int x = 1; x < eventTimes.size(); ++x)
    {
        if(eventTimes[x] < minValue)
        {
            minIndex = x;
            minValue = eventTimes[x];
        }
    }
    return minIndex;
}

/* Find the index of the neighbor stoplight for a particular parameter stoplight. */
unsigned int IntersectionNetwork::getNextLightIndex(unsigned int currentLightIndex)
{
    unsigned int nextIndex = currentLightIndex + 1;
    if(nextIndex % 8 != 0)
        return nextIndex;
    else
        return nextIndex - 8;
}

/* Print out the name of a lane according to its id. */
void IntersectionNetwork::printLaneName(unsigned int id)
{
    if(id < 8)
        printf("Source lane %d\n", id);
    else if (id < 16)
        printf("Sink lane %d\n", id - 8);
    else
        printf("Intermediate lane %d\n", id - 16);
}

/* Print out a delimiting line (to make debug output prettier) */
void IntersectionNetwork::printDelimitingLine(char delimiter, unsigned int numberOfDelimiters)
{
    for(unsigned int x = 0; x < numberOfDelimiters; ++x)
        printf("%c", delimiter);
    printf("\n");
}
