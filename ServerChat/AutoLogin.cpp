#include "AutoLogin.h"

AutoLogin::AutoLogin()
{
}

AutoLogin::~AutoLogin()
{
}

AutoLogin::AutoLoginNode::AutoLoginNode() : childrenCount(0), isEndOfWord(false)
{
	for (size_t i = 0; i < ASCII_SIZE; i++)
		children[i] = nullptr;
}

AutoLogin::AutoLoginNode::~AutoLoginNode()
{
}

AutoLogin::AutoLoginNode* AutoLogin::getAutoLoginNode()
{
	AutoLoginNode* pNode = new AutoLoginNode;

	return pNode;
}

void AutoLogin::insert(string key)
{
	AutoLoginNode* node = &root;

	for (size_t i = 0; i < key.length(); i++)
	{
		int index = key[i];

		if (!node->children[index])
		{
			node->children[index] = getAutoLoginNode();
			node->childrenCount++;
		}

		node = node->children[index];
	}

	node->isEndOfWord = true;
}

void AutoLogin::findPrefixWords(string keyword, string* res)
{
	AutoLoginNode* current = &root;

	for (size_t i = 0; i < keyword.size(); i++)
	{
		if (current->children[keyword.at(i)])
		{
			current = current->children[keyword.at(i)];
		}
		else
		{
			current = nullptr;
			break;
		}
	}

	if (current == nullptr)
	{
		*res = ' ';
	}
	else
	{
		if (current->isEndOfWord && current->childrenCount == 0)
		{
			*res = keyword;
		}
		else
		{
			findPrefixWords(current, keyword, "", res);
		}
	}
}

void AutoLogin::findPrefixWords(AutoLoginNode* current, string prefix, string suffix, string* res)
{
	if (current->isEndOfWord)
	{
		*res += prefix + suffix + ' ';
	}

	for (size_t i = 0; i < ASCII_SIZE; i++)
	{
		string temp = suffix;
		
		if (current->children[i])
		{
			temp += i;
			findPrefixWords(current->children[i], prefix, temp, res);
		}
	}
}