#include "TcpPacket.h"

TcpPacket::~TcpPacket()
{
	SAFE_DELETE(this->stream);
}

VOID TcpPacket::SetHeader(INT protocol, INT extra)
{
	TcpPacketHeader header;
	header.length = PACKET_HEADER_LENGTH + this->stream->GetOffset();
	header.protocol = protocol;
	header.extra = extra;
	this->header = header;
}

VOID TcpPacket::SetHeader(TcpPacketHeader header)
{
	this->header = header;
}

VOID TcpPacket::Initialize()
{
	CHAR* buffer = new CHAR[MAX_PACKET_DATA_LENGTH];
	this->stream = new MemoryStream(buffer, MAX_PACKET_DATA_LENGTH);
}

VOID TcpPacket::Initialize(CHAR* buffer, INT length)
{
	if (length != 0)
		this->stream = new MemoryStream(buffer, length);
	else
		this->stream = NULL;
}

VOID TcpPacket::Reset()
{	
	SAFE_DELETE(this->stream);
	CHAR* buffer = new CHAR[MAX_PACKET_DATA_LENGTH];
	this->stream = new MemoryStream(buffer, MAX_PACKET_DATA_LENGTH);
}