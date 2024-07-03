#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>

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

	/**
	 Establish a connection with the server
	*/
	void connectServer();

	/**
	 Registering an account for a new chat participant
	*/
	void registration();

	/**
	 Account authorization
	*/
	void authorization();

	/**
	 Entering a message
	*/
	void enteringMessage();

	/**
	 Reading the message
	*/
	void readMessages();

	/**
	 * Send an integer type variable to the server
	 *
	 * @param action
	*/
	void sendServerInt(int action);

private:
	/**
     Get an integer type variable from the server
    */
	int getServerInt();

	/**
	 Get a string variable from the server
	*/
	string getServerString();
	
	/**
	 * Send a string variable to the server
	 *
	 * @param message
	*/
	void sendServerString(string message);

	string nameParticipant; // Name of the authorized chat participant from the _participants array

	SOCKET connectSocket; // Socket for connecting to the server
};