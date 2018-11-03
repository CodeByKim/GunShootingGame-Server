#pragma once
#include "Library.h"
#include "IocpWorkerThread.h"
#include "Player.h"

class IOCP
{
private:
	WSAData wsaData;
	HANDLE iocpHandle;
	INT workerThreadCount;
	vector<IocpWorkerThread*> workerThreads;

public:
	virtual VOID Initialize();
	virtual VOID Release();
	
	virtual VOID Run();
	VOID RegisterSessionToIOCP(Player* player);

	HANDLE GetHandle();
};