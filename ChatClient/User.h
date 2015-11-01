#pragma once

/*
Представляет пользователя
*/

#include <qlist.h>
#include <qstring.h>

class User
{
public:
	int Id;								//Идентификатор
	QString Name;						//Имя
	QString Password;					//Пароль
	bool IsOnline;						//Подключен ли к серверу

	QList<int> Chats;					//Идентификаторы чатов
};