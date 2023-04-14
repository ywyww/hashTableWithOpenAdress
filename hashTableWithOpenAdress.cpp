#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

struct PassportData {
	string name = "GENA";
	string dateOfBirth = "00.00.0000";
	string passportNumber = "6135383468";
	string isDeleted = "false";

	PassportData* ptrNext = nullptr;
};

string arrName[6] = { "Геннадий", "Павел", "Анатолий", "Василий", "Иоанн", "Прокофий" };
string arrSurname[6] = { "Широкий", "Угрюмый", "Тощий", "Хмурый", "Темный", "Праздный" };
string arrPatronymic[6] = { "Олегович", "Святославович", "Игоревич", "Алексеевич", "Владимирович", "Павлович" };
string arrPassportNumber[6] = { "5993256849", "3523650795", "5523246273", "3523562446", "4126337536", "8463465431" };

string generateStructName();
string generateStructDateOfBirth();
string generateStructPassportNumber();

PassportData* getHeadPointer(int);
void fillSingleNode(PassportData&);
void printList(PassportData*);
void printListNode(const PassportData&);

int getHash(string, int);	// принимает поле dateOfBirth и quantity чисел в массиве 
int sumStringNumbers(string);

PassportData** createTable(PassportData*, int);
PassportData** createEmptyTable(int);
void insertTableEmptyPlace(PassportData**, PassportData*, int);
void pushBackChain(PassportData*, PassportData*);
void dataCopy(PassportData*, PassportData);

void printTable(PassportData**, int);
void printChain(PassportData*);
void deleteTable(PassportData**, int);
void deleteChain(PassportData*);

PassportData* searchTable(string, PassportData**, int);
void setDeletedNodeByKey(PassportData*, string);
void setDeletedNodeByNumber(PassportData*, int);

bool checkIfHashInMassive(int* massiveCollisions, int quantity, int hash);
int collisionCount(PassportData* headHumanList, int numberOfTableKeys);

void pushBackChainNode(PassportData*, int&);
void setDeletedNodeByKey(string key, PassportData*);
void setDeletedNodeByNumber(int, PassportData*);

void writeToFile(PassportData*, string);
void writeSingleNode(PassportData*, ofstream&);
PassportData* readFromFile(int&, string);
void readSingleNode(PassportData*, ifstream&);

int countForDeletedNodes(PassportData*);
PassportData* deleteTaggedNodes(PassportData*, int&);

void correctNodeByKey(PassportData*, string);
void correctNodeByNumber(PassportData*, int);
PassportData cinNode();

