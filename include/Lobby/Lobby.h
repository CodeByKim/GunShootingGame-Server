#pragma once
#include "Library.h"
#include "Room.h"
#include "Player.h"
#include "RoomPlayer.h"

class Lobby : public Singleton<Lobby>
{
private:
	vector<Player*> players;

public:
	VOID Initialize();
	VOID Release();

	VOID EnterPlayer(Player* player);
	VOID LeavePlayer(Player* player);

	VOID RemovePlayer(Player* player);
	INT GetPlayerCount();
	vector<Player*>& GetPlayers();
};