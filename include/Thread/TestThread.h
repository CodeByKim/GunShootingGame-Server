#pragma once
#include "SystemThread.h"

class TestThread : public SystemThread
{
private:	

public:
	TestThread();
	virtual VOID Process();
};