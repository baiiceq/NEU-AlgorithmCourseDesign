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
        int g;  // ʵ�ʴ���
        int f;  // �ܹ��ƴ���

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
        int value;      // ��Ҽ�ֵ (����start��end����ֵΪ0)
        int coin_id;    // ����ǽ�ң����ڽ���б��е�ID (0 to N-1)������λ���롣������ǽ�ң�Ϊ-1��
    };

    static int heuristic(const Vector2& a, const Vector2& b) 
    {
        return abs(a.x - b.x) + abs(a.y - b.y); // �����پ���
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

	// �������㵽�յ������·��
	// ����һ������·���������
	static PathResult find_direct_path(const Vector2& start, const Vector2& end, const std::vector<std::vector<int> > resource, const std::vector<std::vector<TileType> > grid);

    static PathResult find_best_path(
        const Vector2& start,
        const Vector2& end,
        const std::vector<Vector2>& all_coins,
        const std::vector<std::vector<TileType>>& grid,
		const std::vector<std::vector<int>>& resource_grid
    );
};

