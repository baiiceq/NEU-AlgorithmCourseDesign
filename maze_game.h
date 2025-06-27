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
#include "session.h"
#include "password.h"
#include "Greedy.h"
#include <string>
#include <graphics.h>
#include <sstream>
#include <iomanip>
#include <codecvt>

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
	std::string to_three_digit_string(int num) 
	{
		std::ostringstream oss;
		oss << std::setw(3) << std::setfill('0') << num;
		return oss.str();
	}

	std::wstring string_to_wstring(const std::string& str) 
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}


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
	OptionSelector ai_mode_selector;

	TextInput password_input;
	Button enter_button;

	GreedyPick gp;

	int ai_mode;
	std::vector<Vector2> ai_path;
	int ai_path_idx;
	
private:
	PasswordCracker* pc = nullptr;
	CrackingSession cs;
	int password;
	int try_password;
	std::vector<std::vector<int> > clue;
};

