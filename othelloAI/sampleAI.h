#pragma once
#include "baseAI.h"
class sampleAI : public baseAI
{
public:
	sampleAI();
	~sampleAI();
	void setOrder(int x, int y){};
	virtual void AI(int board[8][8], int turn, int* x, int* y);
	CString getName(){ return name; }
	bool isPut(int board[8][8], int turn, int x, int y);
	const int white = 1;
	const int black = 2;
	const int blank = 0;
	CString name = _T("sampleAI");
};

