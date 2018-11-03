#pragma once
#include "Library.h"
#include "RawSocket.h"
#include "TcpPacket.h"
#include "Locker.h"

class NetworkSession
{
protected:
	RawSocket* socket;
	queue<TcpPacket*> sendPacketQueue;
	queue<TcpPacket*> completedSendPacketQueue;
	queue<TcpPacket*> recvPacketQueue;

	Locker sendQueueLocker;
	Locker completedSendQueueLocker;
	Locker recvQueueLocker;

	BOOL isRecvHeartBeat;
public:
	NetworkSession();
	virtual ~NetworkSession();

	virtual VOID SendPacket(TcpPacket* packet) = 0;
	virtual VOID OnReceive(INT readDataLength) = 0;
	virtual VOID OnSent() = 0;
	virtual VOID PushSendPacketToPacketQueue(TcpPacket* packet) = 0;
	virtual VOID SendPacketInPacketQueue() = 0;

	VOID Bind(ULONG address, USHORT port);
	VOID Bind(CHAR* address, USHORT port);

	RawSocket* GetSocket();
	VOID SetSocket(RawSocket* socket);
	queue<TcpPacket*>& GetRecvPacketQueue();
	TcpPacket* GetPacketFromRecvQueue();
	INT GetRecvQueueSize();
	VOID CheckHeartBeat();
	VOID ResetHeartBeat();
	BOOL IsRecvHeartBeat();
	virtual VOID Release();	
};
