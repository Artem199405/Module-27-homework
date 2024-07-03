#pragma once

#include <string>

using namespace std;

class Messages
{
public:
	/**
	 Constructor
	*/
	Messages();

	/**
	 Destructor
	*/
	~Messages();

	// Setter
	void setSender(string);

	void setText(string);

	// Getter
	string getSender();

	string getText();

private:
	string _sender, _text;
};