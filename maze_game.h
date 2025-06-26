#pragma once
#include "maze.h"
#include "text_input.h"
#include "option_selector.h"
#include "player.h"
#include "random.h"
#include "boss.h"
#include "optimal_path.h"
#include "button.h"
#include "util.h"
#include <string>
#include <graphics.h>

class MazeGame
{
private:
	enum GameState
	{
		Init,
		Generate,
		Gamer,
		Ai,
		Locker,
		Boss
	};

public:
    MazeGame();
    
	void on_update(int delta);

	void on_render();

	void on_input(const ExMessage& msg);

	

private:
    Maze maze;
    Player player;
	GameState state;

	std::vector<Vector2> path_with_no_resource; // 不考虑资源的路径
	int path_with_no_resource_idx = 0;

	int layers;
	int rows;
	int cols;

	int now_layer = 0; // 当前层数

	bool is_show_resource = false;
	bool is_ai = false;
	bool is_maze_generate = false;

	unsigned int seed;

private:
	Button start_button;
	Button exit_button;
	TextInput seed_input;
	OptionSelector row_selector;
	OptionSelector col_selector;
	OptionSelector layer_selector;
	OptionSelector ai_selector;

	TextInput password_input;
	Button enter_button;
};

