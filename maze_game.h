#pragma once
#include "maze.h"
#include "player.h"
#include "random.h"
#include "boss.h"
#include "util.h"
#include <string>

class MazeGame
{
    MazeGame(int layers, int rows, int cols);
    void run();           // 整体流程
    void testMaze();      // 迷宫结构展示
    void testPath();      // 路径测试
    void testGreedy();    // 贪心策略
    void testPuzzle();    // 解谜测试
    void testBoss();      // BOSS战斗

    void movePlayer(const std::string& direction);
private:
    Maze maze;
    Player player;
    Boss boss;

	std::string currentPuzzle; // 当前谜题
	std::string currentSolution; // 当前谜题答案

	void generateMaze();          // 生成迷宫
	void placeStartAndExit();     // 放置起点和终点
	void placeBoss();             // 放置BOSS
	void placeTraps();            // 放置陷阱
	void placePuzzles();          // 放置谜题

	bool checkPuzzleSolution(const std::string& attempt);  // 检查谜题答案
	void handlePlayerMove(const std::string& direction);   // 处理玩家移动
	void handleBossBattle();                                 // 处理BOSS战斗
	void handleTrap();                                      // 处理陷阱
	void handlePuzzle();                                    // 处理谜题
	void printMaze() const;                                  // 打印迷宫状态
	void printPlayerStatus() const;                          // 打印玩家状态
	void printBossStatus() const;                            // 打印BOSS状态
	void printCurrentPuzzle() const;                         // 打印当前谜题

	bool isGameOver() const;                                 // 检查游戏是否结束
	void resetGame();                                        // 重置游戏状态

};

