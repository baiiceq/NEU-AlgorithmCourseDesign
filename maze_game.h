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
    void run();           // ��������
    void testMaze();      // �Թ��ṹչʾ
    void testPath();      // ·������
    void testGreedy();    // ̰�Ĳ���
    void testPuzzle();    // ���ղ���
    void testBoss();      // BOSSս��

    void movePlayer(const std::string& direction);
private:
    Maze maze;
    Player player;
    Boss boss;

	std::string currentPuzzle; // ��ǰ����
	std::string currentSolution; // ��ǰ�����

	void generateMaze();          // �����Թ�
	void placeStartAndExit();     // ���������յ�
	void placeBoss();             // ����BOSS
	void placeTraps();            // ��������
	void placePuzzles();          // ��������

	bool checkPuzzleSolution(const std::string& attempt);  // ��������
	void handlePlayerMove(const std::string& direction);   // ��������ƶ�
	void handleBossBattle();                                 // ����BOSSս��
	void handleTrap();                                      // ��������
	void handlePuzzle();                                    // ��������
	void printMaze() const;                                  // ��ӡ�Թ�״̬
	void printPlayerStatus() const;                          // ��ӡ���״̬
	void printBossStatus() const;                            // ��ӡBOSS״̬
	void printCurrentPuzzle() const;                         // ��ӡ��ǰ����

	bool isGameOver() const;                                 // �����Ϸ�Ƿ����
	void resetGame();                                        // ������Ϸ״̬

};

