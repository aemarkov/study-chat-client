#include "Serializers.h"

///////////////////////////// STD::STRING ////////////////////////////////////////
//Размер буфера под строку
int Serializers::Sizeof(const std::string& string)
{
	int size = (string.size() + 1);		//Размер строки+нуль-символ
	return size*sizeof(char) + sizeof(size);
}

//Сереализует string
int Serializers::Serialize(const std::string & string, char* data)
{
	int size = (string.size() + 1);		//Размер строки+нуль-символ

	//Записываем длину строки в символах
	memcpy(data, &size, sizeof(size));

	//Записываем строку
	memcpy(data + sizeof(size), string.data(), size);

	return size*sizeof(char) + sizeof(size);
}

//Десериализует string
int Serializers::Deserialize(std::string& string, const char* data)
{
	int size;			//Размер строки+нуль-символ

	//Считываем размер
	memcpy(&size, data, sizeof(size));
	string.resize(size - 1);

	//Считываем строку
	char* cstr = new char[size];
	memcpy(cstr, data + sizeof(size), sizeof(char)*size);
	string = std::string(cstr);

	return size*sizeof(char) + sizeof(size);
}

///////////////////////// Простые типы ////////////////////////////////////////



//////////////////////////// User ////////////////////////////////////////////
int Serializers::Sizeof(const User& val)
{
	return sizeof(int) +				//Id
		Sizeof(val.Name) +				//Name
		Sizeof(val.Password) +			//Password
		sizeof(bool);	                //IsOnline
}

int Serializers::Serialize(const User& val, char* buffer)
{
	char* buffer0 = buffer;

	buffer += Serialize(val.Id, buffer);
	buffer += Serialize(val.Name, buffer);
	buffer += Serialize(val.Password, buffer);
	buffer += Serialize(val.IsOnline, buffer);

	return buffer - buffer0;
}

int Serializers::Deserialize(User& val, const char* buffer)
{
	const char* buffer0 = buffer;

	buffer += Deserialize(val.Id, buffer);
	buffer += Deserialize(val.Name, buffer);
	buffer += Deserialize(val.Password, buffer);
	buffer += Deserialize(val.IsOnline, buffer);

	return buffer - buffer0;
}

////////////////////////// Chat ///////////////////////////////////////////////
int Serializers::Sizeof(const Chat& val)
{
	return sizeof(int) + Sizeof<Message>(val.Messages);
}

int Serializers::Serialize(const Chat& val, char* buffer)
{
	char* buffer0 = buffer;
	buffer += Serialize(val.Id, buffer);
	buffer += Serialize<Message>(val.Messages, buffer);
	return buffer - buffer0;
}

int Serializers::Deserialize(Chat& val, const char* buffer)
{
	const char* buffer0 = buffer;
	buffer += Deserialize(val.Id, buffer);
	buffer += Deserialize<Message>(val.Messages, buffer);
	return buffer - buffer0;
}


/////////////////////////// Message //////////////////////////////////////////
int Serializers::Sizeof(const Message& val)
{
	return sizeof(int) + Sizeof(val.Text) + Sizeof(val.Author);
}

int Serializers::Serialize(const Message& val, char* buffer)
{
	char* buffer0 = buffer;
	buffer += Serialize(val.Id, buffer);
	buffer += Serialize(val.Text, buffer);
	buffer += Serialize(val.Author, buffer);
	return buffer - buffer0;
}

int Serializers::Deserialize(Message& val, const char* buffer)
{
	const char* buffer0 = buffer;
	buffer += Deserialize(val.Id, buffer);
	buffer += Deserialize(val.Text, buffer);
	buffer += Deserialize(val.Author, buffer);
	return buffer - buffer0;
}

