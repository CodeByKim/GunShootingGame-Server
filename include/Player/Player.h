#pragma once
#include "Library.h"
#include "NetworkSession.h"
#include "Locker.h"

class Player
{
private:
	NetworkSession* session;
	wstring playerKey;		
	wstring playerID;		

	BOOL isEnterGame;
	PLAYER_STATE currentState;
	Locker locker;

public:
	Player(NetworkSession* session, wstring playerKey);
	~Player();

	VOID Release();
	wstring GetKey();
	wstring GetPlayerID();
	NetworkSession* GetNetworkSession();

	BOOL IsEnterToGame();
	VOID EnterToGame(wstring playerID);

	VOID PutPackageToNetwork();

	VOID SetState(PLAYER_STATE state);
	PLAYER_STATE GetState();
};