#pragma once
#include "Library.h"
#include "SystemThread.h"

class IocpWorkerThread : public SystemThread
{
private:
	HANDLE iocpHandle;

public:
	IocpWorkerThread();

	VOID SetIocpHandle(HANDLE iocpHandle);
	virtual VOID Process();
};