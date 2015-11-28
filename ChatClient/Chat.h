#pragma once

/*
Представляет чат - общение нескольких пользователь
Хранит идентификаторы пользователей, сообщения
*/

#include <list>
#include "Message.h"

class Chat
{
public:
	int Id;								//Идентификатор чата
	std::list<Message> Messages;		//Список сообщений
};
