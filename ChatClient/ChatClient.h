#pragma once

/*
Отвечает за обмен данными с сервером
*/

#include <Windows.h>
#include "Packets.h"
#include <string>
#include <iomanip>
#include <iostream>

class ChatClinet
{
public:

	bool ConnectToServer(const TCHAR* pipe_name);
	bool Login(std::string name, std::string password);
	bool Register(std::string name, std::string password);
	bool SendChatMessage(std::string message);
	bool LoadChat();
	bool GetUsers(QList<User>& users);
	void Disconnect();

	~ChatClinet();

private:
	HANDLE pipe;
	HANDLE my_pipe;
	User me;

	bool send(char* buffer, int size, HANDLE pipe);		//Отправляет данные и размер данных
	int receive(char*& buffer, HANDLE pipe);			//Принимает данные
	PacketTypes get_type(char* buffer);					//Определяет тип сообщения
	bool create_pipe();									//Создает канал и поток для обратной асинхронной связи
	static DWORD WINAPI thread_func_wrapper(void*);		//Обертка
	void thread_func();									//Поток слушания
	void write_message(Message);						//Печатает сообщение
};