#pragma once

//Представляет сообщения в чате

#include <string>
#include <User.h>

class Message
{
public:
	int Id;
	std::string Text;
	User Author;


	~Message()
	{

	}
};