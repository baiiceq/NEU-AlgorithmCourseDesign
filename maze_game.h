#pragma once
#include "maze.h"
#include "player.h"
#include "random.h"
#include "boss.h"
#include "optimal_path.h"
#include "util.h"
#include <string>
#include <graphics.h>

class MazeGame
{
private:
	enum GameState
	{
		generate,
		start,
		ai
	};

public:
    MazeGame(int layers, int rows, int cols);
    
	void on_update(int delta);

	void on_render();

	void on_input(const ExMessage& msg);

	

private:
    Maze maze;
    Player player;
	GameState state;

	std::vector<Vector2> path_with_no_resource; // ��������Դ��·��
	int path_with_no_resource_idx = 0;

	int layers;
	int rows;
	int cols;

	int now_layer = 0; // ��ǰ����

	bool is_show_resource = false;
};

