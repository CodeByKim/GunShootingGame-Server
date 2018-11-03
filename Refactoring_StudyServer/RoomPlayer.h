#pragma once
#include "Library.h"
#include "Player.h"

class RoomPlayer
{
private:
	Player* player;
	BOOL readyState;
	BOOL isLeader;
	INT index;

	BOOL isLoaded;

public:
	RoomPlayer(Player* player);

	VOID CompleteLoad();
	BOOL IsCopleteLoading();

	Player* GetPlayer();
	BOOL GetReadyState();
	VOID SetReady();
	VOID CancleReady();

	BOOL IsLeader();
	VOID SetLeader();

	INT GetIndex();
	VOID SetIndex(INT index);

	VOID Release();
};