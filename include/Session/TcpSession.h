#pragma once
#include "Library.h"
#include "NetworkSession.h"
#include "StreamBuffer.h"
#include "TcpPacket.h"

class TcpSession : public NetworkSession
{
private:
	BOOL isConnected;
	StreamBuffer* streamBuffer;
	HANDLE completedQueueFlowEvent;

public:
	TcpSession();
	virtual ~TcpSession();

	virtual VOID SendPacket(TcpPacket* packet);
	virtual VOID OnReceive(INT readDataLength);
	virtual VOID OnSent();
	virtual VOID PushSendPacketToPacketQueue(TcpPacket* packet);
	virtual VOID SendPacketInPacketQueue();
	
	VOID SetConnected();		
};