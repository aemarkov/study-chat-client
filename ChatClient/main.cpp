
#include <QtCore/QCoreApplication>
#include <qdebug.h>
#include <iostream>
#include <iomanip>

#include "ChatClient.h"
#include "User.h"

using namespace std;

ChatClinet client;

//Интерфейс
char main_menu();
bool login();
bool sign_up();

void chat_help();
void chat();
void show_users(QList<User> users);
string online_to_string(bool);

QString q_tab(QString str, int width);

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
	char answer = main_menu();
	if (answer == '1')
	{
		if (!sign_up())return 0;
	}
	else if (answer == '2')
	{
		if (!login())return 0;
	}
	else
		return 0;

	chat_help();
	chat();
	client.Disconnect();

	return 0;
}

//Показывает главное меню
char main_menu()
{
	cout << "-------------------------------------------------\n";
	cout << "|              ГЛАВНОЕ МЕНЮ                     |\n";
	cout << "-------------------------------------------------\n";
	cout << "|   1. Регистрация на сервере                   |\n";
	cout << "|   2. Вход на сервер                           |\n";
	cout << "|> ";
	char answer;
	cin >> answer;
	cout << "-------------------------------------------------\n";
	return answer;
}

//Вход на сервер
bool login()
{
	bool repeat = false;
	cout << "-------------------------------------------------\n";
	cout << "|                   ВХОД                        |\n";
	cout << "-------------------------------------------------\n";
	do
	{
		repeat = false;
		std::string name, password;
		cout << "|   Имя:    ";
		cin >> name;
		cout << "|   Пароль: ";
		cin >> password;
		if (client.Login(name, password))
		{
			cout << "|   Успешный вход                               |\n";;
			cout << "-------------------------------------------------\n";
			return true;
		}
		else
		{
			char answer;
			cout << "|   Ошибка. Попробовать снова (y\\n)?           |\n";
			cin >> answer;
			if (answer == 'y')
				repeat = true;
			else
			{
				cout << "-------------------------------------------------\n";
				return false;
			}
		}
	} while (repeat);
}

//Регистрация
bool sign_up()
{
	bool repeat = false;
	cout << "-------------------------------------------------\n";
	cout << "|              РЕГИСТРАЦИЯ                      |\n";
	cout << "-------------------------------------------------\n";
	do
	{
		repeat = false;
		std::string name, password;
		
		cout << "|   Имя:    ";
		cin >> name;
		cout << "|   Пароль: ";
		cin >> password;
		if (client.Register(name,password))
		{
			cout << "|   Успешная регистрация                        |\n";;
			cout << "-------------------------------------------------\n";
			return true;
		}
		else
		{
			char answer;
			cout << "|   Ошибка. Попробовать снова (y\\n)?           |\n";
			cin >> answer;
			if (answer == 'y')
				repeat = true;
			else
			{
				cout << "-------------------------------------------------\n";
				return false;
			}
		}
		

	} while (repeat);
}

//Меню чата
void chat_help()
{
	cout << "-------------------------------------------------\n";
	cout << "|                 СПРАВКА                       |\n";
	cout << "-------------------------------------------------\n";
	cout << "|   \\exit     выход                            |\n";
	cout << "|   \\users    список пользователей             |\n";
	cout << "|   \\help     просмотр справки                 |\n";
	cout << "-------------------------------------------------\n";
}

//Цикл чата
void chat()
{
	//Подгрузка старой истории
	cout << "> ";
	if (!client.LoadChat())
		cout << "Не удалось загрузить чат\n";


	char input[1024];
	cin.get();	//Нужно убрать лишний энтер

	while (true)
	{
		//Ввод сообщеняи\команды
		cin.getline(input, 1024);

		//Обработка команды
		if (strcmp(input,"\\exit")==0)return;
		else if (strcmp(input,"\\users")==0)
		{
			QList<User> users;
			if (client.GetUsers(users))
				show_users(users);
			else
				cout << "Не удалось загрузить список пользователей\n";
			continue;
		}
		else if (strcmp(input,"\\help")==0)
		{
			chat_help();
			continue;
		}

		//Отправка сообщения
		if (!client.SendChatMessage(input))
			cout << "Не удалось отправить сообщение\n";
		cout << "> ";
	}
}

//Выводит всех пользователей
void show_users(QList<User> users)
{
	cout << "\nУчатники чата:\n";
	cout << "|-------------------------|----------|\n";
	cout << "| Имя                     | Статус   |\n";
	cout << "|-------------------------|----------|\n";
	for (int i = 0; i < users.size(); i++)
	{
		cout << "|"<<setw(25)<<std::left<<users[i].Name << "|" <<setw(10) <<std::left<< online_to_string(users[i].IsOnline) << "|\n";
	}

	cout << "|-------------------------|----------|\n";
}

//Переводит булевый статус в тект
string online_to_string(bool status)
{
	if (status)
		return "online";
	else
		return "offline";
}