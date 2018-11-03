#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
	this->socket = WSASocket(AF_INET,
		SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
}

TcpSocket::~TcpSocket()
{	
	if (this->socket != INVALID_SOCKET)
		Close();
}

VOID TcpSocket::Receive()
{
	ZeroMemory(readDataBuffer, MAX_BUFFER_LENGTH);
	WSABUF wsaBuf;
	wsaBuf.buf = readDataBuffer;
	wsaBuf.len = MAX_BUFFER_LENGTH;

	WSARecv(this->socket, &wsaBuf, 1, &this->readDataLength,
	&this->socketFlag, &this->receiveIOData, NULL);
}

VOID TcpSocket::Send(CHAR* data, INT length)
{
	WSABUF wsaBuf;
	wsaBuf.buf = data;
	wsaBuf.len = length;

	DWORD sendByte = 0;
	DWORD sendFlag = 0;
	WSASend(this->socket, &wsaBuf, 1, &sendByte,
		sendFlag, &this->sendIOData, NULL);

	SAFE_DELETE_ARRAY(data);
}

VOID TcpSocket::Listen(INT backlog)
{
	listen(this->socket, backlog);
}

TcpSocket* TcpSocket::Accept()
{
	INT socketAddressLength = sizeof(SOCKADDR_IN);
	SOCKADDR_IN socketAddress;

	SOCKET socket = WSAAccept(this->socket,
							(SOCKADDR*)&socketAddress,
							&socketAddressLength, NULL, 0);

	if (WSAGetLastError() == WSAEINTR)		//Aceept Á¾·á
		return NULL;

	TcpSocket* acceptedSocket = new TcpSocket();
	acceptedSocket->SetSocketHandle(socket);
	acceptedSocket->SetSocketAddress(socketAddress);

	return acceptedSocket;
}