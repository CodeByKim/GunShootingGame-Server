#include "RawSocket.h"

RawSocket::RawSocket()
{
	this->socket = INVALID_SOCKET;
	ZeroMemory(&this->socketAddress, sizeof(SOCKADDR_IN));
	ZeroMemory(&this->receiveIOData, sizeof(OVERLAPPED_EX));
	ZeroMemory(&this->sendIOData, sizeof(OVERLAPPED_EX));
	ZeroMemory(readDataBuffer, MAX_BUFFER_LENGTH);

	this->receiveIOData.ioType = IO_OPERATION_TYPE::IO_RECEIVE;
	this->sendIOData.ioType = IO_OPERATION_TYPE::IO_SEND;
	this->readDataLength = 0;
	this->socketFlag = 0;
}

RawSocket::~RawSocket()
{
	this->socket = INVALID_SOCKET;
	ZeroMemory(&this->socketAddress, sizeof(SOCKADDR_IN));
	ZeroMemory(&this->receiveIOData, sizeof(OVERLAPPED_EX));
	ZeroMemory(&this->sendIOData, sizeof(OVERLAPPED_EX));
	ZeroMemory(readDataBuffer, MAX_BUFFER_LENGTH);
	this->readDataLength = 0;
	this->socketFlag = 0;
}

VOID RawSocket::Bind()
{
	if (this->socketAddress.sin_addr.S_un.S_addr == 0 ||
		this->socketAddress.sin_port == 0)
		bind(this->socket, (SOCKADDR*)&this->socketAddress, sizeof(this->socketAddress));
	else
		Utils::GetInstance()->LogError(L"Socket", L"소켓 주소가 초기화 되지 않음");		
}

VOID RawSocket::SetSocketAddress(SOCKADDR_IN socketAddress)
{
	this->socketAddress = socketAddress;
}

VOID RawSocket::SetSocketAddress(ULONG address, USHORT port)
{
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_addr.S_un.S_addr = htonl(address);
	this->socketAddress.sin_port = htons(port);
}

SOCKADDR_IN RawSocket::GetSocketAddress()
{
	return this->socketAddress;
}

VOID RawSocket::SetSocketHandle(SOCKET socket)
{
	this->socket = socket;
}

SOCKET RawSocket::GetSocketHandle()
{
	return this->socket;
}

CHAR* RawSocket::GetReadDataBuffer()
{
	return this->readDataBuffer;
}

VOID RawSocket::Close()
{
	closesocket(this->socket);
	this->socket = INVALID_SOCKET;
}