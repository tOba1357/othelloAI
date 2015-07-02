#include "stdafx.h"
#include "sampleAI.h"

sampleAI::sampleAI()
{
}


sampleAI::~sampleAI()
{
}

/*
左上から置けるところがあるか探していき、最初に見つかった置けるところに置くプログラム
*/
void sampleAI::AI(int board[8][8], int turn, int* x, int* y)
{
	int i, j;
	for (i = 0; i < 8; i++){
		for (j = 0; j < 8; j++){
			if (isPut(board, turn, i, j)){
				*x = i;
				*y = j;
				return;
			}
		}
	}
}


bool sampleAI::isPut(int board[8][8], int turn, int x, int y)
{
	//上、右上、右、右下、下、左下、左、左上、上の順番に返せるところがあるかを見る
	int dire_x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int dire_y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

	//その場所が空白でなければ置けない
	if (board[y][x] != blank) return false;

	//8方向返せる石があるか調べる
	for (int i = 0; i < 8; i++){
		int cnt = 0;
		int next_x = x;
		int next_y = y;

		/*
		決めた方向に置きたい位置から進めていく。
		進めた方向が
		敵の石->もう一個先に進める
		自分の石->進めた回数が2回以上ならば返せる石がある。
		空白、範囲外->返せる石がない
		*/
		while (true){
			//調べる位置をずらす
			next_x += dire_x[i];
			next_y += dire_y[i];
			//何回進めたか
			cnt++;

			//ボードの外
			if (next_x < 0 || next_x > 7 || next_y < 0 || next_y > 7) break;

			int coler = board[next_y][next_x];
			//空白
			if (coler == blank) break;
			//自分の色
			if (coler == turn){
				if (cnt >= 2) return true;
				else break;
			}
		}
	}
	return false;
}
