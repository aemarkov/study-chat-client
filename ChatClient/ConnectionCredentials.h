#pragma once

/*
Представляет данные пользователя для входа - имя и пароль
*/
#include <string>
class ConnectionCredentials
{
public:
	std::string Name;
	std::string Password;
};
