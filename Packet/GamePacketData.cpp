#include "TcpPacket.h"
#include "GamePacketData.h"

#pragma region Heart Beat
VOID CLIENTtoSERVER_HeartBeatPacketData::Deserialize(TcpPacket* packet)
{

}

VOID SERVERtoCLIENT_HeartBeatPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->SetHeader((int)protocol, (int)extra);
}
#pragma endregion

#pragma region Game Start
VOID CLIENTtoSERVER_GameStartPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
}

VOID SERVERtoCLIENT_GameStartPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->SetHeader((int)protocol, (int)extra);
}
#pragma endregion

#pragma region Game End
VOID CLIENTtoSERVER_GameEndPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
}

VOID SERVERtoCLIENT_GameEndPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->roomName);
	packet->stream->WriteString(this->roomKey);
	packet->stream->WriteInt(this->joinedPlayerCount);

	for (INT i = 0; i < (int)this->joinedPlayers.size(); i++)
		packet->stream->WriteString(this->joinedPlayers[i]);

	for (INT i = 0; i < (int)this->joinedPlayerReadyState.size(); i++)
		packet->stream->WriteChar(this->joinedPlayerReadyState[i]);

	for (INT i = 0; i < (int)this->isLeaders.size(); i++)
		packet->stream->WriteChar(this->isLeaders[i]);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_GameEndPacketData::WriteData(wstring roomName, wstring roomKey, INT joinedPlayerCount, vector<RoomPlayer*>& roomPlayers)
{
	this->roomName = roomName;
	this->roomKey = roomKey;
	this->joinedPlayerCount = joinedPlayerCount;

	for (INT i = 0; i < this->joinedPlayerCount; i++)
	{
		this->joinedPlayers.push_back(roomPlayers[i]->GetPlayer()->GetPlayerID());
		this->joinedPlayerReadyState.push_back(roomPlayers[i]->GetReadyState());
		this->isLeaders.push_back(roomPlayers[i]->IsLeader());
	}
}
#pragma endregion

#pragma region Other Game Start
VOID SERVERtoCLIENT_OtherGameStartPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->roomName);
	packet->stream->WriteString(this->roomKey);	

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_OtherGameStartPacketData::WriteData(wstring roomName, wstring roomKey)
{
	this->roomName = roomName;
	this->roomKey = roomKey;	
}
#pragma endregion

#pragma region Other Game End
VOID SERVERtoCLIENT_OtherGameEndPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->roomName);
	packet->stream->WriteString(this->roomKey);	

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_OtherGameEndPacketData::WriteData(wstring roomName, wstring roomKey)
{
	this->roomName = roomName;
	this->roomKey = roomKey;	
}
#pragma endregion

#pragma region Game Player Move
VOID CLIENTtoSERVER_GamePlayerMovePacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);

	packet->stream->ReadFloat(&this->playerPosition.x);
	packet->stream->ReadFloat(&this->playerPosition.y);
	packet->stream->ReadFloat(&this->playerPosition.z);

	packet->stream->ReadFloat(&this->playerRotation.x);
	packet->stream->ReadFloat(&this->playerRotation.y);
	packet->stream->ReadFloat(&this->playerRotation.z);
}

VOID SERVERtoHOST_GamePlayerMovePacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{	
	packet->stream->WriteString(this->playerKey);

	packet->stream->WriteFloat(this->playerPosition.x);
	packet->stream->WriteFloat(this->playerPosition.y);
	packet->stream->WriteFloat(this->playerPosition.z);

	packet->stream->WriteFloat(this->playerRotation.x);
	packet->stream->WriteFloat(this->playerRotation.y);
	packet->stream->WriteFloat(this->playerRotation.z);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoHOST_GamePlayerMovePacketData::WriteData(wstring playerKey, Vector3 playerPosition, Vector3 playerRotation)
{
	this->playerKey = playerKey;
	this->playerPosition = playerPosition;
	this->playerRotation = playerRotation;
}

VOID SERVERtoCLIENT_GamePlayerMovePacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);

	packet->stream->WriteFloat(this->playerPosition.x);
	packet->stream->WriteFloat(this->playerPosition.y);
	packet->stream->WriteFloat(this->playerPosition.z);

	packet->stream->WriteFloat(this->playterRotation.x);
	packet->stream->WriteFloat(this->playterRotation.y);
	packet->stream->WriteFloat(this->playterRotation.z);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_GamePlayerMovePacketData::WriteData(wstring playerID, Vector3 playerPosition, Vector3 playerRotation)
{
	this->playerID = playerID;
	this->playerPosition = playerPosition;
	this->playterRotation = playerRotation;
}
#pragma endregion

#pragma region Game Player Attack
VOID CLIENTtoSERVER_GamePlayerAttackPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
}

VOID SERVERtoHOST_GamePlayerAttackPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerKey);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoHOST_GamePlayerAttackPacketData::WriteData(wstring playerKey)
{
	this->playerKey = playerKey;	
}

VOID SERVERtoCLIENT_GamePlayerAttackPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_GamePlayerAttackPacketData::WriteData(wstring playerID)
{
	this->playerID = playerID;	
}
#pragma endregion