enum Commands {
	c_createTable = 1,
	c_printTable,
	c_printRecords,
	c_searchByDateOfBirth,
	c_countCollision,
	c_pushBackNode, //6
	c_writeToFile,
	c_readFromFile,
	c_correctNodekey,
	c_correctNodePosition,
	c_setDeletedByKey,
	c_setDeletedByPosition, //12 
	c_deleteTagged,
	c_deleteTable,
	c_restoreDeletedNode
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));

	int numberOfTableKeys = 0;

	PassportData* headHumanList = nullptr;
	PassportData** hashTable = nullptr;

	string key;		// in switch 
	int position;	// in switch

	string backupFilename = "backup.txt";

	bool flagIfArrRecords = false;
	bool isListExist = false;

	int c;
	while (true) {
		system("cls");
		cout << "1. Создать хеш-таблицу с рехешированием методом открытой адресации" << endl;
		cout << "2. Вывести таблицу" << endl;
		cout << "3. Вывести лист записей" << endl;
		cout << "-----------------------------" << endl;
		cout << "4. Поиск человека с заданной датой рождения" << endl;
		cout << "5. Подсчет количества коллизий при заданном размере" << endl;
		cout << "-----------------------------" << endl;
		cout << "6. Добавление элемента в хеш-таблицу" << endl;
		cout << "-----------------------------" << endl;
		cout << "7. Записать список людей в файл" << endl;
		cout << "8. Получить список людей из файла" << endl;
		cout << "-----------------------------" << endl;
		cout << "9. Корректировка по ключу" << endl;
		cout << "10. Корректировка по номеру" << endl;
		cout << "-----------------------------" << endl;
		cout << "11. Удаление элемента по ключу" << endl;
		cout << "12. Удаление элемента по номеру" << endl;
		cout << "13. Удалить помеченные ячейки" << endl;
		cout << "14. Удаление хеш-таблицы" << endl;
		cout << "-----------------------------" << endl;
		cout << "15. Восстановить последнее удаление" << endl;

		cout << "Введите c: "; cin >> c;


		// блок выполняется несколько раз

		if (c == Commands::c_createTable) {
			cout << "Введите число элементов" << endl; cin.get();
			while (numberOfTableKeys <= 0) cin >> numberOfTableKeys;
			if (flagIfArrRecords) {
				deleteChain(headHumanList);
				flagIfArrRecords = true;
			}
			headHumanList = getHeadPointer(numberOfTableKeys);
			isListExist = true;
		}
		else if (c != Commands::c_restoreDeletedNode && c != Commands::c_readFromFile && c != Commands::c_createTable && isListExist == false) {
			cout << "try again" << endl;
			c = 100;
		}
		else if (c == Commands::c_setDeletedByKey || c == Commands::c_searchByDateOfBirth || c == Commands::c_correctNodekey) {
			cout << "Enter the key" << endl; cin.get();
			getline(cin, key);
		}
		else if (c == Commands::c_setDeletedByPosition || c == Commands::c_correctNodePosition) {
			cout << "Enter the position" << endl;
			cin >> position;
		}

		switch (c) {
		case Commands::c_createTable:
		{
			hashTable = createTable(headHumanList, numberOfTableKeys);
			cout << "Your hash table:" << endl; cin.get();
			printTable(hashTable, numberOfTableKeys);
			isListExist = true;
			break;
		}
		case Commands::c_printTable:
		{
			cout << "Your hash table:" << endl; cin.get();
			printTable(hashTable, numberOfTableKeys);
			break;
		}
		case Commands::c_printRecords:
		{
			cout << "Your list:" << endl; cin.get();
			printList(headHumanList);
			break;
		}
		case Commands::c_searchByDateOfBirth:
		{
			PassportData* pd = nullptr;
			pd = searchTable(key, hashTable, numberOfTableKeys);

			if (pd == nullptr) {
				cout << "Такого гражданина нет" << endl;
			}
			else {
				printListNode(*pd);
			}
			break;
		}
		case Commands::c_countCollision:
		{
			cout << "num of collision is: ";
			cout << collisionCount(headHumanList, numberOfTableKeys);
			cout << endl;
			break;
		}
		case Commands::c_deleteTable:
		{
			cout << "Sure? Y: ";
			string answer;
			cin >> answer;
			if (answer == "Y" or answer == "y") {
				deleteChain(headHumanList);
				deleteTable(hashTable, numberOfTableKeys);
				headHumanList = nullptr;
				hashTable = nullptr;
				flagIfArrRecords = false;
				numberOfTableKeys = 0;
				isListExist = false;
				cout << "deleted" << endl;
			}
			break;
		}
		case Commands::c_pushBackNode:
		{
			deleteTable(hashTable, numberOfTableKeys);
			pushBackChainNode(headHumanList, numberOfTableKeys);
			hashTable = createTable(headHumanList, numberOfTableKeys);
			break;
		}
		case Commands::c_setDeletedByKey:
		{
			deleteTable(hashTable, numberOfTableKeys);
			writeToFile(headHumanList, backupFilename);
			setDeletedNodeByKey(headHumanList, key);
			hashTable = createTable(headHumanList, numberOfTableKeys);
			break;
		}
		case Commands::c_writeToFile:
		{
			writeToFile(headHumanList, "myFile.txt");
			break;
		}
		case Commands::c_readFromFile:
		{
			if (isListExist) {
				deleteChain(headHumanList);
				deleteTable(hashTable, numberOfTableKeys);
				numberOfTableKeys = 0;
			}
			headHumanList = readFromFile(numberOfTableKeys, "myFile.txt");
			hashTable = createTable(headHumanList, numberOfTableKeys);
			isListExist = true;
			break;
		}
		case Commands::c_deleteTagged:
		{
			if (headHumanList != nullptr) {
				deleteTable(hashTable, numberOfTableKeys);
				headHumanList = deleteTaggedNodes(headHumanList, numberOfTableKeys);
				hashTable = createTable(headHumanList, numberOfTableKeys);
			}
			else {
				cout << "source list is empty" << endl;
			}
			break;
		}
		case Commands::c_correctNodekey: {
			correctNodeByKey(headHumanList, key);
			deleteTable(hashTable, numberOfTableKeys);
			hashTable = createTable(headHumanList, numberOfTableKeys);
			break;
		}
		case Commands::c_correctNodePosition: {
			correctNodeByNumber(headHumanList, position);
			deleteTable(hashTable, numberOfTableKeys);
			hashTable = createTable(headHumanList, numberOfTableKeys);
			break;
		}
		case Commands::c_setDeletedByPosition: {
			writeToFile(headHumanList, backupFilename);
			deleteTable(hashTable, numberOfTableKeys);
			setDeletedNodeByNumber(headHumanList, position);
			hashTable = createTable(headHumanList, numberOfTableKeys);
			break;
		}
		case Commands::c_restoreDeletedNode: {
			if (isListExist) {
				deleteChain(headHumanList);
				deleteTable(hashTable, numberOfTableKeys);
				numberOfTableKeys = 0;
			}
			headHumanList = readFromFile(numberOfTableKeys, backupFilename);
			hashTable = createTable(headHumanList, numberOfTableKeys);
			isListExist = true;
			break;
		}
		default:
		{
			//cout << numberOfTableKeys;
		}

		}//endswitch
		system("pause");

		if (headHumanList != nullptr && countForDeletedNodes(headHumanList) >= numberOfTableKeys / 2) {
			deleteTable(hashTable, numberOfTableKeys);
			headHumanList = deleteTaggedNodes(headHumanList, numberOfTableKeys);
			if (numberOfTableKeys == 0) {
				isListExist = false;
				hashTable = nullptr;
				headHumanList = nullptr;
			}
			else {
				hashTable = createTable(headHumanList, numberOfTableKeys);
				isListExist = true;
			}
		}

	}//endwhile
	return 0;
}

