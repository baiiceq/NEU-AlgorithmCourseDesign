#pragma once
#include "maze.h"
#include "player.h"
#include "random.h"
#include "boss.h"
#include "util.h"
#include <string>
#include <graphics.h>

class MazeGame
{
private:
	enum GameState
	{
		generate,
		start
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

	int layers;
	int rows;
	int cols;

	int now_layer = 0; // µ±Ç°²ãÊý
};

