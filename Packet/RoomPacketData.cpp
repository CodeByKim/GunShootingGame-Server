#include "TcpPacket.h"
#include "RoomPacketData.h"

#pragma region Create Room
VOID CLIENTtoSERVER_CreateRoomPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->roomName);
}

VOID SERVERtoCLIENT_CreateRoomPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->roomName);
	packet->stream->WriteString(this->roomID);	

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_CreateRoomPacketData::WriteData(wstring roomID, wstring roomName)
{
	this->roomID = roomID;
	this->roomName = roomName;
}
#pragma endregion

#pragma region Create Room Other Player
VOID SERVERtoCLIENT_CreateRoomOtherPlayerPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->roomName);
	packet->stream->WriteString(this->roomID);	
	packet->stream->WriteString(this->playerID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_CreateRoomOtherPlayerPacketData::WriteData(wstring roomName, wstring roomID, wstring playerID)
{
	this->roomName = roomName;
	this->roomID = roomID;
	this->playerID = playerID;
}
#pragma endregion

#pragma region Leave Room
VOID CLIENTtoSERVER_LeaveRoomPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->roomID);
}

VOID SERVERtoCLIENT_LeaveRoomPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
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

VOID SERVERtoCLIENT_LeaveRoomPacketData::WriteData(INT roomCount, vector<Room*>& rooms, INT playerCount, vector<Player*>& players)
{
	this->roomCount = roomCount;

	for (INT i = 0; i < this->roomCount; i++)
	{
		this->roomIds.push_back(rooms[i]->GetRoomID());
		this->roomNames.push_back(rooms[i]->GetName());
		this->joinedRoomPlayerCount.push_back(rooms[i]->GetJoinedPlayerCount());
		this->isStart.push_back(rooms[i]->IsStartGame());
	}

	this->playerCount = playerCount;

	for (INT i = 0; i < this->playerCount; i++)
		this->playerNames.push_back(players[i]->GetPlayerID());
}
#pragma endregion

#pragma region Leave Room Other Player
VOID SERVERtoCLIENT_LeaveRoomOtherPlayerPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);
	packet->stream->WriteString(this->roomName);
	packet->stream->WriteString(this->roomID);
	packet->stream->WriteInt(this->joinedRoomPlayerCount);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_LeaveRoomOtherPlayerPacketData::WriteData(wstring playerID, wstring roomName, wstring roomID, INT joinedRoomPlayerCount)
{
	this->playerID = playerID;
	this->roomName = roomName;
	this->roomID = roomID;
	this->joinedRoomPlayerCount = joinedRoomPlayerCount;
}
#pragma endregion

#pragma region Join Room
VOID CLIENTtoSERVER_JoinRoomPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->roomID);
}

VOID SERVERtoCLIENT_JoinRoomPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->roomID);
	packet->stream->WriteString(this->roomName);
	packet->stream->WriteInt(this->joinedPlayerCount);

	for (INT i = 0; i < (int)this->joinedPlayers.size(); i++)
		packet->stream->WriteString(this->joinedPlayers[i]);

	for (INT i = 0; i < (int)this->joinedPlayerReadyState.size(); i++)
		packet->stream->WriteChar(this->joinedPlayerReadyState[i]);

	for (INT i = 0; i < (int)this->joinedPlayerReadyState.size(); i++)
		packet->stream->WriteChar(this->isLeaders[i]);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_JoinRoomPacketData::WriteData(wstring roomID, wstring roomName, INT joinedPlayerCount, vector<RoomPlayer*>& roomPlayers)
{
	this->roomID = roomID;
	this->roomName = roomName;
	this->joinedPlayerCount = joinedPlayerCount;

	for (INT i = 0; i < this->joinedPlayerCount; i++)
	{
		this->joinedPlayers.push_back(roomPlayers[i]->GetPlayer()->GetPlayerID());
		this->joinedPlayerReadyState.push_back(roomPlayers[i]->GetReadyState());
		this->isLeaders.push_back(roomPlayers[i]->IsLeader());
	}
}
#pragma endregion

#pragma region Join Room Other Player
VOID SERVERtoCLIENT_JoinRoomOtherPlayerPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);
	packet->stream->WriteString(this->roomID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_JoinRoomOtherPlayerPacketData::WriteData(wstring playerID, wstring roomID)
{
	this->playerID = playerID;
	this->roomID = roomID;
}
#pragma endregion

#pragma region Change Room Leader
VOID SERVERtoCLIENT_ChangeRoomLeaderPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerKey);
	packet->stream->WriteString(this->playerID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_ChangeRoomLeaderPacketData::WriteData(wstring playerKey, wstring playerID)
{
	this->playerKey = playerKey;
	this->playerID = playerID;
}
#pragma endregion

#pragma region Remove Room
VOID SERVERtoCLIENT_RemoveRoomPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->roomID);	

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_RemoveRoomPacketData::WriteData(wstring roomID)
{
	this->roomID = roomID;
}
#pragma endregion

#pragma region Room Chatting
VOID CLIENTtoSERVER_RoomChattingPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->roomKey);
	packet->stream->ReadString(&this->chatString);
}

VOID SERVERtoCLIENT_RoomChattingPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);
	packet->stream->WriteString(this->chatString);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_RoomChattingPacketData::WriteData(wstring playerID, wstring chatString)
{
	this->playerID = playerID;
	this->chatString = chatString;
}
#pragma endregion

#pragma region Request Ready
VOID CLIENTtoSERVER_ReadyPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->roomKey);
}

VOID SERVERtoCLIENT_ReadyPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteChar(this->ready);
	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_ReadyPacketData::WriteData(BOOL ready)
{	
	this->ready = ready;
}
#pragma endregion

#pragma region Ready Other Player
VOID SERVERtoCLIENT_ReadyOtherPlayerPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);
	packet->stream->WriteChar(this->ready);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_ReadyOtherPlayerPacketData::WriteData(wstring playerID, BOOL ready)
{
	this->playerID = playerID;
	this->ready = ready;
}
#pragma endregion

#pragma region Request Start
VOID CLIENTtoSERVER_RoomStartPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->roomKey);
}

VOID SERVERtoCLIENT_RoomStartPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->gameKey);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_RoomStartPacketData::WriteData(wstring gameKey)
{
	this->gameKey = gameKey;
}
#pragma endregion