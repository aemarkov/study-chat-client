#include "ChatClient.h"

ChatClinet::ChatClinet(void(*incoming_message_callback)(std::string), void(*users_updated_callback)(std::vector<User>))
{
	this->incoming_message_callback = incoming_message_callback;
	this->users_updated_callback = users_updated_callback;
}

//Подключение к серверу
bool ChatClinet::ConnectToServer(const TCHAR * pipe_name)
{
	//Подключаемся к каналу
	pipe = CreateFile(pipe_name, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (pipe == INVALID_HANDLE_VALUE)
		return false;

	DWORD mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
	SetNamedPipeHandleState(pipe, &mode, NULL, NULL);
	return true;
}

//Вход
bool ChatClinet::Login(std::string name, std::string password)
{
	//Отправляем данные
	char* buffer;
	User user;
	user.Name = name;
	user.Password = password;
	int size = PacketCoderDecoder::CodeRequestUserConnect(user, buffer);
	if (!send(buffer, size, pipe))return false;
	delete[] buffer;

	//Ждем ответа

	if (receive(buffer, pipe)==-1)return false;
	PacketTypes type = get_type(buffer);

	if (type == DATA_USER)
	{
		PacketCoderDecoder::DecodeDataUser(me, buffer);
		delete[] buffer;
		return create_pipe();
	}
	delete[] buffer;

	return false;
}

//Регистрация
bool ChatClinet::Register(std::string name, std::string password)
{
	//Отправляем данные
	char* buffer;
	User user;
	user.Name = name;
	user.Password = password;
	int size = PacketCoderDecoder::CodeRequestUserRegister(user, buffer);
	if (!send(buffer, size, pipe))return false;
	delete[] buffer;

	//Ждем ответа
	if (receive(buffer, pipe)==-1)return false;
	PacketTypes type = get_type(buffer);
	

	if (type == DATA_USER)
	{
		PacketCoderDecoder::DecodeDataUser(me, buffer);
		delete[] buffer;
		return create_pipe();
	}

	delete[] buffer;
	return false;
}

//Отправляет сообщение
bool ChatClinet::SendChatMessage(std::string message)
{
	char* buffer;
	Message msg;
	msg.Text = message;
	msg.Author = me;

	//Отправляем данные
	int size = PacketCoderDecoder::CodeDataMessage(msg, buffer);
	if (!send(buffer, size, pipe))return false;
	delete[] buffer;

	return true;
}

//Загружает чат - предыдущую историю сообщений
bool ChatClinet::LoadChat()
{
	//Отправляем запрос
	int size;
	char* buffer;
	size = PacketCoderDecoder::CodeRequestLoadChat(buffer);
	if (!send(buffer, size, pipe))return false;
	delete[] buffer;

	//Ждем ответа
	if (receive(buffer, pipe)==-1)return false;
	PacketTypes type = get_type(buffer);
	if (type == DATA_CHAT)
	{
		Chat chat;
		PacketCoderDecoder::DecodeDataChat(chat, buffer);
		for (auto i = chat.Messages.begin(); i != chat.Messages.end(); ++i)
			write_message(*i);
		
		return true;
	}

	return false;
}

//Получает список пользователей
bool ChatClinet::GetUsers(std::vector<User>& users)
{
	//Отправляем данные
	char* buffer;
	int size = PacketCoderDecoder::CodeRequestUsersList(buffer);
	if (!send(buffer, size, pipe))return false;
	delete[] buffer;

	//Ждем ответа
	if (receive(buffer, pipe)==-1)return false;
	PacketTypes type = get_type(buffer);
	if (type == DATA_USERS_LIST)
		PacketCoderDecoder::DecodeDataUsersList(users, buffer);
	delete[] buffer;

	return  type == DATA_USERS_LIST;
}

//Отправляет  сигнал, что клиент откоючен
void ChatClinet::Disconnect()
{
	char* buffer;
	int size = PacketCoderDecoder::CodeRequestClose(me.Id, buffer);
	send(buffer, size, pipe);
}

ChatClinet::~ChatClinet()
{
	CloseHandle(pipe);
}

//Отправляет размер данных и данные
bool ChatClinet::send(char * buffer, int size, HANDLE pipe)
{
	DWORD bw;
	WriteFile(pipe, &size, sizeof(size), &bw, NULL);
	if (bw != sizeof(size))
	{
		delete[] buffer;
		return false;
	}

	WriteFile(pipe, buffer, size, &bw, NULL);
	if (bw != size)
	{
		delete[] buffer;
		return false;
	}
	return true;
}

int ChatClinet::receive(char*& buffer, HANDLE pipe)
{
	int size;
	DWORD br;

	//Читаем размер буфера
	ReadFile(pipe, &size, sizeof(int), &br, NULL);
	if (br != sizeof(int))return -1;
	buffer = new char[size];

	//Читаем данные
	ReadFile(pipe, buffer, size, &br, NULL);
	if (br != size)
	{
		delete[] buffer;
		return -1;
	}

	return size;
}

PacketTypes ChatClinet::get_type(char * buffer)
{
	PacketTypes type;
	memcpy(&type, buffer, sizeof(PacketTypes));
	return type;
}

//Создает канал и поток для обратной асинхронной связи
bool ChatClinet::create_pipe()
{
	char pipe_name[255];
	sprintf(pipe_name, "\\\\.\\pipe\\%d", me.Id);

	my_pipe = CreateNamedPipeA(pipe_name,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
		1024, 1024, 10000, NULL);
	if (my_pipe == INVALID_HANDLE_VALUE)
		return false;

	CreateThread(NULL, 0, thread_func_wrapper, (LPVOID)this, NULL, NULL);

	return true;
}

//Обертка над функцией потока
DWORD ChatClinet::thread_func_wrapper(void * param)
{
	ChatClinet* context = static_cast<ChatClinet*>(param);
	context->thread_func();
	return 0;
}

//Функция потока
void ChatClinet::thread_func()
{
	char* buffer;
	Message message;

	while (true)
	{
		//Ожидаем подключение к каналу и чтения
		ConnectNamedPipe(my_pipe, NULL);
		int size = receive(buffer, my_pipe);

		//Определяем тип сообщения
		PacketTypes type;
		memcpy(&type, buffer, sizeof(PacketTypes));

		if (type == PacketTypes::DATA_MESSAGE)
			decode_message(buffer);
		else if (type == PacketTypes::DATA_USERS_LIST)
			write_users(buffer);

		DisconnectNamedPipe(my_pipe);

	}
}

void ChatClinet::decode_message(const char * buffer)
{
	//Принимаем сообщение
	Message message;
	PacketCoderDecoder::DecodeDataMessage(message, buffer);
	if (message.Author.Id != me.Id)
		write_message(message);
}

//Пишет сообщение
void ChatClinet::write_message(Message message)
{
		incoming_message_callback(message.Author.Name + ":\n" + message.Text);
}

//печатает пользователей
void ChatClinet::write_users(char* buffer)
{
	std::vector<User> users;
	PacketCoderDecoder::DecodeDataUsersList(users, buffer);
	users_updated_callback(users);
}