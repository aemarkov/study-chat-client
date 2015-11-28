#include "Packets.h"


///////////////////////////// Сериализация ////////////////////////////////////
//Для клиента


int PacketCoderDecoder::CodeRequestLoadChat(char*& buffer)
{
	int size = sizeof(PacketTypes);
	buffer = new char[size];
	Serializers::Serialize(REQUEST_LOAD_CHAT, buffer);
	return size;
}

int PacketCoderDecoder::CodeRequestUsersList(char *& buffer)
{
	int size = sizeof(PacketTypes);
	buffer = new char[size];
	Serializers::Serialize(REQUEST_USERS_LIST, buffer);
	return size;
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
int PacketCoderDecoder::CodeDataUsersList(const std::vector<User>& users, char*& buffer)
{
	//Этот карнавал нужен, чтобы не отправлять пользовательские пароли
	
	//Определяем размер буфера
	int size = sizeof(PacketTypes) + sizeof(int);
	for (int i = 0;i < users.size(); i++)
	{
		User u = users[i];
		u.Password = "";
		size += Serializers::Sizeof(u);
	}

	//Отправляем
	int pos;
	buffer = new char[size];
	PacketTypes type = DATA_USERS_LIST;
	pos = Serializers::Serialize(type, buffer);
	pos += Serializers::Serialize(users.size(), buffer + pos);

	for (int i = 0; i < users.size(); i++)
	{
		User u = users[i];
		u.Password = "";
		pos += Serializers::Serialize(u, buffer + pos);
	}
	return size;
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
	int size = sizeof(PacketTypes);
	buffer = new char[size];
	Serializers::Serialize(DATA_ERROR, buffer);
	return size;
}

//для клиента и сервера
int PacketCoderDecoder::CodeDataMessage(const Message& message, char*& buffer)
{
	return serialize(message, DATA_MESSAGE, buffer);
}


/////////////////////////// Десериализация //////////////////////////

//для сервера
int PacketCoderDecoder::DecodeRequestUserRegister(User& user, const char* buffer)
{
	return Serializers::Deserialize(user, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeRequestUserConnect(User& cc, const char* buffer)
{
	return Serializers::Deserialize(cc, buffer + sizeof(PacketTypes));
}


//для клиента
int PacketCoderDecoder::DecodeDataUsersList(std::vector<User>& users, const char* buffer)
{
	return decode_list(users, buffer + sizeof(PacketTypes));
}

int PacketCoderDecoder::DecodeDataChatsList(std::vector<Chat>& chats, const char* buffer)
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
static int PacketCoderDecoder::code_list(const std::vector<T>& list, const PacketTypes type, char*& buffer)
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
static int PacketCoderDecoder::decode_list(std::vector<T>& list, const char* buffer)
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