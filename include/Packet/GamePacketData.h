#pragma once
#include "Library.h"
#include "TcpPacket.h"
#include "Game.h"
#include "Player.h"

#pragma region Heart Beat
class CLIENTtoSERVER_HeartBeatPacketData : public BasePacketData
{
public:
	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_HeartBeatPacketData : public BasePacketData
{
public:
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
};
#pragma endregion

#pragma region Game Start
class CLIENTtoSERVER_GameStartPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_GameStartPacketData : public BasePacketData
{
public:
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
};
#pragma endregion

#pragma region Game End
class CLIENTtoSERVER_GameEndPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_GameEndPacketData : public BasePacketData
{
public:
	wstring roomName;
	wstring roomKey;
	INT joinedPlayerCount;
	vector<wstring> joinedPlayers;
	vector<BOOL> joinedPlayerReadyState;
	vector<BOOL> isLeaders;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring roomName, wstring roomKey, INT joinedPlayerCount, vector<RoomPlayer*>& roomPlayers);
};
#pragma endregion

#pragma region Other Game Start
class SERVERtoCLIENT_OtherGameStartPacketData : public BasePacketData
{
public:
	wstring roomName;
	wstring roomKey;	

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring roomName, wstring roomKey);
};
#pragma endregion

#pragma region Other Game End
class SERVERtoCLIENT_OtherGameEndPacketData : public BasePacketData
{
public:
	wstring roomName;
	wstring roomKey;	

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring roomName, wstring roomKey);
};
#pragma endregion

#pragma region Game Player Move
class CLIENTtoSERVER_GamePlayerMovePacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	Vector3 playerPosition;
	Vector3 playerRotation;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoHOST_GamePlayerMovePacketData : public BasePacketData
{
public:
	wstring playerKey;
	Vector3 playerPosition;
	Vector3 playerRotation;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerKey, Vector3 playerPosition, Vector3 playerRotation);
};

class SERVERtoCLIENT_GamePlayerMovePacketData : public BasePacketData
{
public:
	wstring playerID;
	Vector3 playerPosition;
	Vector3 playterRotation;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID, Vector3 playerPosition, Vector3 playerRotation);
};
#pragma endregion

#pragma region Game Player Attack
class CLIENTtoSERVER_GamePlayerAttackPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;	

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoHOST_GamePlayerAttackPacketData : public BasePacketData
{
public:
	wstring playerKey;	

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerKey);
};

class SERVERtoCLIENT_GamePlayerAttackPacketData : public BasePacketData
{
public:
	wstring playerID;	

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID);
};
#pragma endregion

#pragma region Game Player Damaged
class CLIENTtoSERVER_GamePlayerDamagedPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring playerID;
	INT hp;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_GamePlayerDamagedPacketData : public BasePacketData
{
public:
	wstring playerID;
	INT hp;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID, INT hp);
};
#pragma endregion

#pragma region Game Player Die
class CLIENTtoSERVER_GamePlayerDiePacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring playerID;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_GamePlayerDiePacketData : public BasePacketData
{
public:
	wstring playerID;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID);
};
#pragma endregion

#pragma region Enemy Spawn
class CLIENTtoSERVER_EnemySpawnPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring enemyID;

	Vector3 position;	

	virtual VOID Deserialize(TcpPacket* packet);	
};

class SERVERtoCLIENT_EnemySpawnPacketData : public BasePacketData
{
public:
	wstring enemyID;
	Vector3 position;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring enemyID, Vector3 position);
};
#pragma endregion

#pragma region Enemy Move
class CLIENTtoSERVER_EnemyMovePacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring enemyID;

	Vector3 position;
	Vector3 rotation;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_EnemyMovePacketData : public BasePacketData
{
public:
	wstring enemyID;
	Vector3 position;
	Vector3 rotation;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring enemyID, Vector3 position, Vector3 rotation);
};
#pragma endregion

#pragma region Enemy Damaged
class CLIENTtoSERVER_EnemyDamagedPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring enemyID;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_EnemyDamagedPacketData : public BasePacketData
{
public:
	wstring enemyID;
	
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring enemyID);
};
#pragma endregion

#pragma region Enemy Attack
class CLIENTtoSERVER_EnemyAttackPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring enemyID;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_EnemyAttackPacketData : public BasePacketData
{
public:
	wstring enemyID;
	
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring enemyID);
};
#pragma endregion

#pragma region Enemy Die
class CLIENTtoSERVER_EnemyDiePacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring enemyID;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_EnemyDiePacketData : public BasePacketData
{
public:
	wstring enemyID;
	
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring enemyID);
};
#pragma endregion

#pragma region Potion Spawn
class CLIENTtoSERVER_PotionSpawnPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring potionID;

	Vector3 position;

	virtual VOID Deserialize(TcpPacket* packet);

};

class SERVERtoCLIENT_PotionSpawnPacketData : public BasePacketData
{
public:
	wstring potionID;
	Vector3 position;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring potionID, Vector3 position);
};
#pragma endregion

#pragma region Potion Pickup
class CLIENTtoSERVER_PotionPickupPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring gameKey;
	wstring playerID;
	wstring potionID;
	INT hp;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_PotionPickupPacketData : public BasePacketData
{
public:
	wstring playerID;
	wstring potionID;
	INT hp;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring playerID, wstring potionID, INT hp);
};
#pragma endregion