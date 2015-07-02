#pragma once
#include "baseAI.h"
class othellogic : public baseAI
{
public:
	void setOrder(int x, int y){};
	CString getName(){ return name; };
	othellogic();
	~othellogic();
	CString name = _T("othellogic");
	const int ON = 1;
	const int OFF = 0;
	const int BLACK = 2;
	const int WHITE = 1;
	const int BLANK = 0;
	const int WALL = -1;
#define LEN 256
	const int AI_START = 49; //奇数に設定する
	const int MID_LIMIT = 7;//奇数に設定

	/*余裕手の評価関数を作る　ウイングになるが余裕手など　手番を変更しても相手が打てないなど*/
	//ウイングは悪いという評価関数を作る　角を取ることは良いことという評価関数を作る　確定石など
	//中盤の評価関数で最後まで探索せずに、途中で対局終了になったときは、
	//石の数を数えて多いほうが＋１０００0などとする
	//隅が取られたら辺の評価はしない

	int calc_num = 0;//計算回数の表示に使う

	void AI(int a_board[8][8], int turn, int *sent_x, int *sent_y)
	{
		int tesuu = 0;
		int board[10][10];
		int x, y, run_flag = 1, count_num = 1;
		int player = 2, change_flag = OFF;
		int end = 0;
		int count_black = 0, count_white = 0, i, j;
		int computer, input = ON, computer_cont = 1, computer_count_num;
		int cont_player = 1, player_sub;
		int ai_player;
		int limit;
		int later = 0;
		//	int aimode =1,human_ai;
		/*初期盤面の配置*/
		init_board(board);
		//print_board(board);
		ai_player = turn;
		player = turn;
		tesuu = 0;
		for (i = 0; i < 8; i++) for (j = 0; j < 8; j++) board[i + 1][j + 1] = a_board[i][j];
		for (i = 0; i < 8; i++) for (j = 0; j < 8; j++) if (a_board[i][j] == turn) tesuu++;

		while (run_flag == 1){
			if (true){//(is_legal_move(board, player, x, y) == 1 && later == 1) || later == 2){/*プレイヤーが置ける場所に打ったならボードをプリント*/				later = 1;
				while (computer_cont){
					if (tesuu < AI_START){
						computer = decide_put(board, player, ai_player);
					}
					else{
						limit = 60 - tesuu;
						computer = move(board, player, ai_player, limit);
					}
					if (computer){
						*sent_y = computer / 10 - 1;
						*sent_x = computer % 10 - 1;
						return;
						printf("\n");
						set_and_turn_over(board, player, x, y);

						printf("計算回数は%d回です\n", calc_num);
						print_board(board);

						tesuu++;

						change_player(&player);
						if (exist_move(board, player)){
							change_flag = OFF;
							break;
						}
						else{
							computer_cont = 1;
							change_player(&player);
						}
					}
					else{
						printf("コンピュータはパスです");
						//						printf("computerの値：%d",computer);
						change_flag = ON;
						break;
					}
				}

				if (count_num >= (8 * 8 - 4)){
					run_flag = 0;
				}
				count_num++;
			}
			else{
				printf("そこには置けません\a\n");
				change_flag = OFF;
				later = 1;
			}

		}


		printf("対局が終了しました\n");
		for (i = 0; i<10 - 2; i++){
			for (j = 0; j<10 - 2; j++){
				if (board[i + 1][j + 1] == BLACK){
					count_black++;
				}
				if (board[i + 1][j + 1] == WHITE){
					count_white++;
				}
			}
		}
		printf("●の個数は%dです\n", count_black);
		printf("○の個数は%dです\n", count_white);

		return;
	}

