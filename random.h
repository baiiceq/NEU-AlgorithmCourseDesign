#pragma once

#include <random>
#include <vector>

class Random 
{
public:
    // 初始化随机种子（建议在 main 中调用一次）
    static void init(unsigned int seed = std::random_device{}());

    static int randint(int low, int high);
    static double randfloat(double low, double high);
    static bool chance(double probability);

    template<typename T>
    static T choice(const std::vector<T>& vec);

private:
    static std::mt19937 rng;
};

