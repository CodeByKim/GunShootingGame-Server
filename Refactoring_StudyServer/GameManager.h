#pragma once
#include "Library.h"
#include "Game.h"

class GameManager : public Singleton<GameManager>
{
private:
	vector<Game*> games;

public:
	VOID AddGame(Game* game);
	VOID RemoveGame(Game* game);
	Game* FindGame(wstring gameKey);
};

