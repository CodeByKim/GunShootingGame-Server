#include "Player.h"
#include "PlayerManager.h"
#include "NetworkSession.h"
#include "TcpPacket.h"

VOID PlayerManager::Initialize()
{
	this->players.clear();
	this->senderThreadIndex = 0;
	this->locker.Initialize();

	for (INT i = 0; i < PROCESSOR_COUNT; i++)
	{
		vector<Player*>	players;
		this->sequentialPlayers.push_back(players);
	}	
	Utils::GetInstance()->LogTrace(L"PLAYER MANAGER", L"초기화 완료");
}

VOID PlayerManager::Release()
{
	this->players.clear();
	Utils::GetInstance()->LogTrace(L"PLAYER MANAGER", L"리소스 해제 완료");
}

BOOL PlayerManager::FindPlayer(wstring playerKey)
{
	if (this->players.count(playerKey))
		return TRUE;

	return FALSE;
}

Player* PlayerManager::FindPlayerFromKey(wstring playerKey)
{
	if (this->players.find(playerKey) != this->players.end()) 
	{
		Player* findPlayer = this->players[playerKey];
		return findPlayer;
	}
	else 
	{
		return NULL;
	}	
}

BOOL PlayerManager::OverlapedLoginCheck(wstring playerID)
{	
	map<wstring, Player*>::iterator iter;
	
	for (iter = this->players.begin(); iter != this->players.end(); iter++)
	{
		if (iter->second->GetPlayerID() == playerID && iter->second->IsEnterToGame())
			return FALSE;
	}

	return TRUE;
}

VOID PlayerManager::AddPlayer(Player* player)
{
	this->players.insert(make_pair(player->GetKey(), player));

	if (this->senderThreadIndex > PROCESSOR_COUNT - 1)
		this->senderThreadIndex = 0;

	this->sequentialPlayers[this->senderThreadIndex].push_back(player);
	this->senderThreadIndex += 1;
}

VOID PlayerManager::RemovePlayer(Player* player)
{
	this->players[player->GetKey()]->Release();
	this->players.erase(player->GetKey());

	for (INT i = 0; i < PROCESSOR_COUNT; i++)
	{
		for (INT j = 0; j < (INT)this->sequentialPlayers[i].size(); j++)
		{
			if (this->sequentialPlayers[i][j]->GetKey() == player->GetKey())
				this->sequentialPlayers[i].erase(this->sequentialPlayers[i].begin() + j);
		}
	}
}

INT PlayerManager::GetPlayerCount()
{
	return (INT)this->players.size();
}

map<wstring, Player*>& PlayerManager::GetPlayers()
{
	return this->players;
}

VOID PlayerManager::RegisterRoomPlayer(Player* player, RoomPlayer* roomPlayer)
{
	this->roomPlayerMap.insert(make_pair(player, roomPlayer));
}

RoomPlayer* PlayerManager::GetRoomPlayer(Player* player)
{
	return this->roomPlayerMap[player];
}

VOID PlayerManager::RemoveRoomPlayer(Player* player)
{
	this->roomPlayerMap.erase(player);
}

VOID PlayerManager::RegisterGamePlayer(Player* player, GamePlayer* GamePlayer)
{
	this->gamePlayerMap.insert(make_pair(player, GamePlayer));
}

GamePlayer* PlayerManager::GetGamePlayer(Player* player)
{
	return this->gamePlayerMap[player];
}

VOID PlayerManager::RemoveGamePlayer(Player* player)
{
	this->gamePlayerMap.erase(player);
}

VOID PlayerManager::SendPacketQueue(INT index)
{
	vector<Player*> players = this->sequentialPlayers[index];		

	for (INT i = 0; i < (INT)players.size(); i++)
		players[i]->GetNetworkSession()->SendPacketInPacketQueue();
}