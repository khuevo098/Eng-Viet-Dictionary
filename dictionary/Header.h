#pragma once
#include<iostream>
#include <fstream>
#include<string>
using namespace std;
static const int DEFAULT_SIZE = 151;
/* Cau truc node cua Hash Table cai dat tu dien */

struct Node {
	string key; // search key
	string value; // Data item
	Node* nextPtr;
};
/* Cau truc Hash Table */
struct HashTable {
	int itemCount; // Count of dictionary entries
	int hashTableSize; // Table size must be prime
	Node** hashTable; // Array of pointers to entries
};
/* Ham khoi tao Bang bam */
void init_Hash(HashTable& h);
/* Ham bam */


//Hàm kiểm tra hash table có rỗng
bool is_Empty(HashTable hashtable);

//Hàm băm
int hashFunc(HashTable h, int x);
/* Ham them 1 item vao bang bam */
void add(HashTable& h, string key, string value);


/* Ham xoa 1 item tu Hash Table */
void remove(HashTable& h, const string& searchKey);
//Hàm đọc dữ liệu từ file
void data_from_file(HashTable& hashtable);

//Hàm ghi dữ liệu ra file
void data_to_file(HashTable hashtable);

//Hàm tìm kiếm nghĩa của từ vựng
void search(HashTable hashtable, string key);
//Hàm xóa 1 từ vựng
void delete_vocab(HashTable& hashtable, string key);
//Hàm giải phóng hash table
void free_hashtable(HashTable& hashtable);