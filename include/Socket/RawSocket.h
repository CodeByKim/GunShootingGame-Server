#pragma once
#include "Library.h"

class RawSocket
{
protected:
	SOCKET socket;
	SOCKADDR_IN socketAddress;

	OVERLAPPED_EX receiveIOData;
	OVERLAPPED_EX sendIOData;

	CHAR readDataBuffer[MAX_BUFFER_LENGTH];
	DWORD readDataLength;
	DWORD socketFlag;

public:
	RawSocket();
	virtual ~RawSocket();

	virtual VOID Receive() = 0;
	virtual VOID Send(CHAR* data, INT length) = 0;
	VOID Bind();

	VOID SetSocketAddress(SOCKADDR_IN socketAddress);
	VOID SetSocketAddress(ULONG address, USHORT port);
	SOCKADDR_IN GetSocketAddress();

	VOID SetSocketHandle(SOCKET socket);
	SOCKET GetSocketHandle();

	CHAR* GetReadDataBuffer();

	VOID Close();
};