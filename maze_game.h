#pragma once
#include "maze.h"
#include "text_input.h"
#include "option_selector.h"
#include "player.h"
#include "random.h"
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

	void reset();

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

	void save_ai_result_to_json(std::string filename);

	std::string generate_filename(const std::string& prefix = "maze_test/", const std::string& ext = "json")
	{
		std::time_t t = std::time(nullptr);
		std::tm tm;
		localtime_s(&tm, &t);

		char buf[32];
		std::strftime(buf, sizeof(buf), "%y%m%d_%H%M", &tm); 

		std::ostringstream oss;
		oss << prefix << "maze_" << buf << "." << ext;
		return oss.str();
	}


private:
    Maze maze;
    Player player;
	GameState state;


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
	Button skip_button;
	Button load_button;

	OptionSelector count_selector;
	Button generate_button;

	GreedyPick gp;

	int ai_mode;
	std::vector<Vector2> ai_path;
	int ai_path_idx;
	
private:
	Button basic_attack;
	Button skill1;
	Button skill2;
	Button skill3;

private:
	int password;
	int try_password;
	std::vector<std::vector<int> > clue;
	int method1_tries;
	int method2_tries;

	int ai_try_password_idx;
	std::vector<std::string> ai_try_passwords;

	std::wstring open_json_file_dialog() 
	{
		wchar_t filename[MAX_PATH] = L"";

		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetHWnd(); 
		ofn.lpstrFilter = L"JSON 文件 (*.json)\0*.json\0所有文件 (*.*)\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		ofn.lpstrTitle = L"选择一个迷宫 JSON 文件";

		if (GetOpenFileName(&ofn))
		{
			return std::wstring(filename);
		}
		else 
		{
			return L""; 
		}
	}
};

