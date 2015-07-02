#include "stdafx.h"
#include "othello.h"



othello::othello()
{
	turn = black;
	setDefBoard();
	copyReadBoard();
	int tmp_x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int tmp_y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	for (int i = 0; i < 8; i++){
		dire_x.push_back(tmp_x[i]);
		dire_y.push_back(tmp_y[i]);
	}
	AIs.push_back(new player);
	AIs.push_back(new sampleAI);
	AIs.push_back(new othellogic);
	AIs.push_back(new myAI);
	AIs.push_back(new myAI2);
	
	first_num = 2;
	second_num = 2;
}


othello::~othello()
{
	for each (baseAI *tmp in AIs){
		delete tmp;
	}
}




void othello::reverse(int x, int y, int turn, int dire_num)
{
	int enemy_turn = turn ^ 3;
	while (true){
		x += dire_x[dire_num];
		y += dire_y[dire_num];
		int next_coler = board[y][x];
		if (next_coler == enemy_turn){
			board[y][x] = turn;
		}
		if (next_coler == turn){
			return;
		}
		if (next_coler == blank){
			//ERROR
			//AfxMessageBox(_T("ƒGƒ‰[Freverse"));
			return;
		}
	}
}


bool othello::isPut(int x, int y, int turn)
{
	int enemy_turn = turn ^ 3;
	if (x<1 || x > 8 || y < 1 || x>8) return false;
	if (board[y][x] != blank) return false;
	for (int i = 0; i < 8; i++){
		int cnt = 0;
		int next_x = x;
		int next_y = y;
		while (true){
			next_x += dire_x[i];
			next_y += dire_y[i];
			int next_coler = board[next_y][next_x];
			if (next_coler == turn){
				if (cnt >= 1) return true;
				else break;
			}
			if (next_coler == blank) break;
			cnt++;
		}
	}
	return false;
}


int othello::getTurn()
{
	return turn;
}


void othello::setDefBoard()
{
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			board[i][j] = blank;
		}
	}
	board[4][4] = board[5][5] = white;
	board[4][5] = board[5][4] = black;
}


void othello::setNextTurn()
{
	int next_turn = turn ^ 3;
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (isPut(i, j, next_turn)){
				turn = next_turn;
				return;
			}
		}
	}
	next_turn = turn;
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (isPut(i, j, next_turn)){
				return;
			}
		}
	}
	turn = blank;
}


void othello::put(int x, int y, int turn)
{
	int enemy_turn = turn ^ 3;
	for (int i = 0; i < 8; i++){
		int cnt = 1;
		int next_x = x;
		int next_y = y;
		while (true){
			next_x += dire_x[i];
			next_y += dire_y[i];
			int next_coler = board[next_y][next_x];
			if (next_coler == turn){
				if (cnt > 1){
					reverse(x, y, turn, i);
				}
				break;
			}
			if (next_coler == blank) break;
			cnt++;
		}
	}
	board[y][x] = turn;
	copyReadBoard();
}





void othello::copyReadBoard()
{
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			read_board[i][j] = board[i + 1][j + 1];
		}
	}
}


int othello::getWiner()
{
	int black_num = getBlackNum();
	int white_num = getWhiteNum();
	if (black_num == white_num) return blank;
	if (black_num > white_num) return black;
	else return white;
}



int othello::getWhiteNum()
{
	return getNum(white);
}


int othello::getBlackNum()
{
	return getNum(black);
}


bool othello::nextStep()
{
	int x, y;
	if (turn == blank){
		int winner = getWiner();
		std::string winnerStr;
		if (winner == blank) winnerStr = "ˆø‚«•ª‚¯";
		if (winner == black) winnerStr = "•‚ÌŸ‚¿";
		if (winner == white) winnerStr = "”’‚ÌŸ‚¿";
		std::string out;
		return true;

	}
	if (turn == black){
		while (true){
			copyReadBoard();
			AIs[first_num]->AI(read_board, turn, &x, &y);
			x++; y++;
			if (isPut(x, y, turn)) break;
		}
	}
	else{
		while (true){
			copyReadBoard();
			AIs[second_num]->AI(read_board, turn, &x, &y);
			x++; y++;
			if (isPut(x, y, turn)) break;
		}
	}
	put(x, y, turn);
	setNextTurn();
	return false;
}


int othello::getNum(int color)
{
	int cnt = 0;
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (board[i][j] == color) cnt++;
		}
	}
	return cnt;
}


CString othello::getBlackName(){
	return black_name;
}

CString othello::getWhiteName(){
	return white_name;
}


void othello :: setPlayerOrder(int x, int y){
	AIs[0]->setOrder(x, y);
}

void othello::setAI(int first, int second){
	first_num = first;
	second_num = second;
	black_name = AIs[first_num]->getName();
	white_name = AIs[second_num]->getName();
}


void othello::setDef()
{
	setDefBoard();
	turn = black;
}
