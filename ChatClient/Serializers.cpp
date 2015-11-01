#include "Serializers.h"

///////////////////////////// QString ////////////////////////////////////////
//Размер буфера под строку
int Serializers::Sizeof(const QString& string)
{
	int size = (string.size() + 1);		//Размер строки+нуль-символ
	return size*sizeof(QChar) + sizeof(size);
}

//Сереализует QString
int Serializers::Serialize(const QString& string, char* data)
{
	int size = (string.size() + 1);		//Размер строки+нуль-символ

	//Записываем длину строки в символах
	memcpy(data, &size, sizeof(size));

	//Записываем строку
	memcpy(data + sizeof(size), string.data(), size*sizeof(QChar));

	return size*sizeof(QChar) + sizeof(size);
}

//Десериализует QString
int Serializers::Deserialize(QString& string, const char* data)
{
	int size;			//Размер строки+нуль-символ

	//Считываем размер
	memcpy(&size, data, sizeof(size));
	string.resize(size - 1);

	//Считываем строку
	memcpy(string.data(), data + sizeof(size), sizeof(QChar)*size);

	return size*sizeof(QChar) + sizeof(size);
}

///////////////////////// Простые типы ////////////////////////////////////////



//////////////////////////// User ////////////////////////////////////////////
int Serializers::Sizeof(const User& val)
{
	return sizeof(int) +				//Id
		Sizeof(val.Name) +				//Name
		Sizeof(val.Password) +			//Password
		sizeof(bool) +                  //IsOnline
		Sizeof<int>(val.Chats);//		//Chats
}

int Serializers::Serialize(const User& val, char* buffer)
{
	char* buffer0 = buffer;

	buffer += Serialize(val.Id, buffer);
	buffer += Serialize(val.Name, buffer);
	buffer += Serialize(val.Password, buffer);
	buffer += Serialize(val.Chats, buffer);

	return buffer - buffer0;
}

int Serializers::Deserialize(User& val, const char* buffer)
{
	const char* buffer0 = buffer;

	buffer += Deserialize(val.Id, buffer);
	buffer += Deserialize(val.Name, buffer);
	buffer += Deserialize(val.Password, buffer);
	buffer += Deserialize(val.IsOnline, buffer);
	buffer += Deserialize(val.Chats, buffer);

	return buffer - buffer0;
}

////////////////////////// Chat ///////////////////////////////////////////////
int Serializers::Sizeof(const Chat& val)
{
	return sizeof(int) + Sizeof(val.Users) + Sizeof<Message>(val.Messages);
}

int Serializers::Serialize(const Chat& val, char* buffer)
{
	char* buffer0 = buffer;
	buffer += Serialize(val.Id, buffer);
	buffer += Serialize<int>(val.Users, buffer);
	buffer += Serialize<Message>(val.Messages, buffer);
	return buffer - buffer0;
}

int Serializers::Deserialize(Chat& val, const char* buffer)
{
	const char* buffer0 = buffer;
	buffer += Deserialize(val.Id, buffer);
	buffer += Deserialize<int>(val.Users, buffer);
	buffer += Deserialize<Message>(val.Messages, buffer);
	return buffer - buffer0;
}

////////////////////////// Chat  Only Info ////////////////////////////////////
int Serializers::Sizeof(const ChatOnlyInfo & val)
{
	return sizeof(int) + Sizeof(val.Users);
}

int Serializers::Serialize(const ChatOnlyInfo & val, char* buffer)
{
	char* buffer0 = buffer;
	buffer += Serialize(val.Id, buffer);
	buffer += Serialize<int>(val.Users, buffer);
	return buffer - buffer0;
}

int Serializers::Deserialize(ChatOnlyInfo & val, const char* buffer)
{
	const char* buffer0 = buffer;
	buffer += Deserialize(val.Id, buffer);
	buffer += Deserialize<int>(val.Users, buffer);
	return buffer - buffer0;
}

/////////////////////////// Message //////////////////////////////////////////
int Serializers::Sizeof(const Message& val)
{
	return sizeof(int) + Sizeof(val.Text);
}

int Serializers::Serialize(const Message& val, char* buffer)
{
	char* buffer0 = buffer;
	buffer += Serialize(val.Id, buffer);
	buffer += Serialize(val.Text, buffer);
	return buffer - buffer0;
}

int Serializers::Deserialize(Message& val, const char* buffer)
{
	const char* buffer0 = buffer;
	buffer += Deserialize(val.Id, buffer);
	buffer += Deserialize(val.Text, buffer);
	return buffer - buffer0;
}

