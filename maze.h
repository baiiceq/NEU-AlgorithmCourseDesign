#pragma once

#include "util.h"
#include "random.h"
#include <vector>

enum class TileType 
{
    Wall,
    Path,
    Start,
    Exit,
    Gold,
    Trap,
    Locker,
    Boss,
    Empty = -1,
    DOOR = -2
};


class MazeLayer 
{
private:
    int rows, cols;
    std::vector<std::vector<TileType>> grid;
    bool isFogLayer;  // 是否为迷雾层

    void divide(int left, int top, int right, int bottom);

public:
    MazeLayer(int rows, int cols, bool isFog = false);

    TileType getTile(int x, int y) const;
    void setTile(int x, int y, TileType type);

    int getRows() const;
    int getCols() const;
    bool isWalkable(int x, int y) const;
    bool isFog() const;

    void generate();  // 分治生成本层
    void print() const;

    const std::vector<std::vector<TileType>>& getGrid() const { return grid; }

};


class Maze  
{
public:
    Maze(int l, int r, int c);

    void generate();                      // 多层分治生成
    void placeStairs();                   // 添加楼梯连接层
    void placeStartAndExit();             // 放置起点终点
    void placeElements(TileType type, int count);

    TileType getTile(Position pos) const;
    void setTile(Position pos, TileType type);

    bool isWalkable(Position pos) const;
    int getLayers() const;
    int getRows() const;
    int getCols() const;
    const Position& getStart() const;
    const Position& getExit() const;

	void print() const;                   // 打印整个迷宫
private:
    int layers, rows, cols;
    std::vector<MazeLayer> maze;
};

