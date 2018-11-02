#include "Room.h"
#include "Lobby.h"
#include "PlayerManager.h"
#include "PacketFactory.h"

VOID Lobby::Initialize()
{
	this->players.clear();	
	Utils::GetInstance()->LogTrace(L"LOBBY", L"초기화 완료");
}

VOID Lobby::Release()
{
	this->players.clear();
}

VOID Lobby::EnterPlayer(Player* player)
{
	this->players.push_back(player);	
	player->SetState(PLAYER_STATE::LOBBY);
}

VOID Lobby::LeavePlayer(Player* player)
{
	for (INT i = 0; i < (INT)this->players.size(); i++)
	{
		if (this->players[i]->GetKey() == player->GetKey())
		{
			this->players[i]->Release();
			this->players[i]->SetState(PLAYER_STATE::LOGIN);
			this->players.erase(this->players.begin() + i);			
		}
	}	
}

VOID Lobby::RemovePlayer(Player* player)
{
	for (INT i = 0; i < (INT)this->players.size(); i++)
	{
		if (this->players[i]->GetKey() == player->GetKey())
		{
			this->players.erase(this->players.begin() + i);			
		}
	}
}

INT Lobby::GetPlayerCount()
{
	return (INT)this->players.size();
}

vector<Player*>& Lobby::GetPlayers()
{
	return this->players;
}