	void init_board(int board[10][10])
	{
		int i, j;
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++) {
				board[i][j] = 0;
			}
		}
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++) {
				if (i == 0 || j == 0 || i == 10 - 1 || j == 10 - 1)
					board[i][j] = -1;
			}
		}


		board[10 / 2 - 1][10 / 2 - 1] = WHITE;
		board[10 / 2][10 / 2] = WHITE;
		board[10 / 2 - 1][10 / 2] = BLACK;
		board[10 / 2][10 / 2 - 1] = BLACK;
	}
	/*盤面をプリントする関数*/
	void print_board(int board[10][10])
	{
		int i, j;
		printf(" １２３４５６７８");
		for (i = 0; i < 10; i++) {
			if (i >= 1 && i <= 10 - 2)
				printf("%d", i);
			else
				printf(" ");
			for (j = 0; j < 10; j++) {
				if (board[i][j] == WHITE)
					printf("○");
				if (board[i][j] == BLACK)
					printf("●");
				if (board[i][j] == BLANK)
					printf("□");
				if (board[i][j] == WALL)
					printf("");
			}
			printf("\n");
		}
	}
	/*裏返す個数を数える（１方向のみ）*/
	int count_turn_over(int board[10][10], int player, int x, int y, int d, int e)
	{
		int i;

		for (i = 1; board[x + i*d][y + i*e] == 3 - player; i++){
		};
		if (board[x + i*d][y + i*e] == player) {
			return i - 1;
		}
		else {
			return 0;
		}
	}
	/*すべての方向で裏返す石があるかどうか調べ、１個でも返せる石があるなら置けると判断する*/

	int is_legal_move(int board[10][10], int player, int p, int q)
	{
		if (p < 1 || p > 8 || q < 1 || q > 8) return 0;
		if (board[p][q] != 0) return 0;
		if (count_turn_over(board, player, p, q, -1, 0)) return 1;  /* 上 */
		if (count_turn_over(board, player, p, q, 1, 0)) return 1;  /* 下 */
		if (count_turn_over(board, player, p, q, 0, -1)) return 1;  /* 左 */
		if (count_turn_over(board, player, p, q, 0, 1)) return 1;  /* 右 */
		if (count_turn_over(board, player, p, q, -1, -1)) return 1;  /* 左上 */
		if (count_turn_over(board, player, p, q, -1, 1)) return 1;  /* 右上 */
		if (count_turn_over(board, player, p, q, 1, -1)) return 1;  /* 左下 */
		if (count_turn_over(board, player, p, q, 1, 1)) return 1;  /* 右下 */
		return 0;
	}

	int exist_move(int board[10][10], int player)/*returnで返す値が自然数だからint型で宣言*/
	{
		int i, j;

		for (i = 1; i <= 8; i++) {
			for (j = 1; j <= 8; j++) {
				if (is_legal_move(board, player, i, j)) return 1;
			}
		}
		return 0;
	}

	void set_and_turn_over(int board[10][10], int player, int x, int y)
	{
		/*石を裏返す関数*/
		int count, d, e, i;

		for (d = -1; d <= 1; d++) {/* 上下方向 */
			for (e = -1; e <= 1; e++) {/* 左右方向 */
				if (d == 0 && e == 0) continue;/*0のみ除外して次のループへ*/
				count = count_turn_over(board, player, x, y, d, e);/*一方向に裏返す個数を調べる*/
				for (i = 1; i <= count; i++) {
					board[x + i*d][y + i*e] = player; /* 裏返す */
				}
			}
		}
		board[x][y] = player; /* 石を置く */
	}


	void change_player(int *player){
		if (*player == 2)
			*player = 1;
		else
			*player = 2;
	}

	int input_num(void) {
		int i;
		char str[LEN];

		printf("整数を入力してください: ");
		fgets(str, LEN, stdin);
		i = strtol(str, (char **)NULL, 10);

		return i;
	}
	int count_disc(int board[10][10], int ai_player){
		int black_num = 0, white_num = 0;
		int i, j;

		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (board[i][j] == BLACK){
					black_num++;
				}
				if (board[i][j] == WHITE){
					white_num++;
				}
			}
		}
		if (ai_player == BLACK){
			return (black_num - white_num);
		}
		else{
			return (white_num - black_num);
		}
	}

	int endgame(int board[10][10], int player){

		if (exist_move(board, player) == 0){
			/*パスの設定２回とも連続でパスだったら対局終了*/
			//		printf("パス！");

			change_player(&player);

			if (exist_move(board, player) == 0){
				return 1;
			}

		}
		return 0;
	}
	int count_kakutei(int board[10][10], int kakutei_turn){
		int kb[10][10];
		int i, j, u, l, k;
		int kakutei = 0;

		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				kb[i][j] = board[8 - j + 1][i];
			}
		}

		for (u = 1; board[1][u] == kakutei_turn; u++){
		}
		for (l = 1; board[l][1] == kakutei_turn; l++){
		}

		if (u >= l){
			for (i = 1; i <= l; i++){
				k = 1;
				while (1){
					if (k>u - i + 1){
						break;
					}
					else if (board[i][k] == kakutei_turn){
						kakutei++;
						k++;
					}
					else{
						break;
					}
				}
			}
		}
		else{
			for (j = 1; j <= u; j++){
				k = 1;
				while (1){
					if (k>l - j + 1){
						break;
					}
					else if (board[k][j] == kakutei_turn){
						kakutei++;
						k++;
					}
					else{
						break;
					}
				}
			}
		}

		return kakutei;
	}
	int eval_mid(int board[10][10], int player, int ai_player){
		int i, j, k, e, d;
		int eval_score = 437;
		int kakutei = 0;
		int edge[8][2];
		int human;
		int extinct_human = ON, extinct_ai = ON;
		int break_flag = OFF;

		if (ai_player == BLACK) human = WHITE;
		else human = BLACK;

		//全滅の処理

		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (board[i][j] == human){
					extinct_human = OFF;
					break_flag = ON;
					break;
				}
			}
			if (break_flag == ON) break;
		}

		break_flag = OFF;
		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (board[i][j] == ai_player){
					extinct_ai = OFF;
					break_flag = ON;
					break;
				}
			}
			if (break_flag == ON) break;
		}

		if (extinct_human == ON&&extinct_ai == OFF){
			eval_score = eval_score + 10000;
		}
		if (extinct_ai == ON&&extinct_human == OFF){
			eval_score = eval_score - 10000;
		}

		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (board[i][j] == ai_player){
					for (e = -1; e <= 1; e++){
						for (d = -1; d <= 1; d++){
							if (d == 0 && e == 0) continue;

							if (board[i + e][j + d] == 0){
								eval_score = eval_score - 5;
							}
						}
					}
				}
				else{
					for (e = -1; e <= 1; e++){
						for (d = -1; d <= 1; d++){
							if (d == 0 && e == 0) continue;
							if (board[i + e][j + d] == 0){
								eval_score = eval_score + 5;
							}
						}
					}
				}
			}
		}

		if (board[1][8] == ai_player || board[1][1] == ai_player || board[8][1] == ai_player || board[8][8] == ai_player){
			eval_score = eval_score + 200;
			eval_score = eval_score + 10 * count_kakutei(board, ai_player);
		}
		if (board[1][8] == human || board[1][1] == human || board[8][1] == human || board[8][8] == human){
			eval_score = eval_score - 200;
			eval_score = eval_score - 10 * count_kakutei(board, human);
		}

		if (board[1][1] == BLANK){
			if (board[2][2] == ai_player){
				eval_score -= 100;
			}
		}
		if (board[1][8] == BLANK){
			if (board[2][7] == ai_player){
				eval_score -= 100;
			}
		}
		if (board[8][1] == BLANK){
			if (board[7][2] == ai_player){
				eval_score -= 100;
			}
		}
		if (board[8][8] == BLANK){
			if (board[7][7] == ai_player){
				eval_score -= 100;
			}
		}

		if (board[1][1] == BLANK){
			if (board[2][2] == human){
				eval_score += 100;
			}
		}
		if (board[1][8] == BLANK){
			if (board[2][7] == human){
				eval_score += 100;
			}
		}
		if (board[8][1] == BLANK){
			if (board[7][2] == human){
				eval_score += 100;
			}
		}
		if (board[8][8] == BLANK){
			if (board[7][7] == human){
				eval_score += 100;
			}
		}

		for (i = 1; i <= 8; i++){//左辺のコピー
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[i][j];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);
		for (i = 1; i <= 8; i++){//上辺のコピー
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[j][i];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);
		for (i = 1; i <= 8; i++){//右辺のコピー
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[8 - i + 1][8 - j + 1];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);
		for (i = 1; i <= 8; i++){//下辺のコピー
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[8 - j + 1][i];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);

		return eval_score;
	}

	int edge_eval(int edge[8][2], int ai_player, int board[10][10]){
		//edge[8][2]は添字0から始まっている。
		int x = 0, i, y;
		int ev = 0;
		int j;
		int count_disc = 0;
		int human;

		human = 3 - ai_player;

		if (edge[0][0] == BLANK&&edge[7][0] == BLANK){
			//プレイヤーは人間　相手の形を判別
			//余裕手は?２５
			if (edge[2][0] == human && edge[3][0] == human && edge[4][0] == human && edge[5][0] == human){
				//石が４つ辺に並んでいる
				if (edge[1][0] == human&&edge[6][0] == BLANK){
					if (edge[5][1] == ai_player){
						//ウイングにならずに余裕手になった
						ev = ev - 30;
					}
					else if (edge[5][1] == human){
						ev = ev + 10;
						//ウイングになった
					}
				}
				else if (edge[1][0] == BLANK&&edge[6][0] == human){
					if (edge[2][1] == ai_player){
						//ウイングにならずに余裕手になった
						ev = ev - 30;
					}
					else if (edge[2][1] == human){
						ev = ev + 10;
						//ウイングになった
					}
				}
				else if (edge[1][0] == human&&edge[6][0] == human){
					//山になった
					ev = ev - 20;
				}
				else if (edge[1][0] == BLANK && edge[6][0] == BLANK){
					//ブロックか余裕手か
					if (edge[2][1] == human && edge[5][1] == human){
						ev = ev + 8;
					}
					else{
						//相手の余裕手になる
						if (edge[2][1] == ai_player){
							ev = ev - 20;
						}
						if (edge[5][1] == ai_player){
							ev = ev - 20;
						}
					}
				}
			}
			else if (edge[0][0] == BLANK&&edge[7][0] == BLANK){
				//humanの色を探して、石を見つけたら
				//何個連続かを調べて３つ以上なら連続の石の端の一つ上の色を調べて、
				//相手の色なら点数を付ける
				x = 7;
				while (x >= 1){
					//edge[x][0]がhumanになったらその座標を抽出する
					if (edge[x][0] == human){
						break;
					}
					//問題点　一個も石がなかったら7まで続けてしまう
					x--;
				}
				y = x;
				while (1){
					if (edge[x][0] != human){
						break;
					}
					count_disc++;
					x--;
				}
				if (count_disc <= 2){//連続の石の個数
					if (edge[1][0] == human){
						ev = ev + 20;
					}
					if (edge[6][0] == human){
						ev = ev + 20;
					}
				}
				/*
				else{//3個以上
				if (edge[y][1] == ai_player){
				ev = ev - 20;
				}
				if (edge[x + 1][1] == ai_player){
				ev = ev - 20;
				}
				}
				*/

			}
			//ai_player　始まり

			if (edge[2][0] == ai_player && edge[3][0] == ai_player && edge[4][0] == ai_player && edge[5][0] == ai_player){
				//石が４つ辺に並んでいる
				if (edge[1][0] == ai_player&&edge[6][0] == BLANK){
					if (edge[5][1] == human){
						//ウイングにならずに余裕手になった
						ev = ev + 30;
					}
					else if (edge[5][1] == ai_player){
						ev = ev - 10;//ウイングになった
					}
				}
				else if (edge[1][0] == BLANK&&edge[6][0] == ai_player){
					if (edge[2][1] == human){
						//ウイングにならずに余裕手になった
						ev = ev + 30;
					}
					else{
						ev = ev - 10;
					}
				}
				else if (edge[1][0] == ai_player&&edge[6][0] == ai_player){
					//山になった
					ev = ev + 20;
				}
				else if (edge[1][0] == BLANK && edge[6][0] == BLANK){
					//ブロックか余裕手か
					if (edge[2][1] == ai_player && edge[5][1] == ai_player){//ブロック
						ev = ev - 8;
					}
					else{
						//相手の余裕手になる
						if (edge[2][1] == human){
							ev = ev + 20;
						}
						if (edge[5][1] == human){
							ev = ev + 20;
						}
					}
				}
			}
			else{
				//humanの色を探して、石を見つけたら
				//何個連続かを調べて３つ以上なら連続の石の端の一つ上の色を調べて、
				//相手の色なら点数を付ける
				x = 7;
				while (x >= 1){
					//edge[x][0]がhumanになったらその座標を抽出する
					if (edge[x][0] == ai_player){
						break;
					}
					//問題点　一個も石がなかったら7まで続けてしまう
					x--;
				}
				y = x;
				while (1){
					if (edge[x][0] != ai_player){
						break;
					}
					count_disc++;
					x--;
				}
				if (count_disc<3){//連続の石の個数が3以下で単独c打ち
					if (edge[1][0] == ai_player){
						ev = ev - 20;
					}
					if (edge[6][0] == ai_player){
						ev = ev - 20;
					}
				}
				/*
				else{//3個以上
				if (edge[y][1] == human){
				ev = ev + 20;
				}
				if (edge[x + 1][1] == human){
				ev = ev + 20;
				}
				}
				*/

			}
			//ai_player終わり
		}
		return ev;
	}


	int saiki_put(int board[10][10], int player, int ai_player, int limit, int alpha, int beta){

		int i, j, k, l, score, score_max = -9000;
		int boardcopy[10][10];
		int exist;
		int t, b, a;
		int move[40][3];
		int first_node = ON;

		a = alpha;
		b = beta;

		calc_num++;//計算回数の表示
		change_player(&player);

		if (limit <= 0 || (endgame(board, player) == 1)){
			return eval_mid(board, player, ai_player);
		}

		if (exist_move(board, player) == 0){
			score_max = -saiki_put(board, player, ai_player, limit, -beta, -alpha);
		}

		for (k = 0; k<10; k++){
			for (l = 0; l<10; l++){
				boardcopy[k][l] = board[k][l];
			}
		}
		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (is_legal_move(board, player, i, j)){
					set_and_turn_over(board, player, i, j);

					t = -saiki_put(board, player, ai_player, limit - 1, -b, -a);

					if (t>a && t < beta && first_node == OFF && limit>2){
						t = -saiki_put(board, player, ai_player, limit - 1, -beta, -t);//←　-tを-alphaに変えてみる
					}
					first_node = OFF;
					for (k = 0; k<10; k++){
						for (l = 0; l<10; l++){
							board[k][l] = boardcopy[k][l];
						}
					}
					if (t>score_max){
						if (t >= beta) return t;
						score_max = t;
						if (t>a){
							a = t;
						}
					}
					b = a + 1;
				}
			}
		}


		return score_max;
	}

	int decide_put(int board[10][10], int player, int ai_player){
		int place = 0;
		int can_put[30];
		int i, j;
		int score_max = -9000, score;
		int boardcopy[10][10], p, q;
		int alpha = -10000, beta = 10000;

		for (p = 0; p<10; p++){
			for (q = 0; q<10; q++){
				boardcopy[p][q] = board[p][q];
			}
		}

		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (is_legal_move(board, player, i, j)){
					set_and_turn_over(board, player, i, j);
					score = -saiki_put(board, player, ai_player, MID_LIMIT, -beta, -alpha);
					/*着手箇所の点数付け*/

					for (p = 0; p<10; p++){
						for (q = 0; q<10; q++){
							board[p][q] = boardcopy[p][q];
						}
					}

					//				printf("moveが%d%dに打った時のscoreは%d\n",i,j,score);
					if (score>score_max){
						score_max = score;
						place = 10 * i + j;
						//					printf("%dが有力な手です",place);
					}
				}

			}
		}
		//printf("%d",calc_num);
		return place;
	}



	//2回書かなければいけないところは関数を呼び出して値を変更する。
	int saiki(int board[10][10], int player, int ai_player, int limit, int alpha, int beta){

		int i, j, k, l, score, score_max = -9000;
		int boardcopy[10][10];
		int exist;
		int t, b, a;
		int move[40][3];
		int first_node = ON;

		a = alpha;
		b = beta;

		calc_num++;//計算回数の表示
		change_player(&player);

		if (limit <= 0 || (endgame(board, player) == 1)){
			return count_disc(board, ai_player);
		}

		if (exist_move(board, player) == 0){
			score_max = -saiki(board, player, ai_player, limit, -beta, -alpha);
		}

		for (k = 0; k<10; k++){
			for (l = 0; l<10; l++){
				boardcopy[k][l] = board[k][l];
			}
		}
		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (is_legal_move(board, player, i, j)){
					set_and_turn_over(board, player, i, j);

					t = -saiki(board, player, ai_player, limit - 1, -b, -a);

					if (t>a && t < beta && first_node == OFF && limit>2){
						t = -saiki(board, player, ai_player, limit - 1, -beta, -t);//←　-tを-alphaに変えてみる
					}
					first_node = OFF;
					for (k = 0; k<10; k++){
						for (l = 0; l<10; l++){
							board[k][l] = boardcopy[k][l];
						}
					}
					if (t>score_max){
						if (t >= beta) return t;
						score_max = t;
						if (t>a){
							a = t;
						}
					}
					b = a + 1;
				}
			}
		}


		return score_max;
	}
	int move(int board[10][10], int player, int ai_player, int limit){
		int place = 0;
		int i, j;
		int score_max = -9000, score;
		int boardcopy[10][10], p, q;
		int alpha = -10000, beta = 10000;

		for (p = 0; p<10; p++){
			for (q = 0; q<10; q++){
				boardcopy[p][q] = board[p][q];
			}
		}

		for (i = 1; i <= 8; i++){
			for (j = 1; j <= 8; j++){
				if (is_legal_move(board, player, i, j)){
					set_and_turn_over(board, player, i, j);
					score = -saiki(board, player, ai_player, limit, -beta, -alpha);
					/*着手箇所の点数付け*/

					for (p = 0; p<10; p++){
						for (q = 0; q<10; q++){
							board[p][q] = boardcopy[p][q];
						}
					}

					//				printf("moveが%d%dに打った時のscoreは%d\n",i,j,score);
					if (score>score_max){
						score_max = score;
						place = 10 * i + j;
						//					printf("%dが有力な手です",place);
					}
				}

			}
		}
		//printf("%d",calc_num);
		return place;
	}
}
;