string generateStructName() {
	string name = arrName[rand() % 6];
	string surname = arrSurname[rand() % 6];
	string patronymic = arrPatronymic[rand() % 6];
	return surname + " " + name + " " + patronymic;
}
string generateStructDateOfBirth() {
	int date = rand() % 29;
	int month = rand() % 12 + 1;
	int year = 1980 + rand() % 36;
	return to_string(date) + "." + to_string(month) + "." + to_string(year);
}
string generateStructPassportNumber() {
	return arrPassportNumber[rand() % 6];
}

/// <summary>
/// Создать список людей и возвратить head листа
/// </summary>
/// <param name="numberOfPeople"></param>
/// <returns></returns>
PassportData* getHeadPointer(int numberOfPeople) {
	PassportData* head = nullptr;
	for (int i = numberOfPeople; i > 0; i--) {
		PassportData* node = new PassportData;
		fillSingleNode(*node);
		node->ptrNext = head;
		head = node;
	}
	return head;
}
void fillSingleNode(PassportData& pd) {
	pd.name = generateStructName();
	pd.dateOfBirth = generateStructDateOfBirth();
	pd.passportNumber = generateStructPassportNumber();
}
void printList(PassportData* headHumanList) {
	PassportData* nodePtr = headHumanList;
	while (nodePtr != nullptr) {
		printListNode(*nodePtr);
		nodePtr = nodePtr->ptrNext;
	}
}
void printListNode(const PassportData& pd) {
	cout << "--------------------------" << endl;
	cout << "name: " << pd.name << endl;
	cout << "date birth: " << pd.dateOfBirth << endl;
	cout << "passport number: " << pd.passportNumber << endl;
	cout << "is deleted: " << pd.isDeleted << endl;
	cout << "--------------------------" << endl;
}

