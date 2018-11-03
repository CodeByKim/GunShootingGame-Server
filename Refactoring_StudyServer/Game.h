#pragma once
#include "Library.h"
#include "GamePlayer.h"
#include "Room.h"

class Vector3
{
public:
	FLOAT x;
	FLOAT y;
	FLOAT z;
};

class Game
{
private:
	wstring gameKey;
	vector<GamePlayer*> gamePlayers;
	Room* returnRoom;
	GamePlayer* leader;

public:
	Game();

	wstring GetKey();
	VOID AddGamePlayer(GamePlayer* gamePlayer);

	BOOL IsAllPlayerCompleteLoading();

	VOID SetReturnRoom(Room* room);
	Room* GetReturnRoom();

	vector<GamePlayer*>& GetPlayers();
	GamePlayer* GetLeader();
	VOID SetLeader(GamePlayer* gamePlayer);
};