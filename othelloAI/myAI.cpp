#include "stdafx.h"
#include "myAI.h"


myAI::myAI()
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
	time = 0;
	::AllocConsole();
	freopen_s(&fpw, "CON", "w", stdout);
	std::cout << hash_map.max_size();
}


myAI::~myAI()
{
	fclose(fpw);
	::FreeConsole();
}


void myAI::AI(int board[8][8], int turn, int *x, int *y)
{
	clock_t tmp_time = clock();
	this->turn = turn;
	setBoard(board);
	startAI(x, y);
	time += clock() - tmp_time;
	std::cout << "time:" << time << std::endl;
}


void myAI::setBoard(int board[8][8])
{
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			raw_board[i][j] = -1;
		}
	}
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			raw_board[i+1][j+1] = board[i][j];
		}
	}
}


bool myAI::isPut(int board[10][10], int turn, int x, int y)
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


void myAI::startAI(int *x, int *y)
{
	int blank_num = getNum(raw_board, BLANK);
/*	//****** debug
	int b_board[10][10], a_board[10][10];
	copyBoard(b_board, raw_board);
	hash_key hv = board2hash(b_board);
	hash2board(a_board, hv);
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			if (a_board[i][j] != b_board[i][j]) AfxMessageBox(_T("ERROR"));
		}
	}
	//****** end debug*/
	if (LAST_STAGE_NUM < blank_num){
		firstAI(x, y);
	}
	else{
		lastAI(x, y);
	}
}


int myAI::getPutNum(int board[10][10], int turn)
{
	int cnt = 0;
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (isPut(board, turn, i, j)) cnt++;
		}
	}
	return cnt;	
}


void myAI::lastAI(int *x, int *y)
{
	std::vector<int> tmp_x, tmp_y;
	int num = lastDFS(raw_board, turn, 0, -9999, 9999, x, y);
	std::cout << *x << " " << *y << " " << num <<std::endl;
	show(raw_board);
}


void myAI::firstAI(int* x, int* y)
{
	int num;
	if (hash_flag){
		std::cout << hash_map.size() << std::endl;
		num = firstDFS_hash({ board2hash(raw_board), turn}, 0, INT_MIN, INT_MAX, x, y);
	}
	else{
		num = firstDFS(raw_board, turn, 0, INT_MIN, INT_MAX, x, y);
	}
	put(raw_board, (*y) + 1, (*x) + 1, turn);
	std::cout << "x:" << *x << " y:" << *y << " rtn:"<< num << std::endl;
	show(raw_board);
	bool fix_storn_map[10][10];
	setFixStornMap(raw_board, fix_storn_map);
	show(fix_storn_map);
	return;

}



int myAI::lastDFS(int board[10][10], int turn, int n, int a, int b, int *x, int *y)
{
	if (turn == BLANK) return getNum(board, this->turn) - getNum(board, this->turn^3);
	
	int my_rtn = INT_MIN, enemy_rtn = INT_MAX;
	int sent_board[10][10];
	copyBoard(sent_board, board);

	std::vector<pos > log;
	log = getPutList(board, turn);
	for (int i = 0; i < log.size(); i++){
		pos put_pos = log[i];
		put(sent_board, put_pos.x, put_pos.y, turn);
		if (this->turn == turn){
			int num = lastDFS(sent_board, getNextTurn(sent_board, turn), n + 1, my_rtn, b, x, y);
			if (b < num) return num;
			if (n == 0 && (my_rtn < num)){
				*y = put_pos.x - 1; *x = put_pos.y - 1;
			}
			my_rtn = max(my_rtn, num);
		}
		else{
			int num = lastDFS(sent_board, getNextTurn(sent_board, turn), n + 1, a, enemy_rtn, x, y);
			if (a > num) return num;
			enemy_rtn = min(enemy_rtn, num);
		}
		copyBoard(sent_board, board);
	}
	return (this->turn == turn) ? my_rtn : enemy_rtn;

}


int myAI::getNum(int board[10][10], int turn)
{
	int cnt = 0;
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (board[i][j] == turn) cnt++;
		}
	}
	return cnt;
}


