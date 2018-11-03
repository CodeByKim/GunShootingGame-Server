#include "Library.h"
#include "Protocol.h"
#include "ProtocolManager.h"

VOID ProtocolManager::Initialize()
{		
	this->RegisterProtocolFunc((INT)PROTOCOL::REGISTER_USER, REGISTER_USER);
	this->RegisterProtocolFunc((INT)PROTOCOL::LOGIN, LOGIN);

	this->RegisterProtocolFunc((INT)PROTOCOL::ENTER_LOBBY, ENTER_LOBBY);
	this->RegisterProtocolFunc((INT)PROTOCOL::LEAVE_LOBBY, LEAVE_LOBBY);
	this->RegisterProtocolFunc((INT)PROTOCOL::LOBBY_CHATTING, LOBBY_CHATTING);

	this->RegisterProtocolFunc((INT)PROTOCOL::CREATE_ROOM, CREATE_ROOM);
	this->RegisterProtocolFunc((INT)PROTOCOL::LEAVE_ROOM, LEAVE_ROOM);
	this->RegisterProtocolFunc((INT)PROTOCOL::JOIN_ROOM, JOIN_ROOM);
	this->RegisterProtocolFunc((INT)PROTOCOL::ROOM_CHATTING, ROOM_CHATTING);
	this->RegisterProtocolFunc((INT)PROTOCOL::READY, ROOM_READY);
	this->RegisterProtocolFunc((INT)PROTOCOL::ROOM_START, ROOM_START);

	this->RegisterProtocolFunc((INT)PROTOCOL::GAME_START, GAME_START);
	this->RegisterProtocolFunc((INT)PROTOCOL::GAME_END, GAME_END);

	this->RegisterProtocolFunc((INT)PROTOCOL::GAME_PLAYER_MOVE, GAME_PLAYER_MOVE);
	this->RegisterProtocolFunc((INT)PROTOCOL::GAME_PLAYER_ATTACK, GAME_PLAYER_ATTACK);
	this->RegisterProtocolFunc((INT)PROTOCOL::GAME_PLAYER_DAMAGED, GAME_PLAYER_DAMAGED);
	this->RegisterProtocolFunc((INT)PROTOCOL::GAME_PLAYER_DIE, GAME_PLAYER_DIE);

	this->RegisterProtocolFunc((INT)PROTOCOL::ENEMY_SPAWN, ENEMY_SPAWN);
	this->RegisterProtocolFunc((INT)PROTOCOL::ENEMY_MOVE, ENEMY_MOVE);
	this->RegisterProtocolFunc((INT)PROTOCOL::ENEMY_DAMAGED, ENEMY_DAMAGED);
	this->RegisterProtocolFunc((INT)PROTOCOL::ENEMY_ATTACK, ENEMY_ATTACK);
	this->RegisterProtocolFunc((INT)PROTOCOL::ENEMY_DIE, ENEMY_DIE);

	this->RegisterProtocolFunc((INT)PROTOCOL::POTION_SPAWN, POTION_SPAWN);
	this->RegisterProtocolFunc((INT)PROTOCOL::POTION_PICKUP, POTION_PICKUP);

	this->RegisterProtocolFunc((INT)PROTOCOL::KOREAN_STRING_TEST, KOREAN_STRING_TEST);
	this->RegisterProtocolFunc((INT)PROTOCOL::HEART_BEAT, HEART_BEAT);	

	Utils::GetInstance()->LogTrace(L"PROTOCOL MANAGER", L"초기화 완료");
}

VOID ProtocolManager::RegisterProtocolFunc(INT protocol, ProcessProtocolFunc func)
{
	this->protocolFuncMap[protocol] = func;
}

VOID ProtocolManager::Execute(INT protocol, Player* player, TcpPacket* packet)
{
	this->protocolFuncMap[protocol](player, packet);
}