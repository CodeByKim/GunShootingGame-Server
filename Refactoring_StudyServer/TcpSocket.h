#pragma once
#include "Library.h"
#include "RawSocket.h"

class TcpSocket : public RawSocket
{
private:

public:
	TcpSocket();
	virtual ~TcpSocket();

	virtual VOID Receive();
	virtual VOID Send(CHAR* data, INT length);
	VOID Listen(INT backlog);
	TcpSocket* Accept();
};