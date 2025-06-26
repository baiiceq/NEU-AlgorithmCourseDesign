#pragma once

#include <random>
#include <vector>
#include <algorithm>

#include "util.h"

class Random 
{
public:
    // 初始化随机种子（建议在 main 中调用一次）
    static unsigned int init(unsigned int seed = std::random_device{}());

    static int randint(int low, int high);
    static double randfloat(double low, double high);
    static bool chance(double probability);

	static std::mt19937& get_rng() { return rng; }


    template<typename T>
    static T choice(const std::vector<T>& vec)
    {
        std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
        return vec[dist(rng)];
    }


    // 生成基础 2D 噪声网格
    static std::vector<std::vector<float>> generate_base_noise(int rows, int cols) 
    {
        std::vector<std::vector<float>> noise(rows, std::vector<float>(cols));
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                noise[i][j] = dist(rng);
        return noise;
    }

    static float value_noise_2D(float x, float y, const std::vector<std::vector<float>>& base) 
    {
        int rows = base.size();
        int cols = base[0].size();

        int x0 = std::floor(x);
        int y0 = std::floor(y);
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        float sx = x - x0;
        float sy = y - y0;

        // 防越界
        x0 = clamp(x0, 0, rows - 1);
        y0 = clamp(y0, 0, cols - 1);
        x1 = clamp(x1, 0, rows - 1);
        y1 = clamp(y1, 0, cols - 1);

        float n00 = base[x0][y0];
        float n10 = base[x1][y0];
        float n01 = base[x0][y1];
        float n11 = base[x1][y1];

        float ix0 = lerp(n00, n10, sx);
        float ix1 = lerp(n01, n11, sx);
        return lerp(ix0, ix1, sy);
    }

private:
    static std::mt19937 rng;

    static float lerp(float a, float b, float t);
};