#pragma region Game Player Damaged
VOID CLIENTtoSERVER_GamePlayerDamagedPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->playerID);
	packet->stream->ReadInt(&this->hp);
}

VOID SERVERtoCLIENT_GamePlayerDamagedPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);
	packet->stream->WriteInt(this->hp);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_GamePlayerDamagedPacketData::WriteData(wstring playerID, INT hp)
{
	this->playerID = playerID;
	this->hp = hp;
}
#pragma endregion

#pragma region Game Player Die
VOID CLIENTtoSERVER_GamePlayerDiePacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->playerID);
}

VOID SERVERtoCLIENT_GamePlayerDiePacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_GamePlayerDiePacketData::WriteData(wstring playerID)
{
	this->playerID = playerID;
}
#pragma endregion

#pragma region Enemy Spawn
VOID CLIENTtoSERVER_EnemySpawnPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->enemyID);

	packet->stream->ReadFloat(&this->position.x);
	packet->stream->ReadFloat(&this->position.y);
	packet->stream->ReadFloat(&this->position.z);
}

VOID SERVERtoCLIENT_EnemySpawnPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->enemyID);

	packet->stream->WriteFloat(this->position.x);
	packet->stream->WriteFloat(this->position.y);
	packet->stream->WriteFloat(this->position.z);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnemySpawnPacketData::WriteData(wstring enemyID, Vector3 position)
{
	this->enemyID = enemyID;
	this->position = position;
}
#pragma endregion

#pragma region Enemy Move
VOID CLIENTtoSERVER_EnemyMovePacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->enemyID);

	packet->stream->ReadFloat(&this->position.x);
	packet->stream->ReadFloat(&this->position.y);
	packet->stream->ReadFloat(&this->position.z);

	packet->stream->ReadFloat(&this->rotation.x);
	packet->stream->ReadFloat(&this->rotation.y);
	packet->stream->ReadFloat(&this->rotation.z);
}

VOID SERVERtoCLIENT_EnemyMovePacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->enemyID);

	packet->stream->WriteFloat(this->position.x);
	packet->stream->WriteFloat(this->position.y);
	packet->stream->WriteFloat(this->position.z);

	packet->stream->WriteFloat(this->rotation.x);
	packet->stream->WriteFloat(this->rotation.y);
	packet->stream->WriteFloat(this->rotation.z);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnemyMovePacketData::WriteData(wstring enemyID, Vector3 position, Vector3 rotation)
{
	this->enemyID = enemyID;
	this->position = position;
	this->rotation = rotation;
}
#pragma endregion

#pragma region Enemy Damaged
VOID CLIENTtoSERVER_EnemyDamagedPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->enemyID);
}

VOID SERVERtoCLIENT_EnemyDamagedPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->enemyID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnemyDamagedPacketData::WriteData(wstring enemyID)
{
	this->enemyID = enemyID;
}
#pragma endregion

#pragma region Enemy Attack
VOID CLIENTtoSERVER_EnemyAttackPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->enemyID);
}

VOID SERVERtoCLIENT_EnemyAttackPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->enemyID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnemyAttackPacketData::WriteData(wstring enemyID)
{
	this->enemyID = enemyID;	
}
#pragma endregion

#pragma region Enemy Die
VOID CLIENTtoSERVER_EnemyDiePacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->enemyID);	
}

VOID SERVERtoCLIENT_EnemyDiePacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->enemyID);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_EnemyDiePacketData::WriteData(wstring enemyID)
{
	this->enemyID = enemyID;
}
#pragma endregion

#pragma region Potion Spawn
VOID CLIENTtoSERVER_PotionSpawnPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->potionID);

	packet->stream->ReadFloat(&this->position.x);
	packet->stream->ReadFloat(&this->position.y);
	packet->stream->ReadFloat(&this->position.z);
}

VOID SERVERtoCLIENT_PotionSpawnPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->potionID);

	packet->stream->WriteFloat(this->position.x);
	packet->stream->WriteFloat(this->position.y);
	packet->stream->WriteFloat(this->position.z);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_PotionSpawnPacketData::WriteData(wstring potionID, Vector3 position)
{
	this->potionID = potionID;
	this->position = position;
}
#pragma endregion

#pragma region Potion Pickup
VOID CLIENTtoSERVER_PotionPickupPacketData::Deserialize(TcpPacket* packet)
{
	packet->stream->ReadString(&this->playerKey);
	packet->stream->ReadString(&this->gameKey);
	packet->stream->ReadString(&this->playerID);
	packet->stream->ReadString(&this->potionID);
	packet->stream->ReadInt(&this->hp);
}

VOID SERVERtoCLIENT_PotionPickupPacketData::Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra)
{
	packet->stream->WriteString(this->playerID);
	packet->stream->WriteString(this->potionID);
	packet->stream->WriteInt(this->hp);

	packet->SetHeader((int)protocol, (int)extra);
}

VOID SERVERtoCLIENT_PotionPickupPacketData::WriteData(wstring playerID, wstring potionID, INT hp)
{
	this->playerID = playerID;
	this->potionID = potionID;
	this->hp = hp;
}
#pragma endregion