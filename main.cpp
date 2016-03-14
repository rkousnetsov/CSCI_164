#include "intersectionnetwork.hpp"

/*
 * Get the sum of the second elements in a vector of pairs
 */
unsigned int vectorSum(std::vector<std::pair<float, unsigned int>> a)
{
    unsigned int total = 0;
    for(unsigned int x = 0; x < a.size(); ++x)
        total += std::get<1>(a[x]);
    return total;
}

/*
 * Get the maximum line length for a sub-lane over the course of the simulation using
 * its arrival (a) and departure (d) vectors.
 */
unsigned int maxLineLength(std::vector<std::pair<float, unsigned int>> a,
                           std::vector<std::pair<float, unsigned int>> d)
{
    unsigned int maxLength = 0;
    unsigned int currentLength = 0;
    unsigned int x = 0;
    unsigned int y = 0;
    while (x < a.size() && y < d.size())
    {
        if (std::get<0>(a[x]) < std::get<0>(d[y]))
        {
            currentLength += std::get<1>(a[x]);
            maxLength = std::max(maxLength, currentLength);
            ++x;
        }
        else
        {
            currentLength -= std::get<1>(d[y]);
            ++y;
        }
    }
    while (x < a.size())
    {
        currentLength += std::get<1>(a[x]);
        maxLength = std::max(maxLength, currentLength);
        ++x;
    }
    return maxLength;
}

/*
 * Get statistics for the UL and SR sub-lanes of a lane.
 * These statistics include:
 * (1) Total arrivals
 * (2) Total departures
 * (3) Efficiency (ratio of departures / arrivals)
 * (4) Maximum line length
 * Return an array of these statistics.
 * Elements 0-3 give these statistics (in that order) for the UL sub-lane.
 * Elenents 4-7 give thsee statistics (in that order) for the SR sub-lane.
 */
std::array<float, 8> getLaneStatistics(Lane * l)
{
    std::array<float, 8> retVal;
    retVal[0] = (float) vectorSum(l->arrivalsUL);
    retVal[1] = (float) vectorSum(l->departuresUL);
    retVal[2] = 100 * retVal[1] / retVal[0];
    retVal[3] = (float) maxLineLength(l->arrivalsUL, l->departuresUL);

    retVal[4] = (float) vectorSum(l->arrivalsSR);
    retVal[5] = (float) vectorSum(l->departuresSR);
    retVal[6] = 100 * retVal[5] / retVal[4];
    retVal[7] = (float) maxLineLength(l->arrivalsSR, l->departuresSR);
    return retVal;
}

/*
 * Average the lane statistics for each source and intermediate lane (sink lanes
 * only receive arrivals, so they are excluded) over multiple iterations of the
 * intersection network simulation.
 */
void getAverageLaneStatistics(IntersectionNetwork net, unsigned int iterations)
{
    std::array<std::array<float, 8>, 16> results;
    for(unsigned int x = 0; x < 16; ++x)
        for(unsigned int y = 0; y < 8; ++y)
            results[x][y] = 0.0;

    for(unsigned int i = 0; i < iterations; ++i)
    {
        net.runSimulation(false);
        for(unsigned int x = 0; x < 8; ++x)
        {
            std::array<float, 8> srcLanes = getLaneStatistics(net.sourceLanes[x]);
            std::array<float, 8> intLanes = getLaneStatistics(net.intermediateLanes[x]);
            for(unsigned int y = 0; y < 8; ++y)
            {
                results[x][y] += srcLanes[y] / iterations;
                results[x + 8][y] += intLanes[y] / iterations;
            }
        }
    }

    printf("%-10s %-13s %-13s %-13s %-13s %-13s %-13s %-13s %-13s\n",
           "Lane", "TNA(UL)", "TND(UL)", "Eff(UL)(%)", "MLL(UL)","TNA(SR)", "TND(SR)", "Eff(SR)(%)", "MLL(SR)");
    for(unsigned int x = 0; x < 8; ++x)
    {
        printf("SrcL%-6d %-13f %-13f %-13f %-13f %-13f %-13f %-13f %-13f\n", x,
               results[x][0], results[x][1], results[x][2], results[x][3], results[x][4], results[x][5], results[x][6], results[x][7]);
    }
    for(unsigned int x = 8; x < 16; ++x)
    {
        printf("IntL%-6d %-13f %-13f %-13f %-13f %-13f %-13f %-13f %-13f\n", x - 8,
               results[x][0], results[x][1], results[x][2], results[x][3], results[x][4], results[x][5], results[x][6], results[x][7]);
    }
}

/*
 * Returns the array of arrival functions.
 * Modify the lambda expressions for the nth element of the array to alter
 * the arrival function for the nth source lane.
 */
std::array<std::function<float()>, 8> arrivalFunctions()
{
    std::array<std::function<float()>, 8> retVal;
    retVal[0] = []() { return Random::exponential(1.0); };
    retVal[1] = []() { return Random::exponential(1.0); };
    retVal[2] = []() { return Random::exponential(1.0); };
    retVal[3] = []() { return Random::exponential(1.0); };
    retVal[4] = []() { return Random::exponential(1.0); };
    retVal[5] = []() { return Random::exponential(1.0); };
    retVal[6] = []() { return Random::exponential(1.0); };
    retVal[7] = []() { return Random::exponential(1.0); };
    return retVal;
}

/*
 * Departure function for cars based on the time a stoplight spends in the green state.
 * Adjust the singleDeparture lambda expression to change the departure time for a single car.
 */
unsigned int departureFunction(float greenTime)
{
    std::function<float()> singleDeparture = []() { return Random::weibull(1.0, 1.0); };
    float time = singleDeparture();
    unsigned int departures = 0;
    while (time < greenTime)
    {
        ++departures;
        time += singleDeparture();
    }
    return departures;
}

/*
 * Changing function for stoplight.
 * Adjust the return value to change how long each stoplight spends in the green state.
 */
float changingFunction()
{
    return 10.0;
}

int main()
{
    /*
     * Initialize intersection network and get average lane statistics from running it many times
     * Change the arrival functions, departure function, and changing function to modify the behavior
     * of the intersection network.
     */
    IntersectionNetwork net(0, 1000, arrivalFunctions(), departureFunction, changingFunction);
    getAverageLaneStatistics(net, 1000);

    return 0;
}
