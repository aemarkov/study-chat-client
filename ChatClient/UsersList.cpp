#include "UsersList.h"

UsersList::UsersList(HANDLE consoleHandle, RECT rect, int attributes)
{
	this->consoleHandle = consoleHandle;
	this->rect = rect;
	this->attributes = attributes;
	
	//ConsoleGraphics::fillRect(rect, RED_WHITE);
}

//Отображает список пользователей
void UsersList::DrawUsers(std::vector<User> users)
{
	COORD c;

	//Положение
	c.X = rect.left;
	c.Y = rect.top;
	SetConsoleCursorPosition(consoleHandle, c);
	SetConsoleTextAttribute(consoleHandle, attributes);

	//Очистка
	ConsoleGraphics::fillRect(rect, attributes);

	/*
	| name ....... [x] |
	|                  |
	|<---- width ----->|
	
	*/
	int iom_width = rect.right - rect.left - 3;

	//Отрисовка пользоватеелй
	for (int i = 0; i < users.size(); i++)
	{
		SetConsoleCursorPosition(consoleHandle, c);
		std::cout << std::left << std::setw(iom_width) << users[i].Name << "["<<online_to_char(users[i].IsOnline)<<"]";
		c.Y++;
	}
}

//Переводит статсус в символ
char UsersList::online_to_char(bool is_online)
{
	return is_online ? 'x' : ' ';
}
