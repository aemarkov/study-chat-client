
#include <QtCore/QCoreApplication>
#include <iostream>
#include "ChatClient.h"

using namespace std;

ChatClinet client;

//Интерфейс
int main_menu();
bool login();
bool sign_up();

void chat_help();
void chat();

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	cout << "-------------------------------------------------\n";
	cout << "|         Консольный чат, v0.1                  |\n";
	cout << "-------------------------------------------------\n";
	
	//Подключаемся к серверу
	if (!client.ConnectToServer(L"\\\\.\\pipe\\chat_server_pipe"))
	{
		cout << "Не удалось подключиться к серверу";
		return 0;
	}

	//Показ главного меню
	int answer = main_menu();
	if (answer == 1)
	{
		if (!sign_up())return 0;
	}
	else if (answer == 2)
	{
		if (!login())return 0;
	}

	chat_help();
	chat();

	return 0;
}

//Показывает главное меню
int main_menu()
{
	cout << "          ГЛАВНОЕ МЕНЮ\n";
	cout << "    1. Регистрация на сервере\n";
	cout << "    2. Вход на сервер\n";
	cout << "> ";
	int answer;
	cin >> answer;
	return answer;
}

//Вход на сервер
bool login()
{
	bool repeat = false;
	cout << "             ВХОД\n";
	do
	{
		repeat = false;
		std::string name, password;
		cout << "Имя:    ";
		cin >> name;
		cout << "Пароль: ";
		cin >> password;
		if (client.Login(QString(name.c_str()), QString(password.c_str())))
		{
			cout << "Успешный вход\n";
			return true;
		}
		else
		{
			char answer;
			cout << "Ошибка. Попробовать снова? (y\\n) ";
			cin >> answer;
			if (answer == 'y')
				repeat = true;
			else return false;
		}
	} while (repeat);
}

//Регистрация
bool sign_up()
{
	bool repeat = false;
	cout << "          РЕГИСТРАЦИЯ\n";
	do
	{
		repeat = false;
		std::string name, password;
		
		cout << "Имя:    ";
		cin >> name;
		cout << "Пароль: ";
		cin >> password;
		if (client.Register(QString(name.c_str()), QString(password.c_str())))
		{
			cout << "Успешная регистрация\n";
			return true;
		}
		else
		{
			char answer;
			cout << "Ошибка. Попробовать снова? (y\\n) ";
			cin >> answer;
			if (answer == 'y')
				repeat = true;
			else return false;
		}
		

	} while (repeat);
}

//Меню чата
void chat_help()
{
	cout << "         СПРАВКА\n";
	cout << "\\exit     выход\n";
	cout << "\\users    список пользователей\n";
	cout << "\\help     просмотр справки\n";
}

//Цикл чата
void chat()
{
	string answer;

	while (true)
	{
		//Ввод сообщеняи\команды
		cout << "> ";
		cin >> answer;

		//Обработка команды
		if (answer == "\\exit")return;
		else if (answer == "\\users")return;
		else if (answer == "\\help")
		{
			chat_help();
			continue;
		}

		//Отправка сообщения

	}
}