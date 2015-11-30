#pragma once
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "User.h"
#include "ConsoleGraphics.h"
#include "Locker.h"

//Отображает список пользователей
class UsersList
{
public:
	//Создаем объект
	UsersList(HANDLE consoleHandle, RECT rect, int attributes);

	//Печатаем список пользователей
	void DrawUsers(std::vector<User> users);

private:
	HANDLE consoleHandle;		//Хэндл консоли
	RECT rect;					//Размер и положение области 
	int attributes;				//Аттрибуты текста


	char online_to_char(bool is_online);

};