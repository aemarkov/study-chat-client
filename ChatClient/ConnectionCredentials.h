#pragma once

/*
Представляет данные пользователя для входа - имя и пароль
*/
#include <string>
struct ConnectionCredentials
{
public:
	std::string Name;
	std::string Password;

	ConnectionCredentials(std::string name, std::string password)
	{
		Name = name;
		Password = password;
	}
};
