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
        int g;  // ʵ�ʴ���
        int f;  // �ܹ��ƴ���
        std::vector<Vector2> path;

        bool operator<(const Node& other) const 
        {
            return f > other.f;
        }
    };

    static int heuristic(const Vector2& a, const Vector2& b) 
    {
        return abs(a.x - b.x) + abs(a.y - b.y); // �����پ���
    }

public:
	OptimalPath() = default;
	~OptimalPath() = default;

	// �������㵽�յ������·��
	// ����һ������·���������
	static std::vector<Vector2> calculate(const Vector2& start, const Vector2& end, const std::vector<std::vector<int> > resource, const std::vector<std::vector<TileType> > grid);
};

