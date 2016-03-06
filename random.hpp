#ifndef RANDOM_HPP
#define RANDOM_HPP

/*
 * This class is reserved for all random variable generation.
 * All methods are static (enables the methods to be used without class instantiation).
 * This class requires C++11 (for the <random> library).
 */
class Random
{
public:
    static float standardUniform();
    static float standardNormal();
    static float exponential(float lambda);
    static float weibull(float lambda, float k);
    static float gamma(float alpha, float beta);
};

#endif // RANDOM_HPP
