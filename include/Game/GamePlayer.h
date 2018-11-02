#pragma once
#include "Library.h"
#include "Player.h"

class GamePlayerAttribute
{
private:
	INT hp;

public:
	VOID Initialize();
};

class GamePlayer
{
private:
	Player* player;
	BOOL loading;
	BOOL isLeader;
	GamePlayerAttribute attribute;

public:
	GamePlayer(Player* player);

	Player* GetPlayer();
	VOID CompleteLoading();

	VOID SetLeader();

	BOOL IsCompleteLoading();
	BOOL IsLeader();
};