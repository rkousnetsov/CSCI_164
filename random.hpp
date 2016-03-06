#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

/*
 * This class is reserved for all random variable generation.
 * All methods are static (enables the methods to be used without class instantiation).
 * This class requires C++11.
 */
class Random
{
public:
    static float standardUniform();
    static float standardNormal();
    static float exponential(float lambda);
    static float weibull(float lambda, float k);
    static float gamma(float alpha, float beta);

private:
    static std::random_device rd; // Generator for random real value (as seed for RNG)
    static std::mt19937 gen; // Random number generator, uses Mersenne Twister algorithm
};

#endif // RANDOM_HPP
