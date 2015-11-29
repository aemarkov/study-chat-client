#pragma once
#include <iostream>
#include <Windows.h>
#include "TextArea.h"
#include "ConsoleGraphics.h"
#include "ConnectionCredentials.h"
#include "UsersList.h"
#include <conio.h>


class GUI
{
public:

	//Конструктор
	GUI();

	//Рисует интерфейс
	void DrawInterface();
		
	bool DrawMessage(std::string text);			//Выводит сообщение
	char DrawMenu();							//Выводт меню
	ConnectionCredentials DrawLogin();			//Выводит окно входа

	TextArea* inputArea;			//Поле ввода
	TextArea* chatArea;				//Чаты
	UsersList* list;				//Список пользователей

private:
	HANDLE consoleHandle;			//Хэндл косноли
	COORD  consoleSize;				//Размер консоли
	int    contactsWidth;			//Ширина поля контактов
	int    headerHeight = 2;		//Высота заголовка
	int    inputHeight = 5;			//Высота поля ввода

	//Получение размера консоли
	COORD getConsoleSize();

	void draw_interface();			//Рисует интерфейс
	void create_controls();			//Создает элементы
	void redraw_interface();		//Перерисовывает интерфейс

	RECT draw_popup_box();			//Рисует окно сообщения
};