#pragma once

#define CYAN_NONE 51		//CYAN с текстом CYAN
#define CYAN_BLACK 48		//CYAN с черным текстом
#define DEFAULT 15			//Черная консоль с белым текстом
#define BLUE_NONE 17		//Синий с синим текстом
#define BLUE_WHITE 31		//Синий с белым текстом
#define GRAY_NONE 119
#define GRAY_BLACK 112
#define GRAY_RED 124
#define GRAY_BLUE 121
#define WHITE_NONE 255
#define WHITE_BLACK 240
#define RED_WHITE 79

#include <Windows.h>
#include <iostream>

//Содержит методы по рисованию псевдографикой
class ConsoleGraphics
{
public:

	//Устанавливает ХЭНДЛ КОНСОЛИ
	static void SetHandle(HANDLE handle);


	//Рисование символа в заданной позиции
	static void drawSymbol(char symbol, COORD pos);

	//Рисует символ в нужном месе
	static void drawSymbol(char symbol, int x, int y);


	//Рисование прямоугольника
	static void drawRect(RECT rect, int attribute);
	static void drawRect(int left, int top, int right, int bottom, int attribute);

	//Рисование закрашенного прямоугольника
	static void fillRect(RECT rect, int attribute);
	static void fillRect(int left, int top, int right, int bottom, int attribute);

	//Центрированный вывод текста
	static void drawText(const char* string, RECT rect, int attribute);
	static void drawText(const char* string, int left, int top, int right, int bottom, int attribute);

private:
	static HANDLE consoleHandle;

};