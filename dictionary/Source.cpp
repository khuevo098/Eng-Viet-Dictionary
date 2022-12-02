#include"Header.h"

void init_Hash(HashTable& h) {
	h.hashTableSize = DEFAULT_SIZE;
	h.itemCount = 0;
	h.hashTable = new Node*[h.hashTableSize];
	if (h.hashTable == NULL) {
		cout << "Allocate memory for Hash Table failed!" << endl;
		h.hashTableSize = 0;
	}
	for (int i = 0; i < h.hashTableSize; i++)
	{
		h.hashTable[i] = new Node;
		h.hashTable[i]->key = "#";
	}
}

bool is_Empty(HashTable hashtable) {
	for (int i = 0; i < hashtable.hashTableSize; i++)
	{
		if (hashtable.hashTable[i]->key != "#") {
			return false;
		}
	}
	return true;
}
int hashFunc(string str, int x) {
	int hash_value = 0;
	int r = 127;
	for (int k = 0; k < str.length(); ++k)
		hash_value = (hash_value * r + str[k]) % x;
	return hash_value;
}

void add(HashTable& h, string key,string value) {
	Node* node = new Node;
	if (node == NULL) {
		cout << "Allocate memory for node failed!" << endl;
		return;
	}
	node->key = key;
	node->value = value;
	node->nextPtr = NULL;
	int index = hashFunc(node->key, h.hashTableSize);
	if (h.hashTable[index]->key != "#") {
		for (Node* i = h.hashTable[index]; i != NULL; i = i->nextPtr)
		{
			if (h.hashTable[index]->key == node->key) {
				h.hashTable[index]->value += ", ";
				h.hashTable[index]->value += value;
				return;
			}
		}
	}
	Node* current_item = h.hashTable[index];
	if (current_item->key =="#") {	//Nếu chưa tồn tại key.
		
		if (h.itemCount == h.hashTableSize)/*Hash table đã đầy*/ {
			cout << "Hash Table is full!" << endl;
			//Xóa node vừa tạo
			//free(node);
			return;
		}
		h.hashTable[index] = node;
		h.itemCount++;
	}
	else {
		//trường hợp đó là node đầu tiên của hash table -> thêm kế tiếp nó lun!
		if (h.hashTable[index]->nextPtr == NULL) {
			h.hashTable[index]->nextPtr = node;
			h.itemCount++;
			return;
		}
		//trường hợp nếu linking list đó có từ 2 node trở lên
		for (Node* i = h.hashTable[index]; i != NULL; i = i->nextPtr)
		{
			if (i->nextPtr == NULL) {
				i->nextPtr = node;
				h.itemCount++;
				return;
			}
		}
	}
}

void print_HashTable(HashTable hashtable) {
	for (int i = 0; i < hashtable.hashTableSize; i++)
	{
		if (hashtable.hashTable[i]->key != "#") {
			cout << "Key: " << hashtable.hashTable[i]->key/* << ": " << hashtable.hashTable[i]->value*/;
			for (Node* j = hashtable.hashTable[i]->nextPtr; j != NULL; j = j->nextPtr) {
				cout << "  =>  ";
				cout << j->key;
			}
			cout << endl;
		}
	}
}
void data_from_file (HashTable& hashtable) {
	ifstream file;
	file.open("100_vocab.txt", ios::in);
	if (!file) {
		cout << "Error: Can't open file." << endl;
		return;
	}
	else {  
		string item_file;
		for (int i = 0; i < hashtable.hashTableSize; i++)
		{
			if (item_file == "#") {
				cout << "Complete reading data process." << endl;
				break;
			}
			getline(file, item_file);
			if (item_file != "#") {
				Node* item = new Node;
				//Bắt đầu xử lí cắt chuỗi
				int x = item_file.find(":");
				int y = x + 2;
				item->key = item_file.substr(0, x);
				item->value = item_file.substr(y, item_file.length());
				//
				//Thêm zô bảng băm
				add(hashtable, item->key, item->value);
			}

		}
		file.close();
	}
}

void search(HashTable hashtable, string key) {
	int index = hashFunc(key, hashtable.hashTableSize);
	if (hashtable.hashTable[index]->key == "#") {
		cout << "The vocabulary is not exist in this dictionary!" << endl;
		return;
	}
	else {
		int count = 0;
		cout << "Meaning of " << key << ": ";
		Node* current_item = hashtable.hashTable[index];
		for (Node* i = current_item; i !=NULL; i=i->nextPtr)
		{
			if (i->key == key) {
				count++;
				if (count > 1) {
					cout << ", " << i->value;
				}
				else {
					cout << i->value;
				}
			}
		}
		cout << "." << endl;
	}
}

void delete_vocab(HashTable& hashtable, string key) {
	int index = hashFunc(key, hashtable.hashTableSize);
	Node* current_item = hashtable.hashTable[index];
	if (current_item->key == "#"){
		cout << "The vocabulary that need to be deleted is not exist." << endl;
	}
	else {
		if (current_item->nextPtr == NULL) {
			hashtable.hashTable[index]->key = "#";
			hashtable.itemCount--;
			cout << "Delete vocab successfully." << endl;
			return;
		}
		for (Node* i = current_item; i !=NULL; i=i->nextPtr)
		{
			if (current_item->key == key) {
				Node* temp = current_item->nextPtr;
				hashtable.hashTable[index] = temp;
				free(current_item);
				cout << "Delete vocab successfully." << endl;
				return;
			}
			if (i->key == key) {
				Node* temp = i->nextPtr;
				for (Node* j = current_item; j != NULL; j=j->nextPtr)
				{
					if (j->nextPtr == i) {
						j->nextPtr = temp;
						free(i);
						cout << "Delete vocab successfully." << endl;
						return;
					}
				}
				return;
			}
		}
		cout << "Delete vocab successfully." << endl;
	}
}
void free_hashtable(HashTable& hashtable) {
	for (int i = 0; i < hashtable.hashTableSize; i++)
	{
		if (hashtable.hashTable[i]->nextPtr == NULL) {
			free(hashtable.hashTable[i]);
		}
		else {
			for (Node* temp = hashtable.hashTable[i]; temp != NULL; temp=temp->nextPtr)
			{
				Node* p = hashtable.hashTable[i];
				hashtable.hashTable[i] = p->nextPtr;
				delete p;
			}
		}
	}
	cout << "Deleted entire hash table." << endl;
}

void data_to_file(HashTable hashtable) {
	ofstream file;
	file.open("vocab_new.txt", ios::out);
	if (!file) {
		cout << "Can't open file to write data." << endl;
		return;
	}
	file << "----------" << "DICTIONARY ENGLISH - VIETNAMESE" << "----------" << endl;
	file << "Number of vocabularies: " << hashtable.itemCount << endl;
	file << endl;
	file << endl;
	for (int i = 0; i < hashtable.hashTableSize; i++)
	{
		if (hashtable.hashTable[i]->key != "#") {
			if (hashtable.hashTable[i]->nextPtr == NULL) {
				file << hashtable.hashTable[i]->key;
				file << ": ";
				file << hashtable.hashTable[i]->value << endl;
			}
			else {
				for (Node* temp = hashtable.hashTable[i]; temp != NULL; temp=temp->nextPtr)
				{
					file << temp->key;
					file << ": ";
					file << temp->value << endl;
				}
			}
		}
	}
}

int main() {
	string key;
	string value;
	bool check;
	int choose;
	HashTable hashtable;
	init_Hash(hashtable);
	after7:
	cout << "-------------------------------MENU" << "-------------------------------" << endl;
	cout << "1. Initiate HashTable with 100 vocabularies from available file." << endl;
	cout << "2. Write HashTable data to a file." << endl;
	cout << "3. Add a new vocabulary to Hash Table." << endl;
	cout << "4. Delete a vocabulary from Hash Table." << endl;
	cout << "5. Search a vocabulary in Hash Table and return its meaning." << endl;
	cout << "6. Print dictionary to screen." << endl;
	cout << "7. Clear screen." << endl;
	cout << "8. Exit program." << endl;
	cout << "------------------------------------------------------------------" << endl;
    here:
	cout << "Input choose: ";
	cin >> choose;
	while (1)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Input choose: ";
			cin >> choose;
		}
		if (!cin.fail())
			break;
	}
		switch (choose)
		{
		case 1:
			data_from_file(hashtable);
			cout << "Initiate hashtable successfully!" << endl;
			goto here;
		case 2:
			data_to_file(hashtable);
			cout << "Transfer data to hashtable successfully!" << endl;
			goto here;
		case 3:
			cout << "Input new vocab and its meaning" << endl;
			cout << "Vocab: ";
			cin.ignore();
			getline(cin, key);
			cout << "Meaning: ";
			getline(cin, value);
			add(hashtable, key, value);
			goto here;
		case 4:
			check = is_Empty(hashtable);
			if (check) {
				cout << "Dictionary is empty, there is nothing to delete!" << endl;
			}
			else {
				cout << "Input vocab that you need to delete: ";
				cin >> key;
				delete_vocab(hashtable, key);
			}
			goto here;
		case 5:
			check = is_Empty(hashtable);
			if (check) {
				cout << "Dictionary is empty, there is nothing to search!" << endl;
			}
			else {
				cout << "Input vocab that you need to search: ";
				cin >> key;
				search(hashtable, key);
			}
			goto here;
		case 6:
			check = is_Empty(hashtable);
			if (check) {
				cout << "Dictionary is empty, there is nothing to print!" << endl;
			}
			else {
				cout << "DICTIONARY ENGLISH - VIETNAMESE" << endl;
				print_HashTable(hashtable);
				cout << endl;
			}
			goto here;
		case 7:
			system("cls");
			goto after7;
		case 8:
			exit(0);
			return -1;
		default:
			cout << "Invalid choose! Type 1-8." << endl;
			goto here;
		}
	return -1;
}