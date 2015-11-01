#include "Packets.h"


///////////////////////////// Сериализация ////////////////////////////////////
//Для клиента
int PacketCoderDecoder::CodeRequestLoadChat(const int chatId, char*& buffer)
{
	return serialize(chatId, REQUEST_LOAD_CHAT, buffer);
}

int PacketCoderDecoder::CodeRequestUserRegister(const User& user, char*& buffer)
{
	return serialize(user, REQUEST_USER_REGISTER, buffer);
}

int PacketCoderDecoder::CodeRequestUserConnect(const User& cc, char*& buffer)
{
	return serialize(cc, REQUEST_USER_CONNECT, buffer);
}


int PacketCoderDecoder::CodeRequestClose(const int userId, char*& buffer)
{
	int size = sizeof(PacketTypes);
	buffer = new char[size];
	Serializers::Serialize(REQUEST_CLOSE, buffer);
	return size;
}

//Для сервера
int PacketCoderDecoder::CodeDataUsersList(const QList<User>& users, char*& buffer)
{
	return code_list(users, DATA_USERS_LIST, buffer);
}

int PacketCoderDecoder::CodeDataChat(const Chat& chat, char*& buffer)
{
	return serialize(chat, DATA_CHAT, buffer);
}

int PacketCoderDecoder::CodeDataUser(const User& user, char*& buffer)
{
	return serialize(user, DATA_USER, buffer);
}

int PacketCoderDecoder::CodeDataError(char *& buffer)
{
	buffer = new char[sizeof(PacketTypes)];
	PacketTypes t = DATA_ERROR;
	memcpy(buffer, &t, sizeof(PacketTypes));
	return sizeof(PacketTypes);
}

//для клиента и сервера
int PacketCoderDecoder::CodeDataMessage(const Message& message, char*& buffer)
{
	return serialize(message, DATA_MESSAGE, buffer);
}


/////////////////////////// Десериализация //////////////////////////

//для сервера
int PacketCoderDecoder::DecodeRequestLoadChat(int& chatId, const char* buffer)
{
	return Serializers::Deserialize(chatId, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeRequestUserRegister(User& user, const char* buffer)
{
	return Serializers::Deserialize(user, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeRequestUserConnect(User& cc, const char* buffer)
{
	return Serializers::Deserialize(cc, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeRequestChatCreate(Chat& chat, const char* buffer)
{
	return Serializers::Deserialize(chat, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeRequestAddUser(int& userId, int& chatId, const char* buffer)
{
	buffer += sizeof(PacketTypes);
	buffer += Serializers::Deserialize(userId, buffer);
	Serializers::Deserialize(chatId, buffer);
	return sizeof(int) * 2;
}


//для клиента
int PacketCoderDecoder::DecodeDataUsersList(QList<User>& users, const char* buffer)
{
	return decode_list(users, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeDataChatsList(QList<Chat>& chats, const char* buffer)
{
	return decode_list(chats, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeDataChat(Chat& chat, const char* buffer)
{
	return Serializers::Deserialize(chat, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeDataUser(User& user, const char* buffer)
{
	return Serializers::Deserialize(user, buffer + sizeof(PacketTypes));
}

//для клиента и сервера
int PacketCoderDecoder::DecodeDataMessage(Message& message, const char* buffer)
{
	return Serializers::Deserialize(message, buffer + sizeof(PacketTypes));
}

/////////////////////////////////////////////////////////////////////
//Сериализация списка
template <class T>
static int PacketCoderDecoder::code_list(const QList<T>& list, const PacketTypes type, char*& buffer)
{
	//Определяем размер
	//Код пакета + число записей + необходимое место для каждой запис
	int size = sizeof(PacketTypes) + sizeof(int) + Serializers::Sizeof<T>(list);
	int pos;
	buffer = new char[size];
	pos = Serializers::Serialize(type, buffer);
	Serializers::Serialize(list, buffer+pos);

	return size;
}

//Десериализация списков
template <class T>
static int PacketCoderDecoder::decode_list(QList<T>& list, const char* buffer)
{
	const char* buffer0 = buffer;
	buffer += Serializers::Deserialize(list, buffer);
	return buffer - buffer0;
}

//Сериализует просто тип
template <class T>
int PacketCoderDecoder::serialize(const T& t, const PacketTypes type, char*& buffer)
{
	int size = Serializers::Sizeof(t) + sizeof(PacketTypes);
	int pos;
	buffer = new char[size];
	pos = Serializers::Serialize(type, buffer);
	Serializers::Serialize(t, buffer+pos);
	return size;
}