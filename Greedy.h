#pragma once
#include"Player.h"
#include"maze.h"
#include"vector2.h"
#include<set>
#include<queue>
#include<map>

class GreedyPick
{
private:
	std::vector<std::vector<Vector2>> Path;
	int layer;
	Vector2 pos;
	std::vector<MazeLayer> maze;
public:
	bool in_bounds(Vector2 p, MazeLayer mazelayer);
	GreedyPick(Vector2 start = { 0,0 }, int start_layer = 0);
	void PickGoldLayer(MazeLayer mazelayer);
	void findNearestGold(std::set<Vector2> is_Pick, MazeLayer mazelayer);
};