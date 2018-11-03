#include "SystemThread.h"

UINT32 WINAPI SystemThread::CallThread(VOID* arg)
{
	SystemThread* thread = (SystemThread*)arg;
	thread->Process();

	return 0;
}

SystemThread::SystemThread()
{
	this->attribute = NULL;
	this->thread = INVALID_HANDLE_VALUE;
	this->isRun = FALSE;
	this->exitWaitEvent = CreateEvent(0, FALSE, FALSE, 0);
}

VOID SystemThread::SetAttribute(ThreadAttribute* attribute)
{
	this->attribute = attribute;
}

VOID SystemThread::Start()
{
	if (this->attribute == NULL)
		SetAttribute(new ThreadAttribute());

	UINT threadID = 0;

	this->thread = (HANDLE)_beginthreadex(0,
		this->attribute->GetStackSize(), CallThread, (VOID*)this, 0, &threadID);

	this->attribute->SetThreadID(threadID);
	this->isRun = TRUE;
}

VOID SystemThread::Stop()
{
	this->isRun = FALSE;		
	
}

VOID SystemThread::WaitForExit()
{
	WaitForSingleObject(this->exitWaitEvent, INFINITE);
}

ThreadAttribute* SystemThread::GetAttribute()
{
	return this->attribute;
}