#include "Locker.h"

Locker* Locker::instance;

Locker::Locker()
{
	//sra
	mutex = CreateMutexA(NULL, false, NULL);
}


HANDLE Locker::GetMutex()
{
	if (instance == NULL)
	{
		instance = new Locker();
	}

	return instance->mutex;
}

void Locker::Wait()
{
	WaitForSingleObject(Locker::GetMutex(), INFINITE);
}

void Locker::Release()
{
	ReleaseMutex(Locker::GetMutex());
}