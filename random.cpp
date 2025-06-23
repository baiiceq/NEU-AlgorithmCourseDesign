#include "random.h"

std::mt19937 Random::rng; // 静态成员定义

void Random::init(unsigned int seed)
{
    rng.seed(seed);
}

int Random::randint(int low, int high) 
{
    std::uniform_int_distribution<int> dist(low, high);
    return dist(rng);
}

double Random::randfloat(double low, double high) 
{
    std::uniform_real_distribution<double> dist(low, high);
    return dist(rng);
}

bool Random::chance(double probability)
{
    std::bernoulli_distribution dist(probability);
    return dist(rng);
}

float Random::lerp(float a, float b, float t)
{
    return a * (1 - t) + b * t;
}

template int Random::choice<int>(const std::vector<int>&);
template std::pair<int, int> Random::choice<std::pair<int, int>>(const std::vector<std::pair<int, int>>&);
