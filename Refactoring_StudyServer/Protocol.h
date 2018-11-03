#pragma once
#include "Library.h"
#include "Player.h"
#include "TcpPacket.h"

VOID REGISTER_USER(Player* player, TcpPacket* packet);
VOID LOGIN(Player* player, TcpPacket* packet);

VOID ENTER_LOBBY(Player* player, TcpPacket* packet);
VOID LEAVE_LOBBY(Player* player, TcpPacket* packet);
VOID CREATE_ROOM(Player* player, TcpPacket* packet);
VOID JOIN_ROOM(Player* player, TcpPacket* packet);
VOID LEAVE_ROOM(Player* player, TcpPacket* packet);
VOID LOBBY_CHATTING(Player* player, TcpPacket* packet);
VOID ROOM_CHATTING(Player* player, TcpPacket* packet);
VOID ROOM_READY(Player* player, TcpPacket* packet);
VOID ROOM_START(Player* player, TcpPacket* packet);

VOID GAME_START(Player* player, TcpPacket* packet);
VOID GAME_END(Player* player, TcpPacket* packet);

VOID GAME_PLAYER_MOVE(Player* player, TcpPacket* packet);
VOID GAME_PLAYER_ATTACK(Player* player, TcpPacket* packet);
VOID GAME_PLAYER_DAMAGED(Player* player, TcpPacket* packet);
VOID GAME_PLAYER_DIE(Player* player, TcpPacket* packet);

VOID ENEMY_SPAWN(Player* player, TcpPacket* packet);
VOID ENEMY_MOVE(Player* player, TcpPacket* packet);
VOID ENEMY_DAMAGED(Player* player, TcpPacket* packet);
VOID ENEMY_ATTACK(Player* player, TcpPacket* packet);
VOID ENEMY_DIE(Player* player, TcpPacket* packet);

VOID POTION_SPAWN(Player* player, TcpPacket* packet);
VOID POTION_PICKUP(Player* player, TcpPacket* packet);

VOID KOREAN_STRING_TEST(Player* player, TcpPacket* packet);
VOID HEART_BEAT(Player* player, TcpPacket* packet);