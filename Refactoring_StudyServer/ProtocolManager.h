#pragma once
#include "Library.h"
#include "NetworkSession.h"
#include "Player.h"
#include "TcpPacket.h"

typedef VOID(*ProcessProtocolFunc)(Player*, TcpPacket*);

class ProtocolManager : public Singleton<ProtocolManager>
{
private:
	map<INT, ProcessProtocolFunc> protocolFuncMap;

public:
	VOID Initialize();
	VOID RegisterProtocolFunc(INT protocol, ProcessProtocolFunc func);
	VOID Execute(INT protocol, Player* player, TcpPacket* packet);
};