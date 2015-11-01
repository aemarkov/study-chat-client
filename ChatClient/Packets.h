#pragma once

/*
Эти классы представляют контейнеры для передачи запросов
от клиента к серверу и передачи ответа сервера обратно.

Эти класс общий и для клиента, и для сервера

Все они наследуются от одного базового класса и сереализуются одинаково.
Чтобы сервер\клиент могли корректно выделить память, пакет передаются в два 
примема: сначала заголовок с длиной и типом, потом сами данные

|------------------------------| |-------------------------------------|
| Тип запроса | Длина в байтах | | Данные, зависящие от класса-потомка |
|------------------------------| |-------------------------------------|

*/

#include "User.h"
#include "Chat.h"
#include "Message.h"
#include "Serializers.h"

//Типы контейнеров
enum PacketTypes
{
	//Запросы - сообщения, не передающие информацию
	//(формально, создание нового пользователя и чата 
	//передают информацию, но нет - не считается)
	REQUEST_USERS_LIST,			//список пользователей
	REQUEST_LOAD_CHAT,			//загрузка чата
	REQUEST_USER_REGISTER,		//регистрация
	REQUEST_USER_CONNECT,		//подключение
	REQUEST_CLOSE,				//отключение клиента

	DATA_MESSAGE,				//пришло сообщение. Этот же запрос отсылает сервер
								//клиентам, когда в чат пришло сообщение

	//Передача информации                                    //Это может быть ответом
	//чаще от сервера клиентам                               //на эти запросы
	//но не обязательно
	DATA_USERS_LIST,			//список пользователей       REQUSET_USERS_LIST
	DATA_CHAT,					//содержимое чата            REQUEST_LOAD_CHAT, REQUEST_CREATE_CHAT
	DATA_USER,					//инфа о юзере               REQUEST_USER_REGISTER, REQUEST_USER_CONNECT
	DATA_ERROR					//Ошибка
	
};

/*
Этот класс представляет ряд методов по кодированию и 
декодированию объектов разного типа.
В отличие от сериализаторов объектов, эти методы
ВЫДЕЛЯЮТ память под буфер сами.
*/
class PacketCoderDecoder
{
public:
	
	//Сериализация
	//для клиента
	static int CodeRequestLoadChat(const int chatId, char*& buffer);
	static int CodeRequestUserRegister(const User& user, char*& buffer);
	static int CodeRequestUserConnect(const User& cc, char*& buffer);
	static int CodeRequestChatCreate(const Chat& chat, char*& buffer);
	static int CodeRequestAddUser(const int userId,const int chatId, char*& buffer);
	static int CodeRequestClose(const int userId, char*& buffer);

	//Для сервера
	static int CodeDataUsersList(const QList<User>& users, char*& buffer);
	static int CodeDataChatsList(const QList<Chat>& chats, char*& buffer);
	static int CodeDataChat(const Chat& chat, char*& buffer);
	static int CodeDataUser(const User& user, char*& buffer);
	static int CodeDataError(char*& buffer);

	//для клиента и сервера
	static int CodeDataMessage(const Message& message, char*& buffer);

	//Десериализация
	//для сервера
	static int DecodeRequestLoadChat(int& chatId, const char* buffer);
	static int DecodeRequestUserRegister(User& user, const char* buffer);
	static int DecodeRequestUserConnect(User& cc, const char* buffer);
	static int DecodeRequestChatCreate(Chat& chat, const char* buffer);
	static int DecodeRequestAddUser(int& userId, int& chatId, const char* buffer);

	//для клиента
	static int DecodeDataUsersList(QList<User>& users, const char* buffer);
	static int DecodeDataChatsList(QList<Chat>& chats, const char* buffer);
	static int DecodeDataChat(Chat& chat, const char* buffer);
	static int DecodeDataUser(User& user, const char* buffer);

	//для клиента и сервера
	static int DecodeDataMessage(Message& message, const char* buffer);

private:
	//Сериализация списка
	template <class T>
	static int code_list(const QList<T>& list, const PacketTypes type, char*& buffer);

	//Десериализация списков
	template <class T>
	static int decode_list(QList<T>& list, const char* buffer);

	//Сериализует просто тип
	template <class T>
	static int serialize(const T& t, const PacketTypes type, char*& buffer);
};