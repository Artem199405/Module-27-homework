#include "Messages.h"

Messages::Messages()
{
}

Messages::~Messages()
{
}

void Messages::setSender(string sender)
{
	_sender = sender;
}

void Messages::setText(string text)
{
	_text = text;
}

string Messages::getSender()
{
	return _sender;
}

string Messages::getText()
{
	return _text;
}