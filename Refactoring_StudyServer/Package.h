#pragma once
#include "Library.h"
#include "TcpPacket.h"
#include "Player.h"

class Package
{
public:
	virtual ~ Package()
	{
	}

	Player* player;
	TcpPacket* packet;
};
