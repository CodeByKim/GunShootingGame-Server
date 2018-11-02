#pragma once
#include "SystemThread.h"

class HeartBeatCheckThread : public SystemThread
{
private:

public:
	HeartBeatCheckThread();
	virtual VOID Process();
};