#include "Participants.h"

Participants:: Participants()
{
}

Participants::~Participants()
{
}

void Participants::recordMessage(string name, string text)
{
    Messages message;

    message.setSender(name);
    message.setText(text);
    
    _messages.push_back(message);
}

int Participants::showMessageCount()
{
    return _messages.size();
}

string Participants::showMessageSender(int i)
{
    return _messages[i].getSender();
}

string Participants::showMessageText(int i)
{
    return _messages[i].getText();
}

void Participants::clearMessage()
{
    _messages.clear();
}

void Participants::setId(int id)
{
    _id = id;
}

void Participants::setLogin(string login)
{
	_login = login;
}

void Participants::setPassword(string password)
{
	_password = password;

    setPasswordHash(password);
}

void Participants::setName(string name)
{
	_name = name;
}

void Participants::setPasswordHash(string password)
{
    const char* charPassword = password.c_str();
    int length = strlen(charPassword);

    _password_hash = sha1(charPassword, (uint)length);
}

int Participants::getId() const noexcept
{
    return _id;
}

string Participants::getLogin() const noexcept
{
	return _login;
}

string Participants::getPassword() const noexcept
{
	return _password;
}

string Participants::getName() const noexcept
{
	return _name;
}

uint Participants::getPasswordHash() const noexcept
{
    return *_password_hash;
}