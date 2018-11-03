#pragma once
#include "Library.h"
#include "TcpPacket.h"
#include "Room.h"
#include "Player.h"

#pragma region Enter Lobby
class CLIENTtoSERVER_EnterLobbyPacketData : public BasePacketData
{
public:
	wstring playerKey;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_EnterLobbyPacketData : public BasePacketData
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

#pragma region Leave Lobby
class CLIENTtoSERVER_LeaveLobbyPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_LeaveLobbyPacketData : public BasePacketData
{
public:	 
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
};
#pragma endregion

#pragma region Enter Lobby Other Player
class SERVERtoCLIENT_EnterLobbyOtherPlayerPacketData : public BasePacketData
{
public:
	wstring playerName;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerName);
};
#pragma endregion

#pragma region Leave Lobby Other Player
class SERVERtoCLIENT_LeaveLobbyOtherPlayerPacketData : public BasePacketData
{
public:
	wstring playerName;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerName);
};
#pragma endregion

#pragma region Lobby Chatting
class CLIENTtoSERVER_LobbyChattingPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring chatString;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_LobbyChattingPacketData : public BasePacketData
{
public:
	wstring playerID;
	wstring chatString;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID, wstring chatString);
};
#pragma endregion