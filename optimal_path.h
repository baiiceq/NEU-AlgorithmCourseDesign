#pragma once
#include <vector>
#include "Vector2.h"
#include "maze.h"

class OptimalPath
{
private:
    struct Node 
    {
        Vector2 pos;
        int g;  // 实际代价
        int f;  // 总估计代价
        std::vector<Vector2> path;

        bool operator<(const Node& other) const 
        {
            return f > other.f;
        }
    };

    static int heuristic(const Vector2& a, const Vector2& b) 
    {
        return abs(a.x - b.x) + abs(a.y - b.y); // 曼哈顿距离
    }

public:
	OptimalPath() = default;
	~OptimalPath() = default;

	// 计算从起点到终点的最优路径
	// 返回一个包含路径点的向量
	static std::vector<Vector2> calculate(const Vector2& start, const Vector2& end, const std::vector<std::vector<int> > resource, const std::vector<std::vector<TileType> > grid);
};

