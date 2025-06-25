#pragma once
#include <vector>
#include "Vector2.h"
#include "maze.h"

struct PathResult
{
    std::vector<Vector2> path;
    int cost = INT_MAX;
    bool found = false;
};

class OptimalPath
{
private:
    struct Node 
    {
        Vector2 pos;
        int g;  // 实际代价
        int f;  // 总估计代价

        bool operator<(const Node& other) const 
        {
            return f < other.f;
        }
        
        bool operator>(const Node& other) const
        {
            return f > other.f;
        }
    };

    struct Point
    {
        int id;         //0=start, 1-N=coin, N+1=end
        Vector2 pos;
        int value;      // 金币价值 (对于start和end，价值为0)
        int coin_id;    // 如果是金币，其在金币列表中的ID (0 to N-1)，用于位掩码。如果不是金币，为-1。
    };

    static int heuristic(const Vector2& a, const Vector2& b) 
    {
        return abs(a.x - b.x) + abs(a.y - b.y); // 曼哈顿距离
    }

    static int count_ones(int x)
    {
        int count = 0;
        while (x) 
        {
            count += x & 1;
            x >>= 1;
        }
        return count;
    }

    static std::vector<int> generate_sorted_numbers_by_ones(int n)
    {
        std::vector<int> nums;

        for (int i = 1; i < (1 << n); ++i) 
        {
            nums.push_back(i);
        }

        std::sort(nums.begin(), nums.end(), [](int a, int b) 
            {
                int ones_a = count_ones(a);
                int ones_b = count_ones(b);
                if (ones_a == ones_b) return a < b;
                return ones_a < ones_b;
            });

        return nums;
    }

public:
	OptimalPath() = default;
	~OptimalPath() = default;

	// 计算从起点到终点的最优路径
	// 返回一个包含路径点的向量
	static PathResult find_direct_path(const Vector2& start, const Vector2& end, const std::vector<std::vector<int> > resource, const std::vector<std::vector<TileType> > grid);

    static PathResult find_best_path(
        const Vector2& start,
        const Vector2& end,
        const std::vector<Vector2>& all_coins,
        const std::vector<std::vector<TileType>>& grid,
		const std::vector<std::vector<int>>& resource_grid
    );
};

