#pragma once
class baseAI
{
public:
	baseAI();
	~baseAI();
	virtual void AI(int board[8][8], int turn, int *x, int *y) = 0;
	virtual CString getName() = 0;
	virtual void setOrder(int x, int y) = 0;
	CString name;
private:
//	int board[10][10];
public:
};

