#include "stdafx.h"
#include "player.h"


player::player()
{
}


player::~player()
{
}
void player::setOrder(int x, int y){
	order_x = x;
	order_y = y;
}

void player::AI(int board[8][8], int turn, int *x, int *y){
	order_x = order_y = -1;
	while (true){
		if (0 <= order_x && 0 <= order_y && order_x <= 7 && order_y <= 7){
			*x = order_x;
			*y = order_y;
			break;
		}
		Sleep(100);
	}
}