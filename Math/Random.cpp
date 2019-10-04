#include "Math/Random.h"
#include <random>

CT_SCOPE_BEGIN

static std::mt19937 &RandomGenerator()
{
    static std::random_device seed;
    static std::mt19937 generator(seed());
    return generator;
}

namespace Math
{

int32 RandInt(int32 bound)
{
    bound = bound < 0 ? 0 : bound;
    std::uniform_int_distribution<int32> dist(0, bound);
    return dist(RandomGenerator());
}

int32 RandInt(int32 min, int32 max)
{
    if (min < max)
    {
        std::uniform_int_distribution<int32> dist(min, max);
        return dist(RandomGenerator());
    }
    else
    {
        std::uniform_int_distribution<int32> dist(max, min);
        return dist(RandomGenerator());
    }
}

float Rand01()
{
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(RandomGenerator());
}

} // namespace Math

CT_SCOPE_END