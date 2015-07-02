#include "stdafx.h"
#include "sampleAI.h"

sampleAI::sampleAI()
{
}


sampleAI::~sampleAI()
{
}

/*
���ォ��u����Ƃ��낪���邩�T���Ă����A�ŏ��Ɍ��������u����Ƃ���ɒu���v���O����
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
	//��A�E��A�E�A�E���A���A�����A���A����A��̏��ԂɕԂ���Ƃ��낪���邩������
	int dire_x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int dire_y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

	//���̏ꏊ���󔒂łȂ���Βu���Ȃ�
	if (board[y][x] != blank) return false;

	//8�����Ԃ���΂����邩���ׂ�
	for (int i = 0; i < 8; i++){
		int cnt = 0;
		int next_x = x;
		int next_y = y;

		/*
		���߂������ɒu�������ʒu����i�߂Ă����B
		�i�߂�������
		�G�̐�->�������ɐi�߂�
		�����̐�->�i�߂��񐔂�2��ȏ�Ȃ�ΕԂ���΂�����B
		�󔒁A�͈͊O->�Ԃ���΂��Ȃ�
		*/
		while (true){
			//���ׂ�ʒu�����炷
			next_x += dire_x[i];
			next_y += dire_y[i];
			//����i�߂���
			cnt++;

			//�{�[�h�̊O
			if (next_x < 0 || next_x > 7 || next_y < 0 || next_y > 7) break;

			int coler = board[next_y][next_x];
			//��
			if (coler == blank) break;
			//�����̐F
			if (coler == turn){
				if (cnt >= 2) return true;
				else break;
			}
		}
	}
	return false;
}
