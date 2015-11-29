#include <iostream>
#include <iomanip>

#include "ChatClient.h"
#include "User.h"

#include "GUI.h"

using namespace std;

string online_to_string(bool);		//Переводит булевый статус в текст
bool menu();						//Отображает меню
bool login();						//Отображает меню входа
bool registration();				//Отображает меню регистрации
void chat();						//Чат

void print_message(string text);	//Печатает пришедшее сообшение
void update_clients();

ChatClinet client(print_message, update_clients);
GUI gui;

int main(int argc, char *argv[])
{
	gui.DrawInterface();

	//Подключаемся к серверу
	if (!client.ConnectToServer(L"\\\\.\\pipe\\chat_server_pipe"))
	{
		gui.DrawMessage("Can't connect to server");
		return 0;
	}

	//Показ меню
	if (!menu())return 0;

	//Начало чата
	chat();
	client.Disconnect();

	return 0;
}

//Меню
bool menu()
{
	char c;
	do
	{
		c = gui.DrawMenu();
		if (tolower(c) == 'l')
		{
			//Вход
			bool flag = true;
			do
			{
				if (!login())
					flag = gui.DrawMessage("Error, can't login. Try again?");
				else return true;
			} while (flag);

		}
		else if (tolower(c) == 'r')
		{
			//Регистрация		
			bool flag = false;
			do
			{
				if (!registration())
					flag = gui.DrawMessage("Error, can't register. Try again?");
				else return true;
			} while (flag);
		}
		
	} while ((tolower(c)!='l') || (tolower(c)!='r'));


	return false;
}

//Вход
bool login()
{
	auto cc = gui.DrawLogin();
	return client.Login(cc.Name, cc.Password);
}

//Регистрация
bool registration()
{
	auto cc = gui.DrawLogin();
	return client.Register(cc.Name, cc.Password);
}

void chat()
{
	//Загружаем историю
	if (!client.LoadChat())
	{
		gui.DrawMessage("Error while loading chat history");
		return;
	}


	string text;
	bool ctr_mode = false;

	while (text!="\exit")
	{
		char c = getch();

		//Отправка сообщения
		if (c == '\r')
		{
			//Отправляем сообщение
			if (!client.SendChatMessage(gui.inputArea->GetText()))
			{
				gui.DrawMessage("Error while sending message");
				return;
			}

			//Отображаем
			print_message("me:\n" + gui.inputArea->GetText());
			gui.inputArea->Clear();
			continue;
		}

		//Режим доп. кнопок
		if (c == -32)
		{
			ctr_mode = true;
			continue;
		}

		if (ctr_mode)
		{
			//Скролл истории сообщений
			if (c == 80)
			{
				gui.chatArea->ScrollDown();
				continue;
			}
			else if (c == 72)
			{
				gui.chatArea->ScrollUp();
				continue;
			}

			ctr_mode = false;
		}

		gui.inputArea->AddSymbol(c);
	}
}

//Переводит булевый статус в тект
string online_to_string(bool status)
{
	if (status)
		return "online";
	else
		return "offline";
}

//Печатает сообщение
void print_message(string text)
{
	for (int i = 0; i < text.size(); i++)
	{
		gui.chatArea->AddSymbol(text[i]);
	}
}

//Обновляет список пользователей
void update_clients()
{

}