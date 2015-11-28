#pragma once

/*
Представляет пользователя
*/

#include <vector>
#include <string>
#include <Windows.h>

class User
{
public:
	int Id;								//Идентификатор
	std::string Name;					//Имя
	std::string Password;				//Пароль
	bool IsOnline;						//Подключен ли к серверу

	char pipe_name[255];				//Имя канала для широковещательного ответа

	std::vector<int> Chats;				//Идентификаторы чатов
};