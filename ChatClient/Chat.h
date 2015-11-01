#pragma once

/*
Представляет чат - общение нескольких пользователь
Хранит идентификаторы пользователей, сообщения
*/

#include <qlist.h>
#include <qlinkedlist.h>
#include "Message.h"

class Chat
{
public:
	int Id;								//Идентификатор чата
	QList<int> Users;					//Список идентификаторов пользователе	
	QLinkedList<Message> Messages;		//Список сообщений
};


//Чат без сообщений - чтобы при показе списка
//чатов не грузились сообщения для всех
class ChatOnlyInfo
{
public:
	ChatOnlyInfo(const Chat&);
	int Id;								//Идентификатор чата
	QList<int> Users;					//Список идентификаторов пользователе	
};