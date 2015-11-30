#include "GUI.h"


//Получение размера консоли
COORD GUI::getConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD size;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	size.X = csbi.dwSize.X;
	size.Y = csbi.srWindow.Bottom-3;
	return size;
}

void GUI::draw_interface()
{
	consoleSize = getConsoleSize();

	/*
	|---------------------|
	|-----|---------------|
	|-----|               |
	|     |               |
	|     |---------------|
	|-----|---------------|
	*/

	//Заголовок
	//headerHeight = 2;																								//Высота заголовка
	ConsoleGraphics::fillRect(0, 0, consoleSize.X - 1, headerHeight, BLUE_NONE);									//Заливка
	ConsoleGraphics::drawRect(0, 0, consoleSize.X - 1, headerHeight, BLUE_WHITE);									//Рамка
	ConsoleGraphics::drawText("CONSOLE CHAT", 0, 0, consoleSize.X - 1, headerHeight, BLUE_WHITE);					//Текст

	//Поле контактов
	contactsWidth = consoleSize.X / 3;																				//Ширина панели контактов
	ConsoleGraphics::fillRect(0, headerHeight + 1, contactsWidth, consoleSize.Y - 1, CYAN_NONE);					//Заливка
	ConsoleGraphics::drawRect(0, headerHeight + 1, contactsWidth, 2 * headerHeight + 1, CYAN_BLACK);				//Заголовок
	ConsoleGraphics::drawRect(0, 2 * headerHeight + 1, contactsWidth, consoleSize.Y - 1, CYAN_BLACK);				//Поле контактов
	ConsoleGraphics::drawSymbol('\xCC\xB1', 0, 2 * headerHeight + 1);												//Левый уголок
	ConsoleGraphics::drawSymbol('\xB9\xB5', contactsWidth, 2 * headerHeight + 1);									//Правый уголок
	ConsoleGraphics::drawText("CONTACTS", 0, headerHeight + 1, contactsWidth, 2 * headerHeight + 1, CYAN_BLACK);


	//Поле ввода
	//inputHeight = 5;
	ConsoleGraphics::fillRect(contactsWidth + 1, consoleSize.Y - 1 - inputHeight, consoleSize.X - 1, consoleSize.Y - 1, WHITE_NONE);
	ConsoleGraphics::drawRect(contactsWidth + 1, consoleSize.Y - 1 - inputHeight, consoleSize.X - 1, consoleSize.Y - 1, WHITE_BLACK);

	//Рисуем область чата
	ConsoleGraphics::fillRect(contactsWidth + 1, headerHeight + 1, consoleSize.X - 1, consoleSize.Y - 2 - inputHeight, GRAY_NONE);
	ConsoleGraphics::drawRect(contactsWidth + 1, headerHeight + 1, consoleSize.X - 1, consoleSize.Y - 2 - inputHeight, GRAY_BLACK);
}

//Создает элементы управленяи
void GUI::create_controls()
{
	RECT rect;

	//Создает поле ввода
	rect.left = contactsWidth + 2;
	rect.top = consoleSize.Y - 1 - inputHeight + 1;
	rect.right = consoleSize.X - 2;
	rect.bottom = consoleSize.Y - 2;
	inputArea = new TextArea(consoleHandle, rect, WHITE_BLACK);
	inputArea->SetCursor();

	//Создаем область чата
	rect.left = contactsWidth + 2;
	rect.top = headerHeight + 2;
	rect.right = consoleSize.X - 2;
	rect.bottom = consoleSize.Y - 3 - inputHeight;
	chatArea = new TextArea(consoleHandle, rect, GRAY_BLACK);

	//Создает поле ввода
	rect.left = 1;
	rect.right = contactsWidth-1;
	rect.top = headerHeight + 4;
	rect.bottom = consoleSize.Y - 2;
	list = new UsersList(consoleHandle, rect, CYAN_BLACK);
}


//Перерисовыает интерфейс
void GUI::redraw_interface()
{
	draw_interface();
	chatArea->Redraw();
	inputArea->Redraw();
}

RECT GUI::draw_popup_box()
{
	int width = consoleSize.X / 2;
	int height = consoleSize.Y / 2;

	int left = consoleSize.X / 2 - width / 2;
	int top = consoleSize.Y / 2 - height / 2;
	int right = consoleSize.X / 2 + width / 2;
	int bottom = consoleSize.Y / 2 + height / 2;

	ConsoleGraphics::fillRect(left, top, right, bottom, RED_WHITE);
	ConsoleGraphics::drawRect(left, top, right, bottom, RED_WHITE);

	SetConsoleTextAttribute(consoleHandle, BLUE_NONE);
	for (int i = left + 1; i < right + 1; i++)
		ConsoleGraphics::drawSymbol(' ', i, bottom + 1);
	for (int i = top + 1; i <= bottom + 1; i++)
		ConsoleGraphics::drawSymbol(' ', right + 1, i);

	RECT rect;
	rect.left = left;
	rect.bottom = bottom;
	rect.right = right;
	rect.top = top;
	return rect;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Рисование интерфейса
void GUI::DrawInterface()
{
	draw_interface();
	create_controls();
}

//Вывод сообщения
bool GUI::DrawMessage(std::string text)
{
	auto rect = draw_popup_box();
	int x = consoleSize.X / 2;
	int y = consoleSize.Y / 2;

	ConsoleGraphics::drawText(text.c_str(), rect, RED_WHITE);
	ConsoleGraphics::drawText("<ENTER/ESC>", rect.left, y+1, rect.right, y+1, RED_WHITE);
	char c = _getch();

	redraw_interface();
	return c == '\r';
}

char GUI::DrawMenu()
{
	auto rect = draw_popup_box();
	int x = consoleSize.X / 2;
	int y = consoleSize.Y / 2;
	ConsoleGraphics::drawText("MAIN_MENU", rect.left, y - 2, rect.right, y - 2, RED_WHITE);
	ConsoleGraphics::drawText("L - LOGIN, R - REGISTER", rect.left, y + 1, rect.right, y + 1, RED_WHITE);
	
	char c = _getch();
	redraw_interface();

	return c;
}

ConnectionCredentials GUI::DrawLogin()
{
	std::string login, password;
	auto rect = draw_popup_box();
	int x = consoleSize.X / 2;
	int y = consoleSize.Y / 2;
	COORD p;

	//Поле логина
	ConsoleGraphics::drawText("LOGIN: ", rect.left + 3, y - 1, rect.left + 11, y - 1, RED_WHITE);
	ConsoleGraphics::fillRect(rect.left + 12, y - 1, rect.right - 3, y - 1, GRAY_BLACK);

	//Поле пароля
	ConsoleGraphics::drawText("PASSWORD: ", rect.left + 3, y + 2, rect.left + 11, y + 2, RED_WHITE);
	ConsoleGraphics::fillRect(rect.left + 12, y + 2, rect.right - 3, y + 2, GRAY_BLACK);

	//Куроср на поле логина
	p.X = rect.left + 12;
	p.Y = y - 1;
	SetConsoleCursorPosition(consoleHandle, p);

	std::cin >> login;

	p.X = rect.left + 12;
	p.Y = y + 2;
	SetConsoleCursorPosition(consoleHandle, p);

	/*char c;
	while ((c = _getch()) != '\r')
	{
		std::cout << "*";
		password += c;
	}*/
	std::cin >> password;
	
	redraw_interface();
	return ConnectionCredentials(login, password);
}


GUI::GUI()
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleGraphics::SetHandle(consoleHandle);
}