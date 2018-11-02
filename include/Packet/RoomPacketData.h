#pragma once
#include "Library.h"
#include "TcpPacket.h"
#include "Room.h"
#include "RoomPlayer.h"

#pragma region Create Room
class CLIENTtoSERVER_CreateRoomPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring roomName;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_CreateRoomPacketData : public BasePacketData
{
public:
	wstring roomName;
	wstring roomID;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring roomID, wstring roomName);
};
#pragma endregion

#pragma region Create Room Other Player
class SERVERtoCLIENT_CreateRoomOtherPlayerPacketData : public BasePacketData
{
public:
	wstring roomName;
	wstring roomID;
	wstring playerID;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
			VOID WriteData(wstring roomName, wstring roomID, wstring playerID);
};
#pragma endregion

#pragma region Leave Room
class CLIENTtoSERVER_LeaveRoomPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring roomID;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_LeaveRoomPacketData : public BasePacketData
{
public:
	INT roomCount;
	vector<wstring> roomIds;
	vector<wstring> roomNames;
	vector<INT> joinedRoomPlayerCount;
	vector<BOOL> isStart;
	INT playerCount;
	vector<wstring> playerNames;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
			VOID WriteData(INT roomCount, vector<Room*>& rooms, INT playerCount, vector<Player*>& players);
};
#pragma endregion

#pragma region Leave Room Other Player
class SERVERtoCLIENT_LeaveRoomOtherPlayerPacketData : public BasePacketData
{
public:
	wstring playerID;
	wstring roomName;
	wstring roomID;
	INT joinedRoomPlayerCount;		//사실 필요없을수도, 방에서 플레이어가 나오면 기존 수치에서 1 감소하는것과 같거든

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
			VOID WriteData(wstring playerID, wstring roomName, wstring roomID, INT joinedRoomPlayerCount);
};
#pragma endregion

#pragma region Join Room
class CLIENTtoSERVER_JoinRoomPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring roomID;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_JoinRoomPacketData : public BasePacketData
{
public:
	wstring roomID;
	wstring roomName;
	INT joinedPlayerCount;
	vector<wstring> joinedPlayers;
	vector<BOOL> joinedPlayerReadyState;
	vector<BOOL> isLeaders;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
			VOID WriteData(wstring roomID, wstring roomName, INT joinedPlayerCount, vector<RoomPlayer*>& roomPlayers);
};
#pragma endregion

#pragma region Join Room Other Player
class SERVERtoCLIENT_JoinRoomOtherPlayerPacketData : public BasePacketData
{
public:
	wstring playerID;
	wstring roomID;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
			VOID WriteData(wstring playerID, wstring roomID);
};
#pragma endregion

#pragma region Change Room Leader
class SERVERtoCLIENT_ChangeRoomLeaderPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring playerID;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);	
			VOID WriteData(wstring playerKey, wstring playerID);
};
#pragma endregion

#pragma region Remove Room
class SERVERtoCLIENT_RemoveRoomPacketData : public BasePacketData
{
public:
	wstring roomID;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
			VOID WriteData(wstring roomID);
};
#pragma endregion

#pragma region Room Chatting
class CLIENTtoSERVER_RoomChattingPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring roomKey;
	wstring chatString;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_RoomChattingPacketData : public BasePacketData
{
public:
	wstring playerID;
	wstring chatString;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID, wstring chatString);
};
#pragma endregion

#pragma region Request Ready
class CLIENTtoSERVER_ReadyPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring roomKey;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_ReadyPacketData : public BasePacketData
{
public:
	BOOL ready;
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(BOOL ready);
};
#pragma endregion

#pragma region Ready Other Player
class SERVERtoCLIENT_ReadyOtherPlayerPacketData : public BasePacketData
{
public:
	wstring playerID;
	BOOL ready;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID, BOOL ready);
};
#pragma endregion

#pragma region Request Room Start
class CLIENTtoSERVER_RoomStartPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring roomKey;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_RoomStartPacketData : public BasePacketData
{
public:
	wstring gameKey;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring gameKey);
};
#pragma endregion