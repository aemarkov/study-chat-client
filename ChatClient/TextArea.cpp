#include "TextArea.h"

TextArea::TextArea()
{
	consoleHandle = INVALID_HANDLE_VALUE;
}

//Создает объект
TextArea::TextArea(HANDLE consoleHandle, RECT rect, int attributes)
{
	this->consoleHandle = consoleHandle;
	Rect = rect;
	CursorPos.X = Rect.left;
	CursorPos.Y = Rect.top;
	currentCursorPos.X = CursorPos.X - 1;
	currentCursorPos.Y = CursorPos.Y;
	Attributes = attributes;

	//Настраиваем страницы
	current_page = 0;
	pages.push_back(std::string());

	draw_page_indicator();
}

//Устанавливает курсор в текущее положение в текстовом поле
void TextArea::SetCursor()
{
	SetConsoleCursorPosition(consoleHandle, currentCursorPos);
}

//Принимает очередной символ
bool TextArea::AddSymbol(char symbol)
{
	return add_symbol(symbol, true);
}


//Скроллинг вверх на 1 страницу
void TextArea::ScrollUp()
{
	get_upper_page();
}


//Скроллинг вниз на 1 страницу
void TextArea::ScrollDown()
{
	if (current_page < pages.size() - 1)
		get_bottom_page();
}

//Очистка поля
void TextArea::Clear()
{
	pages.clear();
	pages.push_back(std::string());
	draw_page(pages[0]);
	current_page = 0;
}

//Возвращает тект
std::string TextArea::GetText()
{
	std::string str;
	for (int i = 0; i < pages.size(); i++)
		str += pages[i] + "\n";

	return str;
}

//Возвращает позициую курсора
COORD TextArea::GetCursorPos()
{
	return currentCursorPos;
}

//Перерисовывает
void TextArea::Redraw()
{
	draw_page(pages[current_page]);
}

///////////////////////////////////////////////////////////////////////////

//Добавляет символ, но можно выбрать - печатать и добавлять в тект
//или только печатать
bool TextArea::add_symbol(char symbol, bool add)
{
	//Задаем нужные аттрибуты текста
	SetConsoleTextAttribute(consoleHandle, Attributes);

	if (symbol == '\b')
	{
		//Удаление символа
		if (currentCursorPos.X == Rect.left - 1)return false;

		ConsoleGraphics::drawSymbol(' ', currentCursorPos);

		currentCursorPos.X--;

		//Переход на верхнюю строку
		if (currentCursorPos.X < Rect.left)
		{
			if (currentCursorPos.Y > Rect.top)
			{
				currentCursorPos.X = Rect.right;
				currentCursorPos.Y--;
			}
		}

		//Смена положение куросра
		if ((currentCursorPos.X >= Rect.left) && (currentCursorPos.Y >= Rect.top))
			SetCursor();

		//Удаление последнего символа
		if (add)
			pages[current_page] = pages[current_page].substr(0, pages[current_page].length() - 1);

	}
	else if (symbol == '\n')
	{
		//Перенос строки
		if (currentCursorPos.Y < Rect.bottom)
		{
			currentCursorPos.Y++;
			currentCursorPos.X = Rect.left - 1;
			SetCursor();


			if (add)
				pages[current_page] += symbol;
		}
		else
			get_bottom_page();
	}
	else if (symbol == -115)
	{
		//Вверх
		get_upper_page();
	}
	else if (symbol == -111)
	{
		//Вниз
		if (current_page < pages.size() - 1)
			get_bottom_page();
	}
	else
	{
		//Добавление символа
		currentCursorPos.X++;

		//Перенос при завершении строки
		if (currentCursorPos.X > Rect.right)
		{
			//Перенеос только если есть место
			if (currentCursorPos.Y < Rect.bottom)
			{
				currentCursorPos.Y++;
				currentCursorPos.X = Rect.left;
			}
			else
				get_bottom_page();
		}

		if ((currentCursorPos.X <= Rect.right) && (currentCursorPos.Y <= Rect.bottom))
		{
			if (add)
				pages[current_page] += symbol;

			ConsoleGraphics::drawSymbol(symbol, currentCursorPos);
		}
		else
			currentCursorPos.X--;

		SetCursor();
	}

	return false;
}

//Переход к нижней странице
void TextArea::get_bottom_page()
{
	current_page++;
	if (current_page >= pages.size())
	{
		//Создание новой страницы
		pages.push_back(std::string());
	}

	draw_page(pages[current_page]);
}

//Переход к верхней странице
void TextArea::get_upper_page()
{
	if (current_page > 0)
	{
		current_page--;
		draw_page(pages[current_page]);
	}
}

//Отрисовка страницы
void TextArea::draw_page(std::string str)
{
	//Сначала стираем весь текс
	ConsoleGraphics::fillRect(Rect, Attributes);
	currentCursorPos.X = CursorPos.X - 1;
	currentCursorPos.Y = CursorPos.Y;

	//Вводим текст
	int i = 0;
	while ((i < str.length()) && !add_symbol(str[i], false))
		i++;

	draw_page_indicator();
}

//Отображаем индикатор
void TextArea::draw_page_indicator()
{
	COORD c;
	c.X = Rect.left + 1;
	c.Y = Rect.top - 1;
	SetConsoleTextAttribute(consoleHandle, Attributes);
	SetConsoleCursorPosition(consoleHandle, c);
	std::cout << "[" << current_page + 1 << "/" << pages.size() << "]";
	SetCursor();
}

