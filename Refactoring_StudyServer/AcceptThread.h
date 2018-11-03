#pragma once
#include "Library.h"
#include "SystemThread.h"
#include "TcpSocket.h"

class AcceptThread : public SystemThread
{
private:
	TcpSocket* socket;

public:
	AcceptThread();
	virtual ~AcceptThread();

	VOID SetListenSocket(TcpSocket* socket);
	virtual VOID Process();
};