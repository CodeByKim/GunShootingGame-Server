#pragma once
#include "Library.h"
#include "Player.h"
#include "NetworkSession.h"
#include "TcpPacket.h"
#include "RoomPlayer.h"
#include "GamePlayer.h"
#include "Locker.h"

class PlayerManager : public Singleton<PlayerManager>
{
private:
	map<wstring, Player*> players;
	vector<vector<Player*>> sequentialPlayers;

	map<Player*, RoomPlayer*> roomPlayerMap;
	map<Player*, GamePlayer*> gamePlayerMap;
	INT senderThreadIndex;

	Locker locker;

public:
	VOID Initialize();
	VOID Release();

	BOOL FindPlayer(wstring playerKey);
	Player* FindPlayerFromKey(wstring playerKey);
	BOOL OverlapedLoginCheck(wstring playerID);

	VOID AddPlayer(Player* player);
	VOID RemovePlayer(Player* player);

	INT GetPlayerCount();	
	map<wstring, Player*>& GetPlayers();

	RoomPlayer* GetRoomPlayer(Player* player);
	VOID RegisterRoomPlayer(Player* player, RoomPlayer* roomPlayer);
	VOID RemoveRoomPlayer(Player* player);

	GamePlayer* GetGamePlayer(Player* player);
	VOID RegisterGamePlayer(Player* player, GamePlayer* roomPlayer);
	VOID RemoveGamePlayer(Player* player);

	VOID SendPacketQueue(INT index);
};
