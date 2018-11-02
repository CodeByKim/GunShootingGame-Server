#include "TcpPacket.h"
#include "TestPacketData.h"

VOID CLIENTtoSERVER_KoreanStringPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->testString1);
	packet->stream->ReadString(&this->testString2);
}

VOID SERVERtoCLIENT_KoreanStringPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->testString1);
	packet->stream->WriteString(this->testString2);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_KoreanStringPacketData::WriteData(wstring testString1, wstring testString2)
{
	this->testString1 = testString1;
	this->testString2 = testString2;
}