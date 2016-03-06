#include "random.hpp"
#include <math.h>

/* Floating point macro for the mathematical constant pi */
#define PI_F 3.14159265358979f

/*
 * Initialize random number generator, which uses Mersenne Twister algorithm (mt19937),
 * with a random real value seed (random_device)
 */
std::random_device Random::rd;
std::mt19937 Random::gen(Random::rd());

/*
 * Returns a float from a standard uniform distribution ~ U(0, 1).
 */
float Random::standardUniform()
{
    std::uniform_real_distribution<float> unif(0, 1);
    return unif(Random::gen);
}

/*
 * Returns a float from a standard normal distribution.
 * Standard normal = Z ~ N(0, 1) = normal distribution with mean 0 and standard deviation 1.
 * The Box-Mueller algorithm is used here.
 * Note that the C++ <random> library also can directly generate normal random variables.
 */
float Random::standardNormal()
{
    return sqrt(-2*log(standardUniform()))*cos(2*PI_F*standardUniform());
}

/*
 * Returns a float from an exponential distribution with parameter lambda.
 * The inverse transform algorithm is used here.
 * Note that the C++ <random> library also can directly generate exponential random variables.
 */
float Random::exponential(float lambda)
{
    return -log(standardUniform()/lambda);
}

/*
 * Returns a float from a Weibull distribution with scale parameter lambda and shape parameter k.
 * The inverse transform algorithm is used here.
 * Note that the C++ <random> library also can directly generate Weibull random variables.
 */
float Random::weibull(float lambda, float k)
{
    return lambda * pow(-log(standardUniform()), 1/k);
}

/*
 * Returns a float from a Gamma distribution with shape parameter alpha and rate (inverse scale) parameter beta.
 * Marsaglia and Tsang’s method is used here.
 * Note that the C++ <random> library also can directly generate Gamma random variables.
 */
float Random::gamma(float alpha, float beta)
{
    float d, c, Z, V;
    if (alpha > 1)
    {
        d = alpha - 1.0/3.0;
        c = 1.0/sqrt(9*d);
        while(true)
        {
            do
            {
                Z = standardNormal();
            }
            while (Z <= (-1.0/c));
            V = pow(1 + c * Z, 3);
            if (log(standardUniform()) < (0.5*pow(Z,2) + d - d*V + d*log(V)))
                break;
        }
        return d*V/beta;
    }
    else
        return gamma(alpha+1, beta) * pow(standardUniform(), 1/alpha);
}