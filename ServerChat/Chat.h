#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <mysql.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include "Participants.h"
#include "AutoLogin.h"
#include "Logger.h"

using namespace std;

#define port 7777

#pragma warning(disable: 4996)

class Chat
{
public:
	/**
	 Constructor
	*/
	Chat();

	/**
	 Destructor
	*/
	~Chat();

	// Establishing a connection to the database
	void connectToDatabase();

	/**
	 Restoring the chat
	*/
	void restoringChat();

	/**
	 Starting the server
	*/
	void runServer();

private:
	/**
	 * Manager for working with chat participants
	 *
	 * @param i is the client socket
	*/
	void participantHandler(int i);

	/**
	 * Registering an account for a new chat participant
	 *
	 * @param i is the client socket
	*/
	void registration(int i);

	/**
	 * Auto-completion of the entered login
	 *
	 * @param i is the client socket
	 * @param keyword is prefix
	*/
	void findFreeLogin(int i, string keyword);

	/**
	 * Adding a new chat participant
	 *
	 * @param id is the ID in the chat participants database
	 * @param login is the login of the new chat participant
	 * @param password is the password of the new chat participant
	 * @param name is the name of the new chat participant
	*/
	void addParticipant(int id, string login, string password, string name);

	/**
	 * Adding a new chat participant to the database
	 *
	 * @param id is the ID in the chat participants database
	 * @param login is the login of the new chat participant
	 * @param password is the password of the new chat participant
	 * @param name is the name of the new chat participant
	*/
	void addParticipantToDatabase(int id, string login, string password, string name);

	/**
	 * Account authorization
	 *
	 * @param i is the client socket
	*/
	void authorization(int i);

	/**
	 * Log in to your account
	 *
	 * @param login
	 * @param password
	*/
	bool signIn(string login, string password);

	/**
	 * Determining the hash of the password
	 *
	 * @param password
	*/
	uint findHash(string password);

	/**
	 * Entering a message
	 *
	 * @param i is the client socket
	*/
	void enteringMessage(int i);

	/**
	 * Sending a message
	 *
	 * @param nameSender is the name of the sender
	 * @param recipientName is the name of the recipient
	 * @param text
	*/
	void sendMessage(string nameSender, string nameRecipient, string text);

	/**
	 * Reading the message
	 *
	 * @param i is the client socket
	*/
	void readMessages(int i);

	/**
	 * Comparison of existing and entered logins
	 *
	 * @param login
	*/
	bool compareLogin(string login);

	/**
	 * Comparing existing and entered names
	 *
	 * @param name
	*/
	bool compareName(string name);

	/**
	 * Get a name by login
	 *
	 * @param login
	*/
	string getParticipantName(string login);

	/**
	 * Get the index of the user in the _participants array by name
	 *
	 * @param name
	*/
	int getParticipantIndex(string name);

	/**
	 * Get an integer type variable from a chat participant
	 *
	 * @param i is the client socket
	*/
	int getParticipantInt(int i);

	/**
	 * Get a string variable from a chat participant
	 *
	 * @param i is the client socket
	*/
	string getParticipantString(int i);

	/**
	 * Send an int type variable to the chat participant
	 *
	 * @param i is the client socket
	 * @param j is a int variable
	*/
	void sendParticipantInt(int i, int j);

	// Отправить участнику чата переменную типа string
	/**
	 * Send a string variable to the chat participant
	 *
	 * @param i is the client socket
	 * @param word is a string variable
	*/
	void sendParticipantString(int i, string word);
	
	vector<Participants> _participants;

	AutoLogin _loginSource; // Prefix tree for creating new chat logins

	SOCKET _listenSocket; // The socket that listens for client connections
	vector<SOCKET> _participantSocket; // Client sockets

	MYSQL mysql; // Connection descriptor

	Logger log;
};