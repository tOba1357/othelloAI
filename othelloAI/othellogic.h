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
	const int AI_START = 49; //��ɐݒ肷��
	const int MID_LIMIT = 7;//��ɐݒ�

	/*�]�T��̕]���֐������@�E�C���O�ɂȂ邪�]�T��Ȃǁ@��Ԃ�ύX���Ă����肪�łĂȂ��Ȃ�*/
	//�E�C���O�͈����Ƃ����]���֐������@�p����邱�Ƃ͗ǂ����ƂƂ����]���֐������@�m��΂Ȃ�
	//���Ղ̕]���֐��ōŌ�܂ŒT�������ɁA�r���ő΋ǏI���ɂȂ����Ƃ��́A
	//�΂̐��𐔂��đ����ق����{�P�O�O�O0�ȂǂƂ���
	//�������ꂽ��ӂ̕]���͂��Ȃ�

	int calc_num = 0;//�v�Z�񐔂̕\���Ɏg��

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
		/*�����Ֆʂ̔z�u*/
		init_board(board);
		//print_board(board);
		ai_player = turn;
		player = turn;
		tesuu = 0;
		for (i = 0; i < 8; i++) for (j = 0; j < 8; j++) board[i + 1][j + 1] = a_board[i][j];
		for (i = 0; i < 8; i++) for (j = 0; j < 8; j++) if (a_board[i][j] == turn) tesuu++;

		while (run_flag == 1){
			if (true){//(is_legal_move(board, player, x, y) == 1 && later == 1) || later == 2){/*�v���C���[���u����ꏊ�ɑł����Ȃ�{�[�h���v�����g*/				later = 1;
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

						printf("�v�Z�񐔂�%d��ł�\n", calc_num);
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
						printf("�R���s���[�^�̓p�X�ł�");
						//						printf("computer�̒l�F%d",computer);
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
				printf("�����ɂ͒u���܂���\a\n");
				change_flag = OFF;
				later = 1;
			}

		}


		printf("�΋ǂ��I�����܂���\n");
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
		printf("���̌���%d�ł�\n", count_black);
		printf("���̌���%d�ł�\n", count_white);

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
	/*�Ֆʂ��v�����g����֐�*/
	void print_board(int board[10][10])
	{
		int i, j;
		printf(" �P�Q�R�S�T�U�V�W");
		for (i = 0; i < 10; i++) {
			if (i >= 1 && i <= 10 - 2)
				printf("%d", i);
			else
				printf(" ");
			for (j = 0; j < 10; j++) {
				if (board[i][j] == WHITE)
					printf("��");
				if (board[i][j] == BLACK)
					printf("��");
				if (board[i][j] == BLANK)
					printf("��");
				if (board[i][j] == WALL)
					printf("");
			}
			printf("\n");
		}
	}
	/*���Ԃ����𐔂���i�P�����̂݁j*/
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
	/*���ׂĂ̕����ŗ��Ԃ��΂����邩�ǂ������ׁA�P�ł��Ԃ���΂�����Ȃ�u����Ɣ��f����*/

	int is_legal_move(int board[10][10], int player, int p, int q)
	{
		if (p < 1 || p > 8 || q < 1 || q > 8) return 0;
		if (board[p][q] != 0) return 0;
		if (count_turn_over(board, player, p, q, -1, 0)) return 1;  /* �� */
		if (count_turn_over(board, player, p, q, 1, 0)) return 1;  /* �� */
		if (count_turn_over(board, player, p, q, 0, -1)) return 1;  /* �� */
		if (count_turn_over(board, player, p, q, 0, 1)) return 1;  /* �E */
		if (count_turn_over(board, player, p, q, -1, -1)) return 1;  /* ���� */
		if (count_turn_over(board, player, p, q, -1, 1)) return 1;  /* �E�� */
		if (count_turn_over(board, player, p, q, 1, -1)) return 1;  /* ���� */
		if (count_turn_over(board, player, p, q, 1, 1)) return 1;  /* �E�� */
		return 0;
	}

	int exist_move(int board[10][10], int player)/*return�ŕԂ��l�����R��������int�^�Ő錾*/
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
		/*�΂𗠕Ԃ��֐�*/
		int count, d, e, i;

		for (d = -1; d <= 1; d++) {/* �㉺���� */
			for (e = -1; e <= 1; e++) {/* ���E���� */
				if (d == 0 && e == 0) continue;/*0�̂ݏ��O���Ď��̃��[�v��*/
				count = count_turn_over(board, player, x, y, d, e);/*������ɗ��Ԃ����𒲂ׂ�*/
				for (i = 1; i <= count; i++) {
					board[x + i*d][y + i*e] = player; /* ���Ԃ� */
				}
			}
		}
		board[x][y] = player; /* �΂�u�� */
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

		printf("��������͂��Ă�������: ");
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
			/*�p�X�̐ݒ�Q��Ƃ��A���Ńp�X��������΋ǏI��*/
			//		printf("�p�X�I");

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

		//�S�ł̏���

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

		for (i = 1; i <= 8; i++){//���ӂ̃R�s�[
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[i][j];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);
		for (i = 1; i <= 8; i++){//��ӂ̃R�s�[
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[j][i];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);
		for (i = 1; i <= 8; i++){//�E�ӂ̃R�s�[
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[8 - i + 1][8 - j + 1];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);
		for (i = 1; i <= 8; i++){//���ӂ̃R�s�[
			for (j = 1; j <= 2; j++){
				edge[i - 1][j - 1] = board[8 - j + 1][i];
			}
		}
		eval_score = eval_score + edge_eval(edge, ai_player, board);

		return eval_score;
	}

	int edge_eval(int edge[8][2], int ai_player, int board[10][10]){
		//edge[8][2]�͓Y��0����n�܂��Ă���B
		int x = 0, i, y;
		int ev = 0;
		int j;
		int count_disc = 0;
		int human;

		human = 3 - ai_player;

		if (edge[0][0] == BLANK&&edge[7][0] == BLANK){
			//�v���C���[�͐l�ԁ@����̌`�𔻕�
			//�]�T���?�Q�T
			if (edge[2][0] == human && edge[3][0] == human && edge[4][0] == human && edge[5][0] == human){
				//�΂��S�ӂɕ���ł���
				if (edge[1][0] == human&&edge[6][0] == BLANK){
					if (edge[5][1] == ai_player){
						//�E�C���O�ɂȂ炸�ɗ]�T��ɂȂ���
						ev = ev - 30;
					}
					else if (edge[5][1] == human){
						ev = ev + 10;
						//�E�C���O�ɂȂ���
					}
				}
				else if (edge[1][0] == BLANK&&edge[6][0] == human){
					if (edge[2][1] == ai_player){
						//�E�C���O�ɂȂ炸�ɗ]�T��ɂȂ���
						ev = ev - 30;
					}
					else if (edge[2][1] == human){
						ev = ev + 10;
						//�E�C���O�ɂȂ���
					}
				}
				else if (edge[1][0] == human&&edge[6][0] == human){
					//�R�ɂȂ���
					ev = ev - 20;
				}
				else if (edge[1][0] == BLANK && edge[6][0] == BLANK){
					//�u���b�N���]�T�肩
					if (edge[2][1] == human && edge[5][1] == human){
						ev = ev + 8;
					}
					else{
						//����̗]�T��ɂȂ�
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
				//human�̐F��T���āA�΂���������
				//���A�����𒲂ׂĂR�ȏ�Ȃ�A���̐΂̒[�̈��̐F�𒲂ׂāA
				//����̐F�Ȃ�_����t����
				x = 7;
				while (x >= 1){
					//edge[x][0]��human�ɂȂ����炻�̍��W�𒊏o����
					if (edge[x][0] == human){
						break;
					}
					//���_�@����΂��Ȃ�������7�܂ő����Ă��܂�
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
				if (count_disc <= 2){//�A���̐΂̌�
					if (edge[1][0] == human){
						ev = ev + 20;
					}
					if (edge[6][0] == human){
						ev = ev + 20;
					}
				}
				/*
				else{//3�ȏ�
				if (edge[y][1] == ai_player){
				ev = ev - 20;
				}
				if (edge[x + 1][1] == ai_player){
				ev = ev - 20;
				}
				}
				*/

			}
			//ai_player�@�n�܂�

			if (edge[2][0] == ai_player && edge[3][0] == ai_player && edge[4][0] == ai_player && edge[5][0] == ai_player){
				//�΂��S�ӂɕ���ł���
				if (edge[1][0] == ai_player&&edge[6][0] == BLANK){
					if (edge[5][1] == human){
						//�E�C���O�ɂȂ炸�ɗ]�T��ɂȂ���
						ev = ev + 30;
					}
					else if (edge[5][1] == ai_player){
						ev = ev - 10;//�E�C���O�ɂȂ���
					}
				}
				else if (edge[1][0] == BLANK&&edge[6][0] == ai_player){
					if (edge[2][1] == human){
						//�E�C���O�ɂȂ炸�ɗ]�T��ɂȂ���
						ev = ev + 30;
					}
					else{
						ev = ev - 10;
					}
				}
				else if (edge[1][0] == ai_player&&edge[6][0] == ai_player){
					//�R�ɂȂ���
					ev = ev + 20;
				}
				else if (edge[1][0] == BLANK && edge[6][0] == BLANK){
					//�u���b�N���]�T�肩
					if (edge[2][1] == ai_player && edge[5][1] == ai_player){//�u���b�N
						ev = ev - 8;
					}
					else{
						//����̗]�T��ɂȂ�
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
				//human�̐F��T���āA�΂���������
				//���A�����𒲂ׂĂR�ȏ�Ȃ�A���̐΂̒[�̈��̐F�𒲂ׂāA
				//����̐F�Ȃ�_����t����
				x = 7;
				while (x >= 1){
					//edge[x][0]��human�ɂȂ����炻�̍��W�𒊏o����
					if (edge[x][0] == ai_player){
						break;
					}
					//���_�@����΂��Ȃ�������7�܂ő����Ă��܂�
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
				if (count_disc<3){//�A���̐΂̌���3�ȉ��ŒP��c�ł�
					if (edge[1][0] == ai_player){
						ev = ev - 20;
					}
					if (edge[6][0] == ai_player){
						ev = ev - 20;
					}
				}
				/*
				else{//3�ȏ�
				if (edge[y][1] == human){
				ev = ev + 20;
				}
				if (edge[x + 1][1] == human){
				ev = ev + 20;
				}
				}
				*/

			}
			//ai_player�I���
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

		calc_num++;//�v�Z�񐔂̕\��
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
						t = -saiki_put(board, player, ai_player, limit - 1, -beta, -t);//���@-t��-alpha�ɕς��Ă݂�
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
					/*����ӏ��̓_���t��*/

					for (p = 0; p<10; p++){
						for (q = 0; q<10; q++){
							board[p][q] = boardcopy[p][q];
						}
					}

					//				printf("move��%d%d�ɑł�������score��%d\n",i,j,score);
					if (score>score_max){
						score_max = score;
						place = 10 * i + j;
						//					printf("%d���L�͂Ȏ�ł�",place);
					}
				}

			}
		}
		//printf("%d",calc_num);
		return place;
	}



	//2�񏑂��Ȃ���΂����Ȃ��Ƃ���͊֐����Ăяo���Ēl��ύX����B
	int saiki(int board[10][10], int player, int ai_player, int limit, int alpha, int beta){

		int i, j, k, l, score, score_max = -9000;
		int boardcopy[10][10];
		int exist;
		int t, b, a;
		int move[40][3];
		int first_node = ON;

		a = alpha;
		b = beta;

		calc_num++;//�v�Z�񐔂̕\��
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
						t = -saiki(board, player, ai_player, limit - 1, -beta, -t);//���@-t��-alpha�ɕς��Ă݂�
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
					/*����ӏ��̓_���t��*/

					for (p = 0; p<10; p++){
						for (q = 0; q<10; q++){
							board[p][q] = boardcopy[p][q];
						}
					}

					//				printf("move��%d%d�ɑł�������score��%d\n",i,j,score);
					if (score>score_max){
						score_max = score;
						place = 10 * i + j;
						//					printf("%d���L�͂Ȏ�ł�",place);
					}
				}

			}
		}
		//printf("%d",calc_num);
		return place;
	}
}
;

