#pragma once
#include "Library.h"

class Locker
{
private:
	CRITICAL_SECTION cs;
public:
	VOID Initialize();
	VOID Lock();
	VOID Unlock();
};