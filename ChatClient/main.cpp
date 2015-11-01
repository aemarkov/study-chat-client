
#include <QtCore/QCoreApplication>
#include <iostream>
#include "ChatClient.h"

using namespace std;

ChatClinet client;

//Интерфейс
int main_menu();
bool login();
bool sign_up();

int chat_menu();

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
		if (!sign_up())return 0;
	else if (answer == 2)
		if (!login())return 0;

	chat_menu();

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
	std::string name, password;
	cout << "             ВХОД\n";
	cout << "Имя:    ";
	cin >> name;
	cout << "Пароль: ";
	cin >> password;
	client.Login(QString(name.c_str()), QString(password.c_str()));
	return 0;
}

//Регистрация
bool sign_up()
{
	bool repeat = false;
	do
	{
		repeat = false;

		std::string name, password;
		cout << "          РЕГИСТРАЦИЯ\n";
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
			cout << "Ошибка. Попробовать снова? (y\n) ";
			cin >> answer;
			if (answer == 'y')
				repeat = true;
			else return false;
		}
		

	} while (repeat);
}

//Меню чата
int chat_menu()
{
	cout << "             МЕНЮ\n";
	cout << "    1. Просмотр чатов\n";
	cout << "    2. Просмотр пользователей\n";
	cout << "    3. Создать чат\n";
	cout << "    4. Присоединиться к чату\n";
	cout << "> ";
	int answer;
	cin >> answer;
	return answer;
}