#pragma once
#include "baseAI.h"
#include <vector>
#include <iostream>
class myAI2 :
	public baseAI
{
private:
	CString name = _T("myAI2");
	int raw_board[10][10];
	const int BLANK = 0, BLACK = 1, WHITE = 2;
	const int N = 100;
	std::vector<int> dire_x, dire_y;
	int turn;
	int trouble = 0;
public:
	myAI2();
	~myAI2();
	void setOrder(int x, int y){};
	CString getName(){ return name; };
	void AI(int board[8][8], int turn, int *x, int *y);
	void setBoard(int board[8][8]);
	void setPutMap(int board[10][10], bool put_map[10][10]);
	std::vector<std::pair<int, int> > getPutList(int board[10][10], int turn);
	bool isPut(int board[10][10], int turn, int x, int y);
	bool DFS(int board[10][10], int turn);
	int getNextTurn(int board[10][10], int turn);
	bool put(int board[10][10], int x, int y, int turn);
	
	int getWinner(int board[10][10]);
	int getNum(int board[10][10], int turn);
	void copyBoard(int board1[10][10], int board2[10][10]);
};

