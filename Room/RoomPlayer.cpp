#include "RoomPlayer.h"

RoomPlayer::RoomPlayer(Player* player)
{
	this->player = player;
	this->isLeader = FALSE;
	this->readyState = FALSE;
	this->index = 0;
}

VOID RoomPlayer::CompleteLoad()
{
	this->isLoaded = TRUE;
}

BOOL RoomPlayer::IsCopleteLoading()
{
	return this->isLoaded;
}

Player* RoomPlayer::GetPlayer()
{
	return this->player;		
}

VOID RoomPlayer::SetReady()
{
	this->readyState = TRUE;
}

VOID RoomPlayer::CancleReady()
{
	this->readyState = FALSE;
}

BOOL RoomPlayer::GetReadyState()
{
	return this->readyState;
}

BOOL RoomPlayer::IsLeader()
{
	return this->isLeader;
}

VOID RoomPlayer::SetLeader()
{
	this->isLeader = TRUE;
	SetIndex(0);
}

INT RoomPlayer::GetIndex()
{
	return this->index;
}

VOID RoomPlayer::SetIndex(INT index)
{
	this->index = index;
}

VOID RoomPlayer::Release()
{
	CancleReady();
	this->isLoaded = FALSE;
}