bool myAI::put(int board[10][10], int x, int y, int turn)
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
					for(int k = 1; k <= cnt; k++){
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


int myAI::getNextTurn(int board[10][10], int turn)
{
	if (getPutList(board, turn ^ 3).size() != 0) return turn ^ 3;
	if (getPutList(board, turn).size() != 0) return turn;
	return BLANK;
}


int myAI::getPutCount(int board[10][10], int turn)
{
	int cnt = 0;
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if ((i == 2 && j == 2) || (i == 2 && j == 7) || (i == 7 && j == 2) || (i == 7 && j == 7)) continue;
			if(isPut(board, turn, i, j)) cnt++;
		}
	}
	return cnt;
}


int myAI::firstDFS(int board[10][10], int turn, int n, int a, int b, int *x, int *y)
{
	if (n >= N || turn == BLANK) return getEvaluationValue(board,turn,this->turn);
	int sent_board[10][10];
	copyBoard(sent_board, board);
	int my_rtn = INT_MIN, enemy_rtn = INT_MAX;
		std::vector<pos> log;
		log = getPutList(board, turn);
		for (int i = 0; i < log.size(); i++){
			pos put_pos = log[i];
			put(sent_board, put_pos.x, put_pos.y, turn);
			int next_turn = getNextTurn(sent_board, turn);
			if (this->turn == turn){
				int num = firstDFS(sent_board, next_turn, n + 1, my_rtn, b, x, y);
				if ((N - 1) <= n){
					num += log.size();
				}
				if (b < num) return num;
				if (n == 0 && (my_rtn < num)){
					*y = put_pos.x - 1; *x = put_pos.y - 1;
				}
				my_rtn = max(my_rtn, num);
			}
			else{
				int num = firstDFS(sent_board, next_turn, n + 1, a, enemy_rtn, x, y);
				if ((N - 1) <= n){
					num -= log.size();
				}
				if (a > num) return num;
				enemy_rtn = min(enemy_rtn, num);
			}
			copyBoard(sent_board, board);
		}

	return (this->turn == turn) ? my_rtn : enemy_rtn;
}


int myAI::getEvaluationValue(int board[10][10], int turn, int my_turn)
{
	int rtn = 0;
	if (turn == BLANK){
		if (getWinner(board) == my_turn){
			return INT_MAX;
		}else{
			return INT_MIN;
		}
	}
	rtn = getFixStoneNum(board, my_turn);
	if (turn == my_turn){
		rtn += getPutList(board, turn).size();
	}
	else{
		rtn -= getPutList(board, turn).size();
	}
	return rtn;
}


void myAI::copyBoard(int board1[10][10], int board2[10][10])
{
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			board1[i][j] = board2[i][j];
		}
	}
}

void myAI::show(int board[10][10]){
	/*std::cout << "{";
	for (int i = 1; i <= 8; i++){
		std::cout << "{";
		for (int j = 1; j <= 8; j++){
			std::cout << board[i][j];
			if(j!=8) std::cout << ",";
		}
		std::cout << "}";
		if (i == 8) std::cout << "};";
		else std::cout << ",";
		std::cout << std::endl;
	}*/
	using namespace std;
	for (int i = 1; i <= 8; i++){
		showLine(board[i]);
	}
}

void myAI::setPutMap(int board[10][10], bool put_map[10][10], int turn)
{
	bool storn_map[10][10] = {false};
	int enemy_turn = turn ^ 3;

	for (int i = 1; i < 9; i++){
		for (int j = 1; j < 9; j++){
			if (board[i][j] == enemy_turn){
				storn_map[i][j] = true;
				put_map[i + 1][j] = true;
				put_map[i + 1][j + 1] = true;
				put_map[i + 1][j - 1] = true;
				put_map[i - 1][j] = true;
				put_map[i - 1][j+1] = true;
				put_map[i - 1][j-1] = true;
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


std::vector<myAI::pos> myAI::getPutList(int board[10][10], int turn)
{
	std::vector<pos > put_list;
	bool put_map[10][10] = {false};
	setPutMap(board, put_map, turn);
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (put_map[i][j]){
				if (isPut(board, turn, i, j)){
					put_list.push_back({ i, j });
				}
			}
		}
	}
	return put_list;
}

int myAI::getFixStoneNum(int board[10][10], int turn)
{
	bool fix_stone_map[10][10] = { true };
	setFixStornMap(board, fix_stone_map);
	int my_cnt = 0, enemy_cnt = 0;
	//showFixStoneMap(fix_stone_map);
	//show(board);
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (fix_stone_map[i][j]){
				if (turn == board[i][j]){
					my_cnt++;
				}
				else{
					enemy_cnt++;
				}
			}
		}
	}
	return (my_cnt - enemy_cnt * 10) * 20;
}

