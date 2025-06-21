#pragma once

#include <random>
#include <vector>

class Random 
{
public:
    // ��ʼ��������ӣ������� main �е���һ�Σ�
    static void init(unsigned int seed = std::random_device{}());

    static int randint(int low, int high);
    static double randfloat(double low, double high);
    static bool chance(double probability);

    template<typename T>
    static T choice(const std::vector<T>& vec);

private:
    static std::mt19937 rng;
};

