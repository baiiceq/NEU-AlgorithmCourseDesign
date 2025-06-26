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
	std::vector<Vector2> Path;
	Vector2 pos;
public:
	bool in_bounds(Vector2 p, const MazeLayer& mazelayer);
	std::vector<Vector2> getPath(int layer) { return Path; }
	void set_pos(Vector2 newpos);
	GreedyPick(Vector2 start = { 0,0 }, int start_layer = 0);
	void PickGoldLayer(MazeLayer& mazelayer);
	void findNearestGold(std::set<Vector2>& is_Pick, MazeLayer& mazelayer);
	std::vector<Vector2> moveTonext(Vector2 end, const MazeLayer& mazelayer);
	std::vector<Vector2> avoidTrap(const std::vector<Vector2>& path, const MazeLayer& mazelayer);
	std::vector<Vector2> avoidTrap_lastTonext(Vector2 start, Vector2 end, const MazeLayer& mazelayer);
	void PickGold(std::vector<MazeLayer>& maze);//接口传入多层地图
};