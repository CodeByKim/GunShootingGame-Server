#include "PacketFactory.h"
#include "MemoryStream.h"
#include "ProtocolManager.h"

VOID PacketPool::Initialize()
{
	this->locker.Initialize();

	for (int i = 0; i < MAX_PACKET_COUNT; i++)
	{
		TcpPacket* packet = new TcpPacket();
		packet->Initialize();
		this->pool.push(packet);
	}
}

VOID PacketPool::Release()
{
	for (int i = 0; i < MAX_PACKET_COUNT; i++)
	{
		TcpPacket* packet = this->pool.top();
		this->pool.pop();
		SAFE_DELETE(packet);
	}
}

TcpPacket* PacketPool::Pop()
{
	this->locker.Lock();
	if (this->pool.empty())
	{
		for (INT i = 0; i < 100; i++)
		{
			TcpPacket* packet = new TcpPacket();
			packet->Initialize();
			this->pool.push(packet);
		}				
	}

	TcpPacket* packet = this->pool.top();
	this->pool.pop();
	this->locker.Unlock();

	return packet;
}

VOID PacketPool::Push(TcpPacket* packet)
{
	this->locker.Lock();
	this->pool.push(packet);
	this->locker.Unlock();
}

BOOL PacketPool::IsEmpty()
{
	this->locker.Lock();
	return this->pool.empty();
	this->locker.Unlock();
}

INT PacketPool::GetCount()
{
	this->locker.Lock();
	INT count = (INT)this->pool.size();
	this->locker.Unlock();

	return count;
}

VOID PacketFactory::Initialize()
{
	this->packetPool = new PacketPool();
	this->packetPool->Initialize();		
	Utils::GetInstance()->LogTrace(L"PACKET FACTORY", L"초기화 완료");
}

VOID PacketFactory::Release()
{
	this->packetPool->Release();
	SAFE_DELETE(this->packetPool);
	Utils::GetInstance()->LogTrace(L"PACKET FACTORY", L"리소스 해제 완료");
}

PacketPool* PacketFactory::GetPacketPool()
{
	return this->packetPool;
}

BOOL PacketFactory::IsMakePacket(StreamBuffer* streamBuffer)
{
	if (streamBuffer->GetRemainLength() >= PACKET_HEADER_LENGTH)
	{
		TcpPacketHeader header = DeserializeHeader(streamBuffer);

		if (header.length > MAX_BUFFER_LENGTH || header.length <= 0)
		{
			// 패킷의 길이가 이상하면 잘못된 패킷
			streamBuffer->SetRemainLength(0);
			return FALSE;
		}

		if (streamBuffer->GetRemainLength() >= header.length)
			return TRUE;
	}

	return FALSE;
}

TcpPacketHeader PacketFactory::DeserializeHeader(StreamBuffer* streamBuffer)
{
	TcpPacketHeader header;
	CopyMemory(&header.length, streamBuffer->GetBuffer(), sizeof(INT));
	CopyMemory(&header.protocol, streamBuffer->GetBuffer() + sizeof(INT), sizeof(INT));
	CopyMemory(&header.extra, streamBuffer->GetBuffer() + sizeof(INT) + sizeof(INT), sizeof(INT));
	return header;
}

TcpPacket* PacketFactory::DeserializePacket(StreamBuffer* streamBuffer)
{
	TcpPacketHeader packetHeader = DeserializeHeader(streamBuffer);
	INT packetDataLength = packetHeader.length - PACKET_HEADER_LENGTH;

	TcpPacket* packet = new TcpPacket();
	packet->Initialize(streamBuffer->GetBuffer() + PACKET_HEADER_LENGTH, packetDataLength);
	packet->SetHeader(packetHeader);
	
	if (streamBuffer->GetRemainLength() - packetHeader.length > 0)
	{
		MoveMemory(streamBuffer->GetBuffer(),
			streamBuffer->GetBuffer() + packetHeader.length,
			streamBuffer->GetRemainLength() - packetHeader.length);
	}

	INT remainLength = streamBuffer->GetRemainLength();
	remainLength -= packetHeader.length;
	streamBuffer->SetRemainLength(remainLength);

	if (streamBuffer->GetRemainLength() <= 0)
	{
		remainLength = 0;
		ZeroMemory(streamBuffer->GetBuffer(), sizeof(streamBuffer->GetBuffer()));
	}

	return packet;
}

CHAR* PacketFactory::SerializeHeader(TcpPacket* packet)
{
	CHAR* data = new CHAR[PACKET_HEADER_LENGTH];
	TcpPacketHeader header = packet->header;
	CopyMemory(data, &header.length, sizeof(INT));
	CopyMemory(data + sizeof(INT), &header.protocol, sizeof(INT));
	CopyMemory(data + sizeof(INT) + sizeof(INT), &header.extra, sizeof(INT));

	return data;
}

CHAR* PacketFactory::SerializePacket(TcpPacket* packet)
{
	INT length = packet->header.length;
	INT packetDataLength = length - PACKET_HEADER_LENGTH;

	CHAR* serializedHeader = SerializeHeader(packet);

	CHAR* serializedPacket = new CHAR[packet->header.length];
	CopyMemory(serializedPacket, serializedHeader, PACKET_HEADER_LENGTH);

	if(packetDataLength != 0)
		CopyMemory(serializedPacket + PACKET_HEADER_LENGTH, packet->stream->GetBuffer(), packetDataLength);

	return serializedPacket;
}