#include "stdafx.h"
#include "HashBoard.h"


HashBoard::HashBoard()
{
	hash_table = new hash_value*[max_size_i]{};
	hash_table_size = new int[max_size_i];
	cnt = 0;
}


HashBoard::~HashBoard()
{
	for (int i = 0; i < max_size_i; i++){
		if (hash_table[i] != NULL) delete hash_table[i];
	}
	delete hash_table;
	delete hash_table_size;
}

bool HashBoard::insert(hash_key hk, std::vector<hash_value> vec){
	int idx = key2int(hk);
	if (hash_table[idx] != NULL) return false;
	hash_table[idx] = new hash_value[vec.size()];
	hash_table_size[idx] = vec.size();
	for (int i = 0; i < vec.size(); i++){
		hash_table[idx][i] = vec[i];
	}
	cnt++;
	return true;
}

std::vector<HashBoard::hash_value> HashBoard::at(hash_key hk)
{
	int idx = key2int(hk);
	std::vector<hash_value> rtn;
	if (hash_table[idx] == NULL) return rtn;
	rtn.resize(hash_table_size[idx]);
	for (int i = 0; i < hash_table_size[idx]; i++){
		rtn[i] = hash_table[idx][i];
	}
	return rtn;
}

HashBoard::hash_key HashBoard::board2key(int board[10][10])
{
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


int HashBoard::key2int(hash_key hk)
{
	long long int rtn = 0;
	rtn = hk.head*17;
	rtn ^= hk.tail/3;
	return abs(rtn)%max_size_i;
}




int HashBoard::size()
{
	return cnt;
}


int HashBoard::max_size()
{
	return max_size_i;
}
