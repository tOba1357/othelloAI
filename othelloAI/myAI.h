#pragma once
#include "baseAI.h"
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include "HashBoard.h"

namespace std{
	template <>
	class hash < std::pair<long long int, long long int>>{
	public:
		size_t operator()(const std::pair<long long int, long long int>& x)const{
			return hash<int>()(x.first) ^ hash<int>()(x.second);
		}
	};
}

class myAI : public baseAI
{
	
	typedef HashBoard::hash_key hash_key;
	typedef struct { int x; int y; }pos;
	typedef HashBoard::hash_value hash_value;
public:
	myAI();
	~myAI();
	CString getName(){ return name; }
	void setOrder(int x, int y){};
private:
	const CString name = _T("myAI");
	int raw_board[10][10];
	const int BLANK = 0;
	const int BLACK = 1;
	const int WHITE = 2;
	int N = 5;
	const int LAST_STAGE_NUM = 11;
	const int ON = 1;
	const int OFF = 0;
	std::vector<int> dire_x;
	std::vector<int> dire_y;
	//std::unordered_map<std::pair<long long int, long long int>, std::vector<_hash_value>>  hash_map;
	int turn;
	FILE *fpo, *fpw;
	int pass_cnt;
	bool hash_flag = true;
	HashBoard hash_map;
	clock_t time;

public:
	void AI(int board[8][8], int turn, int *x, int *y);
	void setBoard(int board[8][8]);
	bool isPut(int board[10][10], int turn, int x, int y);

	void startAI(int *x, int *y);
	int getPutNum(int board[10][10], int turn);
	void lastAI(int *x, int *y);
	void firstAI(int *x, int *y);
	int lastDFS(int board[10][10], int turn, int n, int a, int b, int *x, int *y);
	int getNum(int board[10][10],int num);
	bool put(int board[10][10], int x, int y, int turn);
	int getNextTurn(int board[10][10], int turn);
	int getPutCount(int board[10][10], int turn);
	int firstDFS(int board[10][10], int turn, int n, int a, int b, int *x, int *y);
	int getEvaluationValue(int board[10][10], int turn, int my_turn);
	void copyBoard(int board1[10][10], int board2[10][10]);
	void show(int board[10][10]);
	int getFixStoneNum(int board[10][10], int turn);
	void setPutMap(int board[10][10], bool put_map[10][10], int turn);
	std::vector<pos> getPutList(int board[10][10], int turn);
	void setFixStornLine(int line[10], int turn, bool fix_storn_line[10]);
	void showFixStoneMap(bool board[10][10]);
	void showLine(bool line[10]);
	void showLine(int line[10]);
	void setFixStornMap(int board[10][10], bool fix_storn_map[10][10]);
	void show(bool board[10][10]);
	hash_key board2hash(int board[10][10]);
	void hash2board(int board[10][10], hash_key hv);
	myAI::pos searchPutPos(int b_board[10][10], int a_board[10][10]);
	bool checkHash(int board[10][10], hash_key hv);
	int firstDFS_hash(hash_value hv, int n, int a, int b, int *x, int *y);
	void show(hash_key hk);
	bool checkHash(hash_key hk1, hash_key hk2);
	int getWinner(int board[10][10]);
};

