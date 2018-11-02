#pragma once
#include "Library.h"
#include "TcpPacket.h"
#include "StreamBuffer.h"
#include "Locker.h"

class PacketPool
{
private:
	stack<TcpPacket*> pool;
	Locker locker;

public:
	VOID Initialize();
	VOID Release();

	TcpPacket* Pop();
	VOID Push(TcpPacket* packet);
	BOOL IsEmpty();
	INT GetCount();
};

class PacketFactory : public Singleton<PacketFactory>
{
private:
	PacketPool* packetPool;
	
	
public:
	VOID Initialize();
	VOID Release();

	PacketPool* GetPacketPool();

	BOOL IsMakePacket(StreamBuffer* streamBuffer);

	TcpPacketHeader DeserializeHeader(StreamBuffer* streamBuffer);
	TcpPacket* DeserializePacket(StreamBuffer* streamBuffer);

	CHAR* SerializeHeader(TcpPacket* packet);
	CHAR* SerializePacket(TcpPacket* packet);
};