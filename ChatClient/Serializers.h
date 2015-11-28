#pragma once

//Тут содержаться все сериализаторы объектов в виде перегруженных функций
//Это нужно для того, чтобы сериализаторы списков могли быть использованы
//не только с простыми типами, которые можно сделать memcpy, но и со сложными,
//требующими сериализации
//Так же здесь находятся сами сериализаторы списков

#include "User.h"
#include "Chat.h"
#include "Message.h"

class Serializers
{

public:

	//std::string
	static int Sizeof(const std::string& string);
	static int Serialize(const std::string& string, char* data);
	static int Deserialize(std::string& string, const char* data);

	//vector
	//Возвращает размер буфера для хранения сериализованного списка
	template <class T>
	static int Sizeof(const std::vector<T>& list)
	{
		return get_list_buffer_size<std::vector<T>, T>((std::vector<T>)list);
	}

	//Сериализует список
	template <class T>
	static int Serialize(const std::vector<T>& list, char* data)
	{
		return serialize_list<std::vector<T>, T>(list, data);
	}

	//Десериализует список
	//Предполагается, что передан массив верной длины
	template <class T>
	static int Deserialize(std::vector<T>& list, const char* data)
	{
		return deserialize_list<std::vector<T>, T>(list, data);
	}

	//list
	//Возвращает размер буфера для хранения связанного списка
	template <class T>
	static int Sizeof(const std::list<T>& list)
	{
		return get_list_buffer_size<std::list<T>, T>(list);
	}

	//Сериализует связанный список
	template <class T>
	static int Serialize(const std::list<T>& list, char* data)
	{
		return serialize_list<std::list<T>, T>(list, data);
	}

	//Десериализует связанный список
	//Предполагается, что передан массив верной длины
	template <class T>
	static int Deserialize(std::list<T>& list, const char* data)
	{
		return deserialize_list<std::list<T>, T>(list, data);
	}
	
	//Простые типы*, которые можно сериализовать
	//через memcpy
	//Всякие структуры без укзателей входят
	template <class T>
	static int Sizeof(const T val)
	{
		return sizeof(val);
	}

	template <class T>
	static int Serialize(const T val, char* buffer)
	{
		memcpy(buffer, &val, sizeof(val));
		return sizeof(val);
	}

	template <class T>
	static int Deserialize(T& val, const char* buffer)
	{
		memcpy(&val, buffer, sizeof(val));
		return sizeof(val);
	}

	//User
	static int Sizeof(const User& val);
	static int Serialize(const User& val, char* buffer);
	static int Deserialize(User& val, const char* buffer);

	//Message
	static int Sizeof(const Message& val);
	static int Serialize(const Message& val, char* buffer);
	static int Deserialize(Message& val, const char* buffer);

	//Chat
	static int Sizeof(const Chat& val);
	static int Serialize(const Chat& val, char* buffer);
	static int Deserialize(Chat& val, const char* buffer);

private:
	//Возвращает размер, необходимый для хранения списка
	template <class LIST, class T>
	static int get_list_buffer_size(const LIST &list)
	{
		int size = sizeof(int);
		for (auto i = list.begin(); i != list.end(); ++i)
			size += Sizeof(*i);
		return size;
	}

	//Десериализует список-шаблон
	template <class LIST, class T>
	static int deserialize_list(LIST& list, const char* data)
	{
		//Определяем размер списка
		int size;
		memcpy(&size, data, sizeof(int));
		data += sizeof(int); //Смещенеи указателя

		//Читаем элементы
		T element;
		for (int i = 0; i < size; i++)
		{
			int d = Deserialize(element, data);
			data += d;
			list.push_back(element);
		}

		return list.size()*sizeof(T);
	}

	//Сереализует список-шаблон
	template <class LIST, class T>
	static int serialize_list(const LIST list, char* data)
	{
		//Записываем размер
		int size = list.size();
		memcpy(data, &size, sizeof(int));
		data += sizeof(int);  //Смещение указателя

		//Записываем элементы
		for (auto i = list.begin(); i != list.end(); ++i)
		{
			int d = Serialize(*i, data);
			data += d;
		}
		

		return sizeof(int) + list.size()*sizeof(T);
	}
};