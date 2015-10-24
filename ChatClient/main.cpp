
#include <QtCore/QCoreApplication>
//#include <QString>
#include <iostream>

using namespace std;

int main_menu();
bool login();
bool sign_up();


int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	cout << "-------------------------------------------------\n";
	cout << "|         Консольный чат, v0.1                  |\n";
	cout << "-------------------------------------------------\n";
	
	int answer = main_menu();
	if (answer == 1)
		sign_up();
	else if (answer == 2)
		login();

	return 0;
}

//Показывает главное меню
int main_menu()
{
	cout << "          ГЛАВНОЕ МЕНЮ\n";
	cout << "    1. Регистрация на сервере\n";
	cout << "    2. Вход на сервер\n";
	cout << "> ";
	int answer;
	cin >> answer;
	return answer;
}

//Вход на сервер
bool login()
{
	std::string name, password;
	cout << "             ВХОД\n";
	cout << "Имя:    ";
	cin >> name;
	cout << "Пароль: ";
	cin >> password;
	return 0;
}

//Регистрация
bool sign_up()
{
	std::string name, password;
	cout << "          РЕГИСТРАЦИЯ\n";
	cout << "Имя:    ";
	cin >> name;
	cout << "Пароль: ";
	cin >> password;
	return 0;
}