#pragma once

#include <string>

using namespace std;

#define ASCII_SIZE 256

class AutoLogin
{
public:
	struct AutoLoginNode
	{
		//Конструктор
		AutoLoginNode();

		// Деструктор
		~AutoLoginNode();

		AutoLoginNode* children[ASCII_SIZE];

		int childrenCount;

		bool isEndOfWord;
	};

	//Конструктор
	AutoLogin();

	// Деструктор
	~AutoLogin();

	// Добавление нового узла с пустыми детьми
	void insert(string);

	// Поиск слов по префиксу
	void findPrefixWords(string, string*);
	void findPrefixWords(AutoLoginNode*, string, string, string*);

	// Геттеры
	AutoLoginNode* getAutoLoginNode();

private:
	AutoLoginNode root;
};