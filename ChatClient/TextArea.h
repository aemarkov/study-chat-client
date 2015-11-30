#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include "ConsoleGraphics.h"
#include "Locker.h"

//Управление текстом в рамках ограниченного прямоугольника
class TextArea
{
public:

	TextArea();

	//Создает объект
	TextArea(HANDLE consoleHandle, RECT rect, int attributes);

	//Устанавливает курсор в текущее положение в текстовом поле
	void SetCursor();

	//Принимает очередной символ
	// возвращаемое значение показывает, был ли осуществлен переход
	// на следующую страницу
	bool AddSymbol(char symbol);

	//Ручное управление прокруткой
	void ScrollUp();
	void ScrollDown();

	//Очищает поле
	void Clear();

	//Выодает тект
	std::string GetText();

	//Возвращает положение курсора
	COORD GetCursorPos();

	//Перерисовывает
	void Redraw();

private:
	COORD currentCursorPos;


	//Переходит к нижней странице, либо создает новую
	void get_bottom_page();
	
	//Переходит к верхней странице
	void get_upper_page();

	//Рисует страницу
	void draw_page(std::string str);

	//Отображает счетчик страниц
	void draw_page_indicator();

	bool add_symbol(char symbol, bool add);

	//Управлание постраничным скроллом
	//int num_pages;
	int current_page;

	RECT Rect;					//Ограничивающий прямоугольник
	COORD CursorPos;			//Положение курсора в блоке
	int Attributes;				//Аттрибуты для графона
	HANDLE consoleHandle;		//

	std::vector<std::string> pages;	//Текст страниц
};