#pragma once
#include "Library.h"
#include "SystemThread.h"
#include "Locker.h"
#include "Package.h"

class ProcessPacketThread : public SystemThread
{
private:
	Locker* locker;
	queue<Package*>* packageQueue;
	
public:
	ProcessPacketThread();

	VOID Initialize(Locker* locker, queue<Package*>* packageQueue);
	virtual VOID Process();
};