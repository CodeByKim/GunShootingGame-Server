#include "Locker.h"

VOID Locker::Initialize()
{
	InitializeCriticalSection(&cs);
}

VOID Locker::Lock()
{
	EnterCriticalSection(&cs);
}

VOID Locker::Unlock()
{
	LeaveCriticalSection(&cs);
}