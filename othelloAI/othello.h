#pragma once

#include <vector>
#include "sampleAI.h"
#include "othellogic.h"
#include "player.h"
#include "myAI.h"
#include "myAI2.h"
#include "baseAI.h"

class othello
{
public:
	othello();
	~othello();
	int read_board[8][8];
	const int white = 2;
	const int black = 1;
	const int blank = 0;
	void reverse(int x, int y, int turn, int dire_num);
	bool isPut(int x, int y, int turn);
	int getTurn();
	void setDefBoard();
	void setNextTurn();
	void put(int x, int y, int turn);
private:
	int board[10][10];
	int turn;
	std::vector<int> dire_x;
	std::vector<int> dire_y;
	CString black_name;
	CString white_name;
public:
	void copyReadBoard();
	int getWiner();
	int getWhiteNum();
	int getBlackNum();
	CString getBlackName();
	CString getWhiteName();
	bool nextStep();
	int getNum(int color);
	std::vector < baseAI* > AIs;
	int first_num;
	int second_num;
	void setPlayerOrder(int x, int y);
	void setAI(int first, int second);
	void setDef();
};

