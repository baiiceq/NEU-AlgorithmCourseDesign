#include <iostream>
#include <graphics.h>
#include "random.h"
#include "maze.h"

void draw_maze(MazeLayer ml)
{
	int TILE_SIZE = 40;
	auto grid = ml.getGrid();
	for (int i = 0; i < ml.getRows(); i++)
	{
		for (int j = 0; j < ml.getCols(); j++)
		{
			if (grid[i][j] == TileType::Wall)
			{
				// Draw wall tile
				Rect rect = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
				putimage_alpha(ResourcesManager::instance()->find_image("wall"), &rect);
			}
			else if (grid[i][j] == TileType::Empty || grid[i][j] == TileType::DOOR)
			{
				// Draw empty tile
				Rect rect = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
				putimage_alpha(ResourcesManager::instance()->find_image("floor"), &rect);
			}
		}
	}
}

int main()
{
	HWND hwnd = initgraph(40 * 22, 40 * 22, EW_SHOWCONSOLE);
	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id)
	{
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("无法加载：%s"), id);
		MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
		return -1;
	}

	Random::init();  
	MazeLayer maze(22, 22, false); 
	maze.generate();
	maze.print();
	draw_maze(maze);
	system("pause");
	return 0;
}