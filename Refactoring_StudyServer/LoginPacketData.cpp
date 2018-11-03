#include "TcpPacket.h"
#include "LoginPacketData.h"

VOID CLIENTtoSERVER_RegisterUserPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->id);
	packet->stream->ReadString(&this->password);
}

VOID SERVERtoCLIENT_RegisterUserPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->SetHeader((int)protocol, (int)extra);
}

VOID CLIENTtoSERVER_LoginPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->id);
	packet->stream->ReadString(&this->password);
}

VOID SERVERtoCLIENT_LoginPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerKey);
	packet->stream->WriteString(this->playerID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_LoginPacketData::WriteData(wstring playerKey, wstring playerID)
{
	this->playerKey = playerKey;
	this->playerID = playerID;
}