int getHash(string dateOfBirth, int numberOfTableKeys) {
	int sumFormat = sumStringNumbers(dateOfBirth);
	double randCoefficient = 0.24728;
	double fractionalPart = sumFormat * randCoefficient - (int)(sumFormat * randCoefficient);
	return (int)(numberOfTableKeys * fractionalPart);
}
int sumStringNumbers(string dateOfBirth) {
	int sumFormat = 0;
	for (int i = 0; i < dateOfBirth.length(); i++) {
		char symbol = dateOfBirth[i];
		if (symbol != '.') {
			sumFormat += symbol - '0';
		}
	}
	return sumFormat;
}

PassportData** createTable(PassportData* headHumanList, int numberOfTableKeys) {
	PassportData** hashTable = createEmptyTable(numberOfTableKeys);
	for (PassportData* nodePtr = headHumanList; nodePtr != nullptr; nodePtr = nodePtr->ptrNext) {
		int hash = getHash(nodePtr->dateOfBirth, numberOfTableKeys);
		PassportData* dataCell = new PassportData;

		dataCopy(dataCell, *nodePtr);

		if (hashTable[hash] == nullptr) {
			hashTable[hash] = dataCell;
		}
		else {
			PassportData* firstChainNode = hashTable[hash];
			insertTableEmptyPlace(hashTable, dataCell, numberOfTableKeys);
		}
	}
	return hashTable;
}
PassportData** createEmptyTable(int numberOfTableKeys) {
	PassportData** hashTable = new PassportData * [numberOfTableKeys];
	for (int i = 0; i < numberOfTableKeys; i++) {
		hashTable[i] = nullptr;
	}
	return hashTable;
}
void insertTableEmptyPlace(PassportData** hashTable, PassportData* insertionCell, int numberOfTableKeys) {
	int hash = getHash(insertionCell->dateOfBirth, numberOfTableKeys);
	int counter = 0;
	while (hashTable[hash] != nullptr) {
		hash++;
		counter++;
		if (hash == numberOfTableKeys) hash = 0;
		if (counter == numberOfTableKeys) {
			cout << "Error in insertTableEmptyPlace" << endl;
			return;
		}
	}
	hashTable[hash] = insertionCell;
}
void pushBackChain(PassportData* firstChainNode, PassportData* insertionNode) {
	PassportData* nodePtr = firstChainNode;
	while (nodePtr->ptrNext != nullptr) {
		nodePtr = nodePtr->ptrNext;
	}
	nodePtr->ptrNext = insertionNode;
}
void dataCopy(PassportData* to, PassportData from) {
	to->name = from.name;
	to->dateOfBirth = from.dateOfBirth;
	to->passportNumber = from.passportNumber;
	to->isDeleted = from.isDeleted;
	to->ptrNext = nullptr;
}

void printTable(PassportData** hashTable, int numberOfTableKeys) {
	for (int i = 0; i < numberOfTableKeys; i++) {
		if (hashTable[i] == nullptr) {
			cout << "null" << endl;
		}
		else {
			PassportData* headChain = hashTable[i];
			printChain(headChain);
		}
	}
}
void printChain(PassportData* headChain) {
	PassportData* chainNode = headChain;

	while (chainNode != nullptr) {
		if (chainNode->isDeleted == "false") cout << chainNode->name << " -> ";
		chainNode = chainNode->ptrNext;
	}
	cout << endl;
}

