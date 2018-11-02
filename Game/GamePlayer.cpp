#include "GamePlayer.h"

VOID GamePlayerAttribute::Initialize()
{
	this->hp = 100;
}

GamePlayer::GamePlayer(Player* player)
{
	this->player = player;
	this->loading = FALSE;
	this->attribute.Initialize();
	this->isLeader = false;
}

Player* GamePlayer::GetPlayer()
{
	return this->player;
}

VOID GamePlayer::CompleteLoading()
{
	this->loading = TRUE;
}

VOID GamePlayer::SetLeader()
{
	this->isLeader = TRUE;
}

BOOL GamePlayer::IsCompleteLoading()
{
	return this->loading;
}

BOOL GamePlayer::IsLeader()
{
	return this->isLeader;
}