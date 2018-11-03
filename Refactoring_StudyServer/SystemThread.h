#pragma once
#include "Library.h"
#include "ThreadAttribute.h"

class SystemThread
{
private:
	ThreadAttribute* attribute;
	HANDLE thread;
	
	static UINT32 WINAPI CallThread(VOID* args);

protected:
	BOOL isRun;
	HANDLE exitWaitEvent;
	
public:
	SystemThread();

	VOID SetAttribute(ThreadAttribute* attribute);
	VOID Start();	
	VOID WaitForExit();
	VOID Stop();

	ThreadAttribute* GetAttribute();

	virtual VOID Process() = 0;
};