#pragma once

/*
Представляет пользователя
*/

#include <qlist.h>
#include <string>

class User
{
public:
	int Id;								//Идентификатор
	std::string Name;						//Имя
	std::string Password;					//Пароль
	bool IsOnline;						//Подключен ли к серверу

	QList<int> Chats;					//Идентификаторы чатов
};