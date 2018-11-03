#include "Room.h"
#include "NetworkSession.h"
#include "Utils.h"
#include "PlayerManager.h"
#include "Lobby.h"
#include "RoomManager.h"

Room::Room(wstring roomName, Player* leader)
{
	RoomPlayer* roomPlayer = new RoomPlayer(leader);
	roomPlayer->SetLeader();
	PlayerManager::GetInstance()->RegisterRoomPlayer(leader, roomPlayer);
	RoomManager::GetInstance()->MapRoomToRoomPlayer(roomPlayer, this);

	this->roomName = roomName;
	this->joinedPlayer.push_back(roomPlayer);
	this->roomLeader = roomPlayer;
	this->roomID = Utils::GetInstance()->GenerateUUID();
	this->joinedPlayerCount = 1;
	this->isStartGame = FALSE;
}

Room::~Room()
{

}

VOID Room::Release()
{
	this->joinedPlayer.clear();
	this->roomLeader = NULL;	
}

VOID Room::Join(Player* player)
{
	if (this->joinedPlayerCount >= 4)
		return;

	RoomPlayer* roomPlayer = new RoomPlayer(player);
	roomPlayer->SetIndex(this->joinedPlayer.size() - 1);
	PlayerManager::GetInstance()->RegisterRoomPlayer(player, roomPlayer);
	RoomManager::GetInstance()->MapRoomToRoomPlayer(roomPlayer, this);

	this->joinedPlayer.push_back(roomPlayer);
	this->joinedPlayerCount += 1;
}

VOID Room::Leave(RoomPlayer* roomPlayer)
{
	for (INT i = 0; i < (INT)this->joinedPlayer.size(); i++)
	{
		if (this->joinedPlayer[i]->GetPlayer()->GetKey() == roomPlayer->GetPlayer()->GetKey())
		{
			this->joinedPlayer.erase(this->joinedPlayer.begin() + i);
		}
	}

	this->joinedPlayerCount -= 1;
	PlayerManager::GetInstance()->RemoveRoomPlayer(roomPlayer->GetPlayer());
	RoomManager::GetInstance()->RemoveJoinRoom(roomPlayer);
}

VOID Room::Ready(RoomPlayer* roomPlayer, BOOL readyState)
{
	if (readyState)
		roomPlayer->SetReady();
	else
		roomPlayer->CancleReady();
}

VOID Room::EndGame()
{
	this->isStartGame = FALSE;

	for (INT i = 0; i < (INT)this->joinedPlayer.size(); i++)
	{		
		joinedPlayer[i]->Release();
		PlayerManager::GetInstance()->RegisterRoomPlayer(joinedPlayer[i]->GetPlayer(), joinedPlayer[i]);
	}
}

Player* Room::ChangeLeader()
{
	RoomPlayer* leader = this->joinedPlayer.front();	
	this->joinedPlayer.erase(this->joinedPlayer.begin()+0);
	this->joinedPlayer.push_back(leader);

	for (INT i = 0; i < (INT)this->joinedPlayer.size(); i++)
	{
		joinedPlayer[i]->SetIndex(i);
	}

	this->roomLeader = this->joinedPlayer[0];
	roomLeader->SetLeader();
	return this->roomLeader->GetPlayer();
}

BOOL Room::IsRoomLeader(Player* player)
{
	if (this->roomLeader->GetPlayer()->GetKey() == player->GetKey())
		return TRUE;

	return FALSE;
}

INT Room::GetJoinedPlayerCount()
{
	return this->joinedPlayerCount;
}

wstring Room::GetName()
{
	return this->roomName;
}

wstring Room::GetRoomID()
{
	return this->roomID;
}

vector<RoomPlayer*>& Room::GetJoinedPlayers()
{
	return this->joinedPlayer;
}

RoomPlayer* Room::GetLeader()
{
	return this->roomLeader;
}

BOOL Room::IsAllPlayersReady()
{
	for (INT i = 0; i < (INT)this->joinedPlayer.size(); i++)
	{
		if (joinedPlayer[i]->GetPlayer()->GetKey() != this->roomLeader->GetPlayer()->GetKey())
		{
			if (!joinedPlayer[i]->GetReadyState())
				return FALSE;
		}
	}
	return TRUE;
}

BOOL Room::IsAllPlayersLoad()
{
	for (INT i = 0; i < (INT)this->joinedPlayer.size(); i++)
	{
		if (!joinedPlayer[i]->IsCopleteLoading())
			return FALSE;
	}
	return TRUE;
}

BOOL Room::IsStartGame()
{
	return this->isStartGame;
}

VOID Room::SetStartedGame()
{
	this->isStartGame = TRUE;
}