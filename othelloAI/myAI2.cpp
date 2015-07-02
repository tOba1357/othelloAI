#include "stdafx.h"
#include "myAI2.h"


myAI2::myAI2()
{
	int tmp[] = { 1, -1, 0 };
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			dire_x.push_back(tmp[i]);
			dire_y.push_back(tmp[j]);
		}
	}
	dire_x.pop_back();
	dire_y.pop_back();
}


myAI2::~myAI2()
{
}

void myAI2::AI(int board[8][8], int turn, int *x, int *y){
	this->turn = turn;
	setBoard(board);
	trouble++;
	srand((unsigned int)time(NULL));
	std::vector<std::pair<int, int> > put_list = getPutList(raw_board, turn);
	int num = (N*trouble) / put_list.size();
	int sent_board[10][10];
	std::vector<int> cnt(put_list.size(), 0);
	for (int i = 0; i < put_list.size(); i++){
		for (int j = 0; j < num; j++){
			copyBoard(sent_board, raw_board);
			if (DFS(sent_board, turn)) cnt[i]++;
		}
	}
	int max_idx = 0;
	for (int i = 1; i < cnt.size(); i++){
		if (cnt[i] > cnt[max_idx]) max_idx = i;
	}
	for (int i = 0; i < cnt.size(); i++){
		std::cout << put_list[i].first << " " << put_list[i].second << " " << cnt[i] << std::endl;
	}
	std::cout << std::endl;
	*x = put_list[max_idx].second - 1; *y = put_list[max_idx].first - 1;
}


void myAI2::setBoard(int board[8][8]){
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			if (i == 0 || i == 9 || j == 0 || j == 9) raw_board[i][j] = -1;
			else raw_board[i][j] = board[i - 1][j - 1];
		}
	}
}


void myAI2::setPutMap(int board[10][10], bool put_map[10][10])
{
	bool storn_map[10][10] = { false };

	for (int i = 1; i < 9; i++){
		for (int j = 1; j < 9; j++){
			if (board[i][j] != BLANK){
				storn_map[i][j] = true;
				put_map[i + 1][j] = true;
				put_map[i + 1][j + 1] = true;
				put_map[i + 1][j - 1] = true;
				put_map[i - 1][j] = true;
				put_map[i - 1][j + 1] = true;
				put_map[i - 1][j - 1] = true;
				put_map[i][j + 1] = true;
				put_map[i][j - 1] = true;

			}
		}
	}
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			put_map[i][j] &= !storn_map[i][j];
		}
	}

}


std::vector<std::pair<int, int> > myAI2::getPutList(int board[10][10], int turn)
{
	std::vector<std::pair<int, int> > put_list;
	bool put_map[10][10] = { false };
	setPutMap(board, put_map);
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (put_map[i][j]){
				if (isPut(board, turn, i, j)){
					put_list.push_back(std::pair<int, int>(i, j));
				}
			}
		}
	}
	return put_list;
}

bool myAI2::isPut(int board[10][10], int turn, int x, int y)
{
	if (board[x][y] != BLANK) return false;
	int enemy_turn = turn ^ 3;
	for (int i = 0; i<dire_x.size(); i++){
		int cnt = 0;
		for (int j = 1; j <= 8; j++){
			int coler = board[x + dire_x[i] * j][y + dire_y[i] * j];
			if (coler == turn){
				if (cnt >= 1) return true;
				else break;
			}
			else if (coler == enemy_turn){
				cnt++;
			}
			else{
				break;
			}
		}
	}

	return false;
}


bool myAI2::DFS(int board[10][10], int turn)
{
	if (turn == BLANK){
		return getWinner(board) == this->turn;//
	}
	std::vector<std::pair<int, int> > put_list = getPutList(board, turn);
	std::pair<int, int> next_order = put_list[rand() % put_list.size()];
	put(board, next_order.first, next_order.second, turn);
	return DFS(board, getNextTurn(board, turn));
}

int myAI2::getNextTurn(int board[10][10], int turn)
{
	if (getPutList(board, turn ^ 3).size() != 0) return turn ^ 3;
	if (getPutList(board, turn).size() != 0) return turn;
	return BLANK;
}

bool myAI2::put(int board[10][10], int x, int y, int turn)
{
	if (board[x][y] != BLANK) return false;
	bool flag = false;
	int enemy_turn = turn ^ 3;
	for (int i = 0; i<dire_x.size(); i++){
		int cnt = 0;
		for (int j = 1; j <= 8; j++){
			int coler = board[x + dire_x[i] * j][y + dire_y[i] * j];
			if (coler == turn){
				if (cnt >= 1){
					for (int k = 1; k <= cnt; k++){
						board[x + dire_x[i] * k][y + dire_y[i] * k] = turn;
					}
					flag = true;
				}
				break;
			}
			else if (coler == enemy_turn){
				cnt++;
			}
			else{
				break;
			}
		}
	}
	if (flag) board[x][y] = turn;
	return flag;
}

int myAI2::getWinner(int board[10][10])
{
	int black_num = getNum(board, BLACK);
	int white_num = getNum(board, WHITE);
	return black_num == white_num ? BLANK : black_num > white_num ? BLACK : WHITE;
}


int myAI2::getNum(int board[10][10], int turn)
{
	int cnt = 0;
	for (int i = 1; i <= 8; i++){
		for (int j = 0; j <= 8; j++){
			if (board[i][j] == turn) cnt++;
		}
	}
	return cnt;
}


void myAI2::copyBoard(int board1[10][10], int board2[10][10])
{
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			board1[i][j] = board2[i][j];
		}
	}
}
