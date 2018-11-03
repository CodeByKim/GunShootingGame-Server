#include "Library.h"
#include "GameManager.h"

VOID GameManager::AddGame(Game* game)
{
	this->games.push_back(game);
}

VOID GameManager::RemoveGame(Game* game)
{
	for (INT i = 0; i < (INT)this->games.size(); i++)
	{
		if (this->games[i]->GetKey() == game->GetKey())
		{
			this->games.erase(this->games.begin() + i);
		}
	}
}

Game* GameManager::FindGame(wstring gameKey)
{
	for (INT i = 0; i < (INT)this->games.size(); i++)
	{
		if (games[i]->GetKey() == gameKey)
			return games[i];
	}

	return NULL;
}