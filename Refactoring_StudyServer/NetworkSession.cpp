#include "NetworkSession.h"

NetworkSession::NetworkSession()
{
	this->recvQueueLocker.Initialize();
	this->sendQueueLocker.Initialize();
	this->completedSendQueueLocker.Initialize();
	this->isRecvHeartBeat = TRUE;
}

NetworkSession::~NetworkSession()
{
	
}

VOID NetworkSession::Bind(ULONG address, USHORT port)
{
	this->socket->SetSocketAddress(address, port);
	this->socket->Bind();	
	Utils::GetInstance()->LogTrace(L"ACCEPTOR", L"바인딩 완료");
}

VOID NetworkSession::Bind(CHAR* address, USHORT port)
{
	SOCKADDR_IN socketAddress = this->socket->GetSocketAddress();	
	this->socket->SetSocketAddress(inet_pton(AF_INET, address, &socketAddress.sin_addr.S_un.S_addr), port);
	this->socket->Bind();	
	Utils::GetInstance()->LogTrace(L"ACCEPTOR", L"바인딩 완료");
}

RawSocket* NetworkSession::GetSocket()
{
	return this->socket;
}

VOID NetworkSession::SetSocket(RawSocket* socket)
{
	this->socket = socket;
}

queue<TcpPacket*>& NetworkSession::GetRecvPacketQueue()
{
	return this->recvPacketQueue;
}

TcpPacket* NetworkSession::GetPacketFromRecvQueue()
{
	this->recvQueueLocker.Lock();
	TcpPacket* packet = this->recvPacketQueue.front();
	this->recvPacketQueue.pop();
	this->recvQueueLocker.Unlock();
	return packet;
}

INT NetworkSession::GetRecvQueueSize()
{
	this->recvQueueLocker.Lock();
	INT size = this->recvPacketQueue.size();
	this->recvQueueLocker.Unlock();
	return size;
}

BOOL NetworkSession::IsRecvHeartBeat()
{
	return this->isRecvHeartBeat;
}

VOID NetworkSession::ResetHeartBeat()
{
	this->isRecvHeartBeat = FALSE;
}

VOID NetworkSession::CheckHeartBeat()
{
	this->isRecvHeartBeat = TRUE;
}

VOID NetworkSession::Release()
{
	// 각종 Delete 작업...
}