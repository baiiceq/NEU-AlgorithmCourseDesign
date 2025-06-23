#include "maze_game.h"

MazeGame::MazeGame(int layers, int rows, int cols) : maze(layers, rows, cols),layers(layers), rows(rows), cols(cols)
{
	// ≥ı ºªØ√‘π¨
	maze.generate(0);

	state = start;
}


void MazeGame::on_update(int delta)
{
	if (state == start)
	{
		player.on_update(delta);
	}
}

void MazeGame::on_render()
{
	if (state == start)
	{
		player.on_render();
		maze.on_render(now_layer);
	}
}

void MazeGame::on_input(const ExMessage& msg)
{
	if (state == start)
	{
		player.on_input(msg, maze.get_layer(now_layer));
	}
}