void myAI::setFixStornMap(int board[10][10], bool fix_storn_map[10][10])
{
	//横ライン
	for (int i = 1; i <= 8; i++){
		setFixStornLine(board[i], turn, fix_storn_map[i]);
	}
	//縦ライン
	for (int i = 1; i <= 8; i++){
		bool fix_stoe_line[10];
		int line[10];
		for (int j = 0; j < 10; j++){
			line[j] = board[j][i];
		}
		setFixStornLine(line, turn, fix_stoe_line);
		for (int j = 1; j <= 8; j++){
			fix_storn_map[j][i] &= fix_stoe_line[j];
		}
	}
	//左上から右下斜めライン
	//上から１、２つ目と下から1,2つ目は固定石
	for (int i = 0; i <= 5; i++){
		bool fix_storn_line[10];
		int line[10];
		for (int j = 0; j < 10; j++){
			int y_idx = i + j;
			if (y_idx < 10){
				line[j] = board[j][y_idx];
			}
			else{
				line[j] = -1;
			}
		}
		setFixStornLine(line, turn, fix_storn_line);
		for (int j = 0; j < 10; j++){
			int y_idx = i + j;
			if (y_idx < 10){
				fix_storn_map[j][y_idx] &= fix_storn_line[j];
			}
			else{
				break;
			}
		}
	}
	for (int i = 1; i <= 5; i++){
		bool fix_storn_line[10];
		int line[10];
		for (int j = 0; j < 10; j++){
			int x_idx = i + j;
			if (x_idx < 10){
				line[j] = board[x_idx][j];
			}
			else{
				line[j] = -1;
			}
		}
		setFixStornLine(line, turn, fix_storn_line);
		for (int j = 0; j < 10; j++){
			int x_idx = i + j;
			if (x_idx < 10){
				fix_storn_map[x_idx][j] &= fix_storn_line[j];
			}
			else{
				break;
			}
		}
	}
	//右上から左下斜めライン
	//上から１、２つ目と下から1,2つ目は固定石
	for (int i = 4; i < 10; i++){
		bool fix_storn_line[10];
		int line[10];
		for (int j = 0; j < 10; j++){
			int y_idx = i - j;
			if (y_idx >= 0){
				line[j] = board[j][y_idx];
			}
			else{
				line[j] = -1;
			}
		}
		setFixStornLine(line, turn, fix_storn_line);
		for (int j = 0; j < 10; j++){
			int y_idx = i - j;
			if (y_idx >= 0){
				fix_storn_map[j][y_idx] &= fix_storn_line[j];
			}
			else{
				break;
			}
		}
	}
	for (int i = 1; i <= 5; i++){
		bool fix_storn_line[10];
		int line[10];
		for (int j = 0; j < 10; j++){
			int y_idx = i + j;
			int x_idx = 9 - j;
			if (y_idx < 0){
				line[j] = board[x_idx][y_idx];
			}
			else{
				line[j] = -1;
			}
		}
		setFixStornLine(line, turn, fix_storn_line);
		for (int j = 0; j < 10; j++){
			int y_idx = i + j;
			int x_idx = 9 - j;
			if (y_idx < 10){
				fix_storn_map[x_idx][y_idx] &= fix_storn_line[j];
			}
			else{
				break;
			}
		}
	}
}

