#include "Player.h"
#include "NetworkSession.h"
#include "Package.h"
#include "Network.h"

Player::Player(NetworkSession* session, wstring playerKey)
{
	this->session = session;
	this->playerKey = playerKey;	
	this->currentState = PLAYER_STATE::LOGIN;
	this->isEnterGame = FALSE;
	this->locker.Initialize();
}

Player::~Player()
{

}

VOID Player::Release()
{
	this->isEnterGame = FALSE;
	this->session->Release();
}

wstring Player::GetKey()
{
	return this->playerKey;
}

wstring Player::GetPlayerID()
{
	return this->playerID;
}

NetworkSession* Player::GetNetworkSession()
{
	return this->session;
}

BOOL Player::IsEnterToGame()
{
	return this->isEnterGame;
}

VOID Player::EnterToGame(wstring playerID)
{
	this->isEnterGame = TRUE;
	this->playerID = playerID;
}

VOID Player::PutPackageToNetwork()
{
	INT queueSize = (INT)this->GetNetworkSession()->GetRecvQueueSize();
	for (INT i = 0; i < queueSize; i++)
	{
		Package* package = new Package();
		package->packet = GetNetworkSession()->GetPacketFromRecvQueue();
		package->player = this;		
		Network::GetInstance()->PutPackage(package);
	}

	this->GetNetworkSession()->GetSocket()->Receive();
}

VOID Player::SetState(PLAYER_STATE state)
{
	this->currentState = state;
}

PLAYER_STATE Player::GetState()
{
	return this->currentState;
}