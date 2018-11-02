#pragma once
#include "Library.h"
#include "SystemThread.h"
#include "Player.h"

class SenderThread : public SystemThread
{
private:
	INT index;
	
public:
	SenderThread(INT index);

	virtual VOID Process();
};