void myAI::setFixStornLine(int line[10], int turn, bool fix_storn_line[10])
{
	for (int i = 0; i < 10; i++) fix_storn_line[i] = true;
	bool end_flag = false;
	for (int i = 1; i <= 8; i++){
		if (line[i] != BLANK) continue;
		
		//連続する空白を数える
		int cnt = 0;
		while (true){
			if (line[i] == BLANK){
				fix_storn_line[i++] = false;
				cnt++;
			}
			else break;
		}
		
		//終端まで空白だったら終了
		if (line[i] == -1) break;
		
		//空白の隣は不定石　空白の個数だけ色層が固定石ではなくなる
		int color = line[i];
		while (true){
			//空白であった場合終了
			if (line[i] == BLANK) break;
			//終端であった場合、色層が変わるまで、固定石
			if (line[i] == -1){
				i--;
				color = line[i];
				end_flag = true;
				while (true){
					if (line[i] == color){
						fix_storn_line[i--] = true;
					}else{
						break;
					}
				}
				break;
			}
			//色層が変わった場合、カウントを下げる。
			//カウントが0になった場合、そこからは固定石の可能性がある。
			if (color != line[i]){
				cnt--;
				if (cnt <= 0) break;
				color = line[i];
			}
			fix_storn_line[i] = false;
			i++;
		}	
		if (end_flag) break;
	}
	end_flag = false;
	for (int i = 8; i >= 1; i--){
		if (line[i] != BLANK) continue;

		//連続する空白を数える
		int cnt = 0;
		while (true){
			if (line[i] == BLANK){
				fix_storn_line[i--] = false;
				cnt++;
			}
			else break;
		}

		//終端まで空白だったら終了
		if (line[i] == -1) break;

		//空白の隣は不定石　空白の個数だけ色層が固定石ではなくなる
		int color = line[i];
		while (true){
			//空白であった場合終了
			if (line[i] == BLANK) break;
			//終端であった場合、色層が変わるまで、固定石
			if (line[i] == -1){
				i++;
				color = line[i];
				end_flag = true;
				while (true){
					if (line[i] == color){
						fix_storn_line[i++] = true;
					}
					else{
						break;
					}
				}
				break;
			}
			//色層が変わった場合、カウントを下げる。
			//カウントが0になった場合、そこからは固定石の可能性がある。
			if (color != line[i]){
				cnt--;
				if (cnt <= 0) break;
				color = line[i];
			}
			fix_storn_line[i] = false;
			i--;
		}
		if (end_flag) break;
	}
}


void myAI::showFixStoneMap(bool board[10][10])
{
	using namespace std;
	for (int i = 1; i <= 8; i++){
		showLine(board[i]);
	}
}


void myAI::showLine(bool line[10])
{
	using namespace std;
	for (int i = 1; i <= 8; i++){
		if (line[i]){
			cout << "o";
		}
		else{
			cout << "x";
		}
	}
	cout << endl;
}


void myAI::showLine(int line[10])
{
	using namespace std;
	for (int i = 1; i <= 8; i++){
		if (line[i] == BLANK){
			cout << "＋";
		}
		else if(line[i] == BLACK){
			cout << "○";
		}
		else{
			cout << "●";
		}
	}
	cout << endl;
}


void myAI::show(bool board[10][10])
{
	using namespace std;
	for (int i = 1; i <= 8; i++){
		showLine(board[i]);
	}
	cout << endl;
}


myAI::hash_key myAI::board2hash(int board[10][10]){
	hash_key rtn;
	rtn.head = rtn.tail = 0;
	for (int i = 1; i <= 4; i++){
		for (int j = 1; j <= 8; j++){
			rtn.head = rtn.head << 2;
			rtn.head |= board[i][j];
		}
	}
	for (int i = 5; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			rtn.tail = rtn.tail << 2;
			rtn.tail |= board[i][j];
		}
	}

	return rtn;
}

void myAI::hash2board(int board[10][10], myAI::hash_key hv)
{
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			board[i][j] = -1;
		}
	}
	for (int i = 4; i >= 1; i--){
		for (int j = 8; j >= 1; j--){
			board[i][j] = hv.head & 3;
			hv.head = hv.head >> 2;
		}
	}
	for (int i = 8; i >= 5; i--){
		for (int j = 8; j >= 1; j--){
			board[i][j] = hv.tail & 3;
			hv.tail = hv.tail >> 2;
		}
	}
}


myAI::pos myAI::searchPutPos(int b_board[10][10], int a_board[10][10])
{
	//show(b_board);
	//std::cout << std::endl;
	//show(a_board);
	for (int i = 1; i <= 8; i++){
		for (int j = 1; j <= 8; j++){
			if (b_board[i][j] == BLANK && a_board[i][j] != BLANK){
				pos rtn = { i, j };
				return rtn;
			}
		}
	}
	std::cout << "ERROR";
}



bool myAI::checkHash(int board[10][10], hash_key hv)
{
	int hash_board[10][10];
	hash2board(hash_board, hv);
	return (getNum(hash_board,BLANK)+1) == getNum(board,BLANK);
}


