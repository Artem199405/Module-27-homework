#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "Messages.h"
#include "Sha1.h"

using namespace std;

class Participants
{
public:
	/**
	 Constructor
	*/
	Participants();

	/**
	 Destructor
	*/
	~Participants();

	/**
	 * Recording a new message
	 *
     * @param name is the name of the sender
	 * @param text
	*/
	void recordMessage(string name, string text);

	/**
	 Number of messages sent
	*/
	int showMessageCount();

	/**
	 * Reading sent messages (sender)
	 *
	 * @param i is the client socket
	*/
	string showMessageSender(int i);

	/**
	 * Reading sent messages (text)
	 *
	 * @param i is the client socket
	*/
	string showMessageText(int i);

	/**
	 Clear sent messages
	*/
	void clearMessage();

	// Setter
	void setId(int id);
	
	void setLogin(string login);

	void setPassword(string password);

	void setName(string name);

	void setPasswordHash(string password);

	// Getter
	int getId() const noexcept;
	
	string getLogin() const noexcept;

	string getPassword() const noexcept;

	string getName() const noexcept;

	uint getPasswordHash() const noexcept;

private:
	int _id;
	
	string _login, _password, _name;

	uint* _password_hash;

	vector<Messages> _messages;
};