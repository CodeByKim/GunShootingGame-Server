#pragma once
#include "Library.h"
#include "Defines.h"

class ThreadAttribute
{
private:
	DWORD stackSize;
	DWORD threadID;
	wstring threadName;

public:
	ThreadAttribute();

	VOID SetName(wstring name);
	wstring GetName();

	VOID SetStackSize(DWORD size);
	DWORD GetStackSize();

	VOID SetThreadID(DWORD id);
	DWORD GetThreadID();
};