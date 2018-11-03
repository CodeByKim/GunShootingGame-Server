#pragma once
#include "Library.h"
#include "AcceptThread.h"
#include "TcpSession.h"

class TcpAcceptor : public TcpSession
{
private:
	AcceptThread* acceptThread;

public:
	TcpAcceptor();
	virtual ~TcpAcceptor();

	VOID Listen(INT backLog);
	VOID Run();
};