#pragma once
#include "baseAI.h"
class player : public baseAI
{
public:
	player();
	~player();
	CString getName(){ return name; }
	CString name = _T("player");
	void setOrder(int x, int y);
	void AI(int board[8][8], int turn, int *x, int *y);
private:
	int order_x;
	int order_y;
};

