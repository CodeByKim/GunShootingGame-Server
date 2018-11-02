#include "TcpPacket.h"
#include "LobbyPacketData.h"

VOID CLIENTtoSERVER_EnterLobbyPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
}

VOID SERVERtoCLIENT_EnterLobbyPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteInt(this->roomCount);

	for (INT i = 0; i < (int)this->roomIds.size(); i++)
		packet->stream->WriteString(this->roomIds[i]);

	for (INT i = 0; i < (int)this->roomNames.size(); i++)
		packet->stream->WriteString(this->roomNames[i]);

	for (INT i = 0; i < (int)this->joinedRoomPlayerCount.size(); i++)
		packet->stream->WriteInt(this->joinedRoomPlayerCount[i]);

	for (INT i = 0; i < (int)this->isStart.size(); i++)
		packet->stream->WriteChar(this->isStart[i]);

	packet->stream->WriteInt(this->playerCount);

	for (INT i = 0; i < (int)this->playerNames.size(); i++)
		packet->stream->WriteString(this->playerNames[i]);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnterLobbyPacketData::WriteData(INT roomCount, vector<Room*>& rooms, INT playerCount, vector<Player*>& players)
{
	this->roomCount = roomCount;

	for (INT i = 0; i < roomCount; i++)
		this->roomIds.push_back(rooms[i]->GetRoomID());

	for (INT i = 0; i < roomCount; i++)
		this->roomNames.push_back(rooms[i]->GetName());

	for (INT i = 0; i < roomCount; i++)
		this->joinedRoomPlayerCount.push_back(rooms[i]->GetJoinedPlayerCount());

	for (INT i = 0; i < roomCount; i++)
		this->isStart.push_back(rooms[i]->IsStartGame());

	this->playerCount = playerCount;

	for (INT i = 0; i < playerCount; i++)
		this->playerNames.push_back(players[i]->GetPlayerID());
}

VOID CLIENTtoSERVER_LeaveLobbyPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
}

VOID SERVERtoCLIENT_LeaveLobbyPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnterLobbyOtherPlayerPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerName);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnterLobbyOtherPlayerPacketData::WriteData(wstring playerName)
{
	this->playerName = playerName;
}

VOID SERVERtoCLIENT_LeaveLobbyOtherPlayerPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerName);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_LeaveLobbyOtherPlayerPacketData::WriteData(wstring playerName)
{
	this->playerName = playerName;
}

VOID CLIENTtoSERVER_LobbyChattingPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->chatString);
}

VOID SERVERtoCLIENT_LobbyChattingPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);
	packet->stream->WriteString(this->chatString);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_LobbyChattingPacketData::WriteData(wstring playerID, wstring chatString)
{
	this->playerID = playerID;
	this->chatString = chatString;
}