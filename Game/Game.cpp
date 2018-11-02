#include "Game.h"

Game::Game()
{
	this->gameKey = Utils::GetInstance()->GenerateUUID();
}

wstring Game::GetKey()
{
	return this->gameKey;
}

VOID Game::AddGamePlayer(GamePlayer* gamePlayer)
{
	this->gamePlayers.push_back(gamePlayer);
}

BOOL Game::IsAllPlayerCompleteLoading()
{
	for (INT i = 0; i < (INT)this->gamePlayers.size(); i++)
	{
		if (!gamePlayers[i]->IsCompleteLoading())
			return FALSE;
	}

	return TRUE;
}

VOID Game::SetReturnRoom(Room* room)
{
	this->returnRoom = room;
}

Room* Game::GetReturnRoom()
{
	return this->returnRoom;
}

vector<GamePlayer*>& Game::GetPlayers()
{
	return this->gamePlayers;
}

VOID Game::SetLeader(GamePlayer* gamePlayer)
{
	this->leader = gamePlayer;
}

GamePlayer* Game::GetLeader()
{
	return this->leader;
}