int myAI::firstDFS_hash(hash_value hv, int n, int a, int b, int *x, int *y)
{
	int turn = hv.turn;
	if (n >= N || turn == BLANK){
		int board[10][10];
		hash2board(board, hv.hk);
		return getEvaluationValue(board, turn, this->turn);
	}
	int my_rtn = INT_MIN, enemy_rtn = INT_MAX;	
	std::vector<hash_value> next_boards = hash_map.at(hv.hk);
	if (next_boards.size() != 0 && checkHash(hv.hk, next_boards[0].hk)){		
		for (int i = 0; i < next_boards.size(); i++){
			if (this->turn == turn){
				int num = firstDFS_hash(next_boards[i], n + 1, my_rtn, b, x, y);
				if ((n + 1) == N) num += next_boards.size();
				if (b < num) return num;
				if (n == 0 && (my_rtn < num)){
					int board[10][10], next_board[10][10];
					hash2board(board, hv.hk);
					hash2board(next_board, next_boards[i].hk);
					pos tmp = searchPutPos(board, next_board);
					*x = tmp.y - 1; *y = tmp.x - 1;
				}
				my_rtn = max(my_rtn, num);
			}
			else{
 				int num = firstDFS_hash(next_boards[i], n + 1, a, enemy_rtn, x, y);
				if ((n+1) == N) num -= next_boards.size();
				if (a > num) return num;
				enemy_rtn = min(enemy_rtn, num);
			}
		}
	}else{
		//ハッシュが登録されていないとき
		int board[10][10], sent_board[10][10];
		std::vector<hash_value> add_hash_list;
		hash2board(board, hv.hk);
		copyBoard(sent_board, board);
		std::vector<pos> log;
		log = getPutList(board, turn);
		for (int i = 0; i < log.size(); i++){
			pos put_pos = log[i];
			put(sent_board, put_pos.x, put_pos.y, turn);
			int next_turn = getNextTurn(sent_board, turn);
			add_hash_list.push_back({ board2hash(sent_board), next_turn });

			if (this->turn == turn){
				int num = firstDFS_hash(add_hash_list[i], n + 1, my_rtn, b, x, y);
				if ((n + 1) == N) num += log.size();
				if (b < num){
					//残りの盤面の登録
					i++;
					for (; i < log.size(); i++){
						put_pos = log[i];
						copyBoard(sent_board, board);
						put(sent_board, put_pos.x, put_pos.y, turn);
						next_turn = getNextTurn(sent_board, turn);
						add_hash_list.push_back({ board2hash(sent_board), next_turn });
					}
					if (!hash_map.insert(hv.hk,add_hash_list)){
						if (hash_map.at(hv.hk).size() == 0) AfxMessageBox(_T("ERROR"));
					}
					return num;
				}
				if (n == 0 && (my_rtn < num)){
					*y = put_pos.x - 1; *x = put_pos.y - 1;
				}
				my_rtn = max(my_rtn, num);
			}
			else{
				int num = firstDFS_hash(add_hash_list[i], n + 1, a, enemy_rtn, x, y);
				if ((n + 1) == N) num -= log.size();
				if (a > num){
					//残りの盤面の登録
					i++;
					for (; i < log.size(); i++){
						put_pos = log[i];
						copyBoard(sent_board, board);
						put(sent_board, put_pos.x, put_pos.y, turn);
						next_turn = getNextTurn(sent_board, turn);
						add_hash_list.push_back({ board2hash(sent_board), next_turn });
					}
					if (!hash_map.insert(hv.hk, add_hash_list)){
						if (hash_map.at(hv.hk).size() == 0) AfxMessageBox(_T("ERROR"));
					}
					return num;
				}
				enemy_rtn = min(enemy_rtn, num);
			}
			copyBoard(sent_board, board);
		}
		if (!hash_map.insert(hv.hk,add_hash_list)){
			if (hash_map.at(hv.hk).size() == 0) AfxMessageBox(_T("ERROR"));
		}
	}
	return (this->turn == turn) ? my_rtn : enemy_rtn;
}


void myAI::show(hash_key hk)
{
	int tmp_board[10][10];
	hash2board(tmp_board, hk);
	show(tmp_board);
	std::cout << std::endl;
}


bool myAI::checkHash(hash_key hk1, hash_key hk2)
{
	int board[10][10];
	hash2board(board, hk1);
	return checkHash(board, hk2);
}


int myAI::getWinner(int board[10][10])
{
	int b_cnt = getNum(board, BLACK);
	int w_cnt = getNum(board, WHITE);
	if (b_cnt > w_cnt) return BLACK;
	else if (w_cnt > b_cnt) return WHITE;
	else return BLANK;
}
