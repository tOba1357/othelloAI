#pragma once
#include <vector>
class HashBoard
{
public:
	typedef struct { long long int head; long long int tail; }hash_key;
	typedef struct { hash_key hk; int turn; } hash_value;
private:
	//const int max_size_i = 9999991;
	const int max_size_i = INT_MAX/50;
	long long int head, tail;
	hash_value **hash_table;
	int *hash_table_size;
	int cnt;
public:
	HashBoard();
	~HashBoard();
	bool insert(hash_key hk, std::vector<hash_value> vec);
	hash_key board2key(int board[10][10]);
	int key2int(hash_key hk);
	std::vector<hash_value> at(hash_key hk);
	int size();
	int max_size();
};

