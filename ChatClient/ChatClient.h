#pragma once

/*
Отвечает за обмен данными с сервером
*/

#include <Windows.h>
#include "Packets.h"
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>

class ChatClinet
{

public:

	ChatClinet(void(*incoming_message_callback)(std::string), void (*users_updated_callback)(std::vector<User>));
	bool ConnectToServer(const TCHAR* pipe_name);
	bool Login(std::string name, std::string password);
	bool Register(std::string name, std::string password);
	bool SendChatMessage(std::string message);
	bool LoadChat();
	bool GetUsers(std::vector<User>& users);
	void Disconnect();

	~ChatClinet();

private:
	HANDLE pipe;
	HANDLE my_pipe;
	User me;

	void(*incoming_message_callback)(std::string);
	void(*users_updated_callback)(std::vector<User>);

	bool send(char* buffer, int size, HANDLE pipe);		//Отправляет данные и размер данных
	int receive(char*& buffer, HANDLE pipe);			//Принимает данные
	PacketTypes get_type(char* buffer);					//Определяет тип сообщения
	bool create_pipe();									//Создает канал и поток для обратной асинхронной связи
	static DWORD WINAPI thread_func_wrapper(void*);		//Обертка
	void thread_func();									//Поток слушания

	void decode_message(const char* buffer);			//Декодиирует сообщение
	void write_message(Message message);				//Печатает сообщение
	void write_users(char* buffer);						//Декодирует и печатает список пользователей
};