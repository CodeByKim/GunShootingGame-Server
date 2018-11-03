#pragma once
#include "Library.h"
#include "Player.h"
#include "RoomPlayer.h"

class Room
{
private:
	wstring roomName;
	wstring roomID;

	vector<RoomPlayer*> joinedPlayer;
	RoomPlayer* roomLeader;

	INT joinedPlayerCount;
	BOOL isStartGame;

public:
	Room(wstring roomName, Player* leader);
	~Room();

	VOID Release();
	VOID Join(Player* player);

	VOID Leave(RoomPlayer* roomPlayer);
	VOID Ready(RoomPlayer* roomPlayer, BOOL readyState);

	VOID EndGame();

	Player* ChangeLeader();

	INT GetJoinedPlayerCount();
	wstring GetName();
	wstring GetRoomID();
	vector<RoomPlayer*>& GetJoinedPlayers();

	RoomPlayer* GetLeader();

	BOOL IsRoomLeader(Player* player);
	BOOL IsAllPlayersReady();
	BOOL IsAllPlayersLoad();

	BOOL IsStartGame();
	VOID SetStartedGame();
};