void deleteTable(PassportData** hashTable, int numberOfTableKeys) {
	for (int i = 0; i < numberOfTableKeys; i++) {
		deleteChain(hashTable[i]);
	}
	delete[] hashTable;
}

void deleteChain(PassportData* hashElem) {
	PassportData* tmpPtr = hashElem;
	while (tmpPtr != nullptr) {
		PassportData* secondTmpPtr = tmpPtr;
		tmpPtr = tmpPtr->ptrNext;
		delete secondTmpPtr;
	}
}

PassportData* searchTable(string key, PassportData** hashTable, int numberOfTableKeys) {
	int hash = getHash(key, numberOfTableKeys);

	int counter = 0;
	bool isFoundKey = false;
	while (!isFoundKey) {
		isFoundKey = (hashTable[hash]->dateOfBirth == key);
		hash++;
		counter++;
		if (counter == numberOfTableKeys) return nullptr;
		if (hash == numberOfTableKeys) hash = 0;
	}
	return hashTable[--hash];
}
PassportData* searchChainByKey(PassportData* chainHead, string key) {
	PassportData* chainNode = chainHead;
	while (chainNode != nullptr) {
		if (chainNode->dateOfBirth == key && chainNode->isDeleted == "false") {
			return chainNode;
		}
		chainNode = chainNode->ptrNext;
	}
	return nullptr;
}
PassportData* searchChainByNumber(PassportData* headChain, int number) {
	PassportData* ptrNode = headChain;
	for (int i = 1; i < number && ptrNode != nullptr; i++) {
		ptrNode = ptrNode->ptrNext;
	}
	return ptrNode;
}

int collisionCount(PassportData* headHumanList, int numberOfTableKeys) {
	int* massiveCollisions = new int[numberOfTableKeys];
	for (int i = 0; i < numberOfTableKeys; i++) massiveCollisions[i] = -1;

	PassportData* chainNode = headHumanList;

	int collisionCount = 0;
	int indexCounter = 0;
	for (PassportData* chainNode = headHumanList; chainNode != nullptr; chainNode = chainNode->ptrNext) {

		int hash = getHash(chainNode->dateOfBirth, numberOfTableKeys);

		if (checkIfHashInMassive(massiveCollisions, numberOfTableKeys, hash)) {
			collisionCount++;
		}
		else {
			massiveCollisions[indexCounter] = hash;
			indexCounter++;
		}

	}
	return collisionCount;
}
bool checkIfHashInMassive(int* massiveCollisions, int quantity, int hash) {
	for (int i = 0; i < quantity; i++) {
		if (massiveCollisions[i] == hash) return 1;
		if (massiveCollisions[i] == -1) return 0;
	}
	return 0;
}

void pushBackChainNode(PassportData* headHumanChain, int& numberOfTableKeys) {
	PassportData* insertionCell = new PassportData;
	fillSingleNode(*insertionCell);
	pushBackChain(headHumanChain, insertionCell);
	numberOfTableKeys++;
}
void setDeletedNodeByKey(PassportData* headHumanChain, string key) {
	PassportData* taggedNode = searchChainByKey(headHumanChain, key);
	if (taggedNode != nullptr) taggedNode->isDeleted = "true";
	else {
		cout << "failed to setDeletedNodeByKey() function" << endl;
	}
}
void setDeletedNodeByNumber(PassportData* headHumanChain, int position) {
	PassportData* taggedNode = searchChainByNumber(headHumanChain, position);
	if (taggedNode != nullptr) taggedNode->isDeleted = "true";
	else {
		cout << "failed to setDeletedNodeByNumber() function" << endl;
	}
}

void writeToFile(PassportData* humanList, string source) {
	std::ofstream out;
	out.open(source);

	PassportData* chainNode = humanList;

	while (chainNode != nullptr) {
		writeSingleNode(chainNode, out);
		chainNode = chainNode->ptrNext;
	}
	out.close();
}
void writeSingleNode(PassportData* chainNode, ofstream& out) {
	out << chainNode->name << '\n';
	out << chainNode->dateOfBirth << '\n';
	out << chainNode->passportNumber << '\n';
	out << chainNode->isDeleted << '\n';
}
PassportData* readFromFile(int& numberOfTableKeys, string source) {
	ifstream in;
	in.open(source);

	PassportData* chainNode = new PassportData;
	readSingleNode(chainNode, in);

	numberOfTableKeys = 0;

	PassportData* head = chainNode;
	PassportData* last = chainNode;

	if (!in.eof()) {
		while (!in.eof()) {
			last = chainNode;
			chainNode->ptrNext = new PassportData;
			chainNode = chainNode->ptrNext;

			readSingleNode(chainNode, in);

			numberOfTableKeys++;
		}
		delete last->ptrNext; last->ptrNext = NULL;
	}
	else {
		delete chainNode; head = NULL;
	}
	in.close();

	return head;
}
void readSingleNode(PassportData* chainNode, ifstream& in) {
	getline(in, chainNode->name);
	getline(in, chainNode->dateOfBirth);
	getline(in, chainNode->passportNumber);
	getline(in, chainNode->isDeleted);
}

int countForDeletedNodes(PassportData* humanList) {
	PassportData* chainNode = humanList;
	int counter = 0;
	while (chainNode != nullptr) {
		if (chainNode->isDeleted == "true") counter++;
		chainNode = chainNode->ptrNext;
	}
	return counter;
}
PassportData* deleteTaggedNodes(PassportData* headChain, int& numberOfTheTableKeys) {
	PassportData* newHead = new PassportData;

	dataCopy(newHead, *headChain);
	newHead->ptrNext = nullptr;
	int counterKeys = 1;

	PassportData* chainNode = headChain->ptrNext;


	while (chainNode != nullptr) {

		if (chainNode->isDeleted == "false") {
			PassportData* taggedPoint = new PassportData;
			dataCopy(taggedPoint, *chainNode);
			taggedPoint->ptrNext = nullptr;

			pushBackChain(newHead, taggedPoint);
			counterKeys++;
			cout << taggedPoint->name << endl;
		}
		chainNode = chainNode->ptrNext;
	}


	if (newHead != nullptr && newHead->isDeleted == "true") {
		if (newHead->ptrNext != nullptr) {
			PassportData* taggedPoint = newHead;
			newHead = newHead->ptrNext;
			delete taggedPoint;
		}
		else {
			delete newHead; newHead = nullptr;
		}
		counterKeys--;
	}

	numberOfTheTableKeys = counterKeys;
	deleteChain(headChain);
	return newHead;
}
void correctNodeByKey(PassportData* headChain, string key) {
	PassportData* toCorrect = searchChainByKey(headChain, key);
	if (toCorrect == nullptr) {
		cout << "failed to correct node" << endl;
		return;
	}

	PassportData pd = cinNode();
	pd.dateOfBirth = toCorrect->dateOfBirth;
	dataCopy(toCorrect, pd);
}
PassportData cinNode() {
	PassportData pd;
	cout << "name? "; cin >> pd.name;
	cout << "passport? "; cin >> pd.passportNumber;
	return pd;
}

void correctNodeByNumber(PassportData* headChain, int numberOfTheTableKeys) {
	PassportData* toCorrect = searchChainByNumber(headChain, numberOfTheTableKeys);
	if (toCorrect == nullptr) {
		cout << "failed to correct node" << endl;
		return;
	}

	PassportData pd = cinNode();
	pd.dateOfBirth = toCorrect->dateOfBirth;
	dataCopy(toCorrect, pd);
}