#pragma once
#include <Windows.h>
#include <time.h>


class Locker
{
public:
	static HANDLE GetMutex();
	static void Wait();
	static void Release();
private:
	Locker();
	Locker(const Locker&){}

	static Locker* instance;
	HANDLE mutex;
};