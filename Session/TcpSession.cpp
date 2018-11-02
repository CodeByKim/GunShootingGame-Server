#include "TcpSocket.h"
#include "TcpSession.h"
#include "PacketFactory.h"
#include "Package.h"
#include "Network.h"

TcpSession::TcpSession()
{
	this->streamBuffer = new StreamBuffer();
	this->isConnected = FALSE;
}

TcpSession::~TcpSession()
{
	this->isConnected = FALSE;
	SAFE_DELETE(this->streamBuffer);
}

VOID TcpSession::PushSendPacketToPacketQueue(TcpPacket* packet)
{
	this->sendQueueLocker.Lock();
	this->sendPacketQueue.push(packet);
	this->sendQueueLocker.Unlock();
}

VOID TcpSession::SendPacketInPacketQueue()
{
	vector<TcpPacket*> tempPackets;

	this->sendQueueLocker.Lock();
	for (INT i = 0; i < (INT)this->sendPacketQueue.size(); i++)
	{
		TcpPacket* packet = this->sendPacketQueue.front();
		this->sendPacketQueue.pop();
		this->SendPacket(packet);
		tempPackets.push_back(packet);
	}
	this->sendQueueLocker.Unlock();

	if (!tempPackets.empty())
	{
		this->completedSendQueueLocker.Lock();

		for (INT i = 0; i < (INT)tempPackets.size(); i++)
			this->completedSendPacketQueue.push(tempPackets[i]);

		this->completedSendQueueLocker.Unlock();
	}
}

VOID TcpSession::SendPacket(TcpPacket* packet)
{
	CHAR* packetData = PacketFactory::GetInstance()->SerializePacket(packet);
	this->socket->Send(packetData, packet->header.length);
}

VOID TcpSession::OnReceive(INT readDataLength)
{
	this->streamBuffer->PutStream(this->socket->GetReadDataBuffer(), readDataLength);

	while (PacketFactory::GetInstance()->IsMakePacket(this->streamBuffer))
	{
		TcpPacket* packet = PacketFactory::GetInstance()->DeserializePacket(this->streamBuffer);

		this->recvQueueLocker.Lock();
		this->recvPacketQueue.push(packet);
		this->recvQueueLocker.Unlock();
	}
}

VOID TcpSession::OnSent()
{
	this->completedSendQueueLocker.Lock();
	if (!this->completedSendPacketQueue.empty())
	{
		for (INT i = 0; i < (INT)this->completedSendPacketQueue.size(); i++)
		{
			TcpPacket* packet = this->completedSendPacketQueue.front();
			this->completedSendPacketQueue.pop();
			packet->Reset();
			PacketFactory::GetInstance()->GetPacketPool()->Push(packet);					
		}
	}
	this->completedSendQueueLocker.Unlock();
}

VOID TcpSession::SetConnected()
{
	this->isConnected = TRUE; 
	this->socket->Receive();
}