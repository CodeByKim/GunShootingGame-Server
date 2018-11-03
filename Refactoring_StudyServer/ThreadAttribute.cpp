#include "ThreadAttribute.h"

ThreadAttribute::ThreadAttribute()
{
	this->stackSize = 0;
	this->threadID = 0;	
	this->threadName = L"None";
}

VOID ThreadAttribute::SetName(wstring name)
{
	this->threadName = name;
}

wstring ThreadAttribute::GetName()
{
	return this->threadName;
}

VOID ThreadAttribute::SetStackSize(DWORD size)
{
	this->stackSize = size;
}

DWORD ThreadAttribute::GetStackSize()
{
	return this->stackSize;
}

VOID ThreadAttribute::SetThreadID(DWORD id)
{
	this->threadID = id;
}

DWORD ThreadAttribute::GetThreadID()
{
	return this->threadID;
}
