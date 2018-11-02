#include "Protocol.h"
#include "UserDB.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Lobby.h"
#include "Utils.h"
#include "Network.h"
#include "GamePacketData.h"
#include "TestPacketData.h"
#include "PacketFactory.h"
#include "Game.h"
#include "GameManager.h"

VOID HEART_BEAT(Player* player, TcpPacket* packet)
{
	player->GetNetworkSession()->CheckHeartBeat();
	Utils::GetInstance()->LogSucc(L"HEART BEAT", L"하트비트 체크");
}

VOID GAME_START(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_GameStartPacketData clientToServerData;
	clientToServerData.Deserialize(packet);
	
	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		
		if (game != NULL)
		{
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);
			gamePlayer->CompleteLoading();

			if (game->IsAllPlayerCompleteLoading())
			{
				Utils::GetInstance()->LogSucc(L"GAME START", L"모든 플레이어의 로딩이 끝남, 게임 시작");

				vector<Package*> gamePackages;
				vector<GamePlayer*>& gamePlayers = game->GetPlayers();

				vector<Package*> lobbyPackages;
				vector<Player*>& lobbyPlayers = Lobby::GetInstance()->GetPlayers();

				SERVERtoCLIENT_GameStartPacketData broadcastData;

				SERVERtoCLIENT_OtherGameStartPacketData otherGameStartData;
				Room* room = game->GetReturnRoom();
				otherGameStartData.WriteData(room->GetName(), room->GetRoomID());

				for (INT i = 0; i < (INT)gamePlayers.size(); i++)
				{
					TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					broadcastData.Serialize(packet, PROTOCOL::GAME_START, EXTRA::SUCCESS);
					Package* package = new Package();
					package->packet = packet;
					package->player = gamePlayers[i]->GetPlayer();
					gamePackages.push_back(package);
				}

				for (INT i = 0; i < (INT)lobbyPlayers.size(); i++)
				{
					TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					otherGameStartData.Serialize(packet, PROTOCOL::OTHER_GAME_START, EXTRA::SUCCESS);
					Package* package = new Package();
					package->packet = packet;
					package->player = lobbyPlayers[i];
					lobbyPackages.push_back(package);
				}
				
				Network::GetInstance()->BroadCastPacket(gamePackages);
				Network::GetInstance()->BroadCastPacket(lobbyPackages);
			}
		}
	}
}

VOID GAME_END(Player* player, TcpPacket* packet)
{	
	CLIENTtoSERVER_GameEndPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		if (game != NULL)
		{			
			GameManager::GetInstance()->RemoveGame(game);
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<GamePlayer*>& players = game->GetPlayers();
			for (INT i = 0; i < (INT)players.size(); i++)
				PlayerManager::GetInstance()->RemoveGamePlayer(players[i]->GetPlayer());

			Room* room = game->GetReturnRoom();
			room->EndGame();

			Utils::GetInstance()->LogSucc(L"GAME END", L"게임 종료");

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			vector<Package*> lobbyPackages;
			vector<Player*>& lobbyPlayers = Lobby::GetInstance()->GetPlayers();

			SERVERtoCLIENT_GameEndPacketData serverToClientData;
			serverToClientData.WriteData(room->GetName(), room->GetRoomID(), room->GetJoinedPlayerCount(), room->GetJoinedPlayers());

			SERVERtoCLIENT_OtherGameEndPacketData otherGameEndData;
			otherGameEndData.WriteData(room->GetName(), room->GetRoomID());

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::GAME_END, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			for (INT i = 0; i < (INT)lobbyPlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				otherGameEndData.Serialize(packet, PROTOCOL::OTHER_GAME_END, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = lobbyPlayers[i];
				lobbyPackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages);
			Network::GetInstance()->BroadCastPacket(lobbyPackages);
		}
	}
}

VOID GAME_PLAYER_MOVE(Player* player, TcpPacket* packet)
{
	if (packet->header.extra == (INT)EXTRA::CLIENT_TO_SERVER)
	{
		CLIENTtoSERVER_GamePlayerMovePacketData clientToServerData;
		clientToServerData.Deserialize(packet);

		if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
		{
			Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
			
			if (game != NULL)
			{				
				GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);
				GamePlayer* leader = game->GetLeader();

				TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				SERVERtoHOST_GamePlayerMovePacketData serverToHostData;
				serverToHostData.WriteData(clientToServerData.playerKey, clientToServerData.playerPosition, clientToServerData.playerRotation);				
				serverToHostData.Serialize(sendPacket, PROTOCOL::GAME_PLAYER_MOVE, EXTRA::SUCCESS);				
				leader->GetPlayer()->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
			}
		}
	}
	else if (packet->header.extra == (INT)EXTRA::HOST_TO_SERVER)
	{
		CLIENTtoSERVER_GamePlayerMovePacketData hostToServerData;
		hostToServerData.Deserialize(packet);

		if (PlayerManager::GetInstance()->FindPlayer(hostToServerData.playerKey))
		{
			Game* game = GameManager::GetInstance()->FindGame(hostToServerData.gameKey);
			
			if (game != NULL)
			{
				GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

				if (gamePlayer->IsLeader())
				{
					Player* findPlayer = PlayerManager::GetInstance()->FindPlayerFromKey(hostToServerData.playerKey);

					vector<Package*> gamePackages;
					vector<GamePlayer*>& gamePlayers = game->GetPlayers();

					SERVERtoCLIENT_GamePlayerMovePacketData serverToClientData;
					serverToClientData.WriteData(findPlayer->GetPlayerID(), hostToServerData.playerPosition, hostToServerData.playerRotation);

					for (INT i = 0; i < (INT)gamePlayers.size(); i++)
					{
						TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
						serverToClientData.Serialize(packet, PROTOCOL::OTHER_GAME_PLAYER_MOVE, EXTRA::SUCCESS);
						Package* package = new Package();
						package->packet = packet;
						package->player = gamePlayers[i]->GetPlayer();
						gamePackages.push_back(package);
					}

					Network::GetInstance()->BroadCastPacket(gamePackages, findPlayer);
				}				
			}
		}
	}
}

VOID GAME_PLAYER_ATTACK(Player* player, TcpPacket* packet)
{
	if (packet->header.extra == (INT)EXTRA::CLIENT_TO_SERVER)
	{
		CLIENTtoSERVER_GamePlayerAttackPacketData clientToServerData;
		clientToServerData.Deserialize(packet);

		if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
		{
			Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
			
			if (game != NULL)
			{
				GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);
				GamePlayer* leader = game->GetLeader();

				TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				SERVERtoHOST_GamePlayerAttackPacketData serverToHostData;
				serverToHostData.WriteData(clientToServerData.playerKey);
				serverToHostData.Serialize(sendPacket, PROTOCOL::GAME_PLAYER_ATTACK, EXTRA::SUCCESS);
				leader->GetPlayer()->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
			}
		}
	}
	else if (packet->header.extra == (INT)EXTRA::HOST_TO_SERVER)
	{
		CLIENTtoSERVER_GamePlayerAttackPacketData hostToServerData;
		hostToServerData.Deserialize(packet);

		if (PlayerManager::GetInstance()->FindPlayer(hostToServerData.playerKey))
		{
			Game* game = GameManager::GetInstance()->FindGame(hostToServerData.gameKey);
			
			if (game != NULL)
			{
				GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

				if (gamePlayer->IsLeader())
				{
					Player* findPlayer = PlayerManager::GetInstance()->FindPlayerFromKey(hostToServerData.playerKey);

					vector<Package*> gamePackages;
					vector<GamePlayer*>& gamePlayers = game->GetPlayers();

					SERVERtoCLIENT_GamePlayerAttackPacketData serverToClientData;
					serverToClientData.WriteData(findPlayer->GetPlayerID());

					for (INT i = 0; i < (INT)gamePlayers.size(); i++)
					{
						TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
						serverToClientData.Serialize(packet, PROTOCOL::OTHER_GAME_PLAYER_ATTACK, EXTRA::SUCCESS);
						Package* package = new Package();
						package->packet = packet;
						package->player = gamePlayers[i]->GetPlayer();
						gamePackages.push_back(package);
					}

					Network::GetInstance()->BroadCastPacket(gamePackages, findPlayer);
				}
			}
		}
	}
}

VOID GAME_PLAYER_DAMAGED(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_GamePlayerDamagedPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);

		if (game != NULL)
		{
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_GamePlayerDamagedPacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.playerID, clientToServerData.hp);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::GAME_PLAYER_DAMAGED, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID GAME_PLAYER_DIE(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_GamePlayerDiePacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		
		if (game != NULL)
		{
			wstring message = L"";
			message.append(L"ID : ").append(player->GetPlayerID());			
			Utils::GetInstance()->LogSucc(L"GAME PLAYER DIE", message);

			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_GamePlayerDiePacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.playerID);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::GAME_PLAYER_DIE, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID ENEMY_SPAWN(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_EnemySpawnPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		
		if (game != NULL)
		{			
			Utils::GetInstance()->LogSucc(L"ENEMY SPAWN", L"몬스터 생성 됨");
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_EnemySpawnPacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.enemyID, clientToServerData.position);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::ENEMY_SPAWN, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID ENEMY_MOVE(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_EnemyMovePacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

		if (game != NULL)
		{
			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_EnemyMovePacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.enemyID, clientToServerData.position, clientToServerData.rotation);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::ENEMY_MOVE, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID ENEMY_DAMAGED(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_EnemyDamagedPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

		if (game != NULL)
		{
			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_EnemyDamagedPacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.enemyID);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::ENEMY_DAMAGED, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID ENEMY_ATTACK(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_EnemyAttackPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		
		if (game != NULL)
		{
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_EnemyAttackPacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.enemyID);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::ENEMY_ATTACK, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID ENEMY_DIE(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_EnemyDiePacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);
		
		if (game != NULL)
		{
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_EnemyDiePacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.enemyID);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::ENEMY_DIE, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID POTION_SPAWN(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_PotionSpawnPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);

		if (game != NULL)
		{
			Utils::GetInstance()->LogSucc(L"POTION SPAWN", L"포션 생성 됨");
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_PotionSpawnPacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.potionID, clientToServerData.position);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::POTION_SPAWN, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}

VOID POTION_PICKUP(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_PotionPickupPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		Game* game = GameManager::GetInstance()->FindGame(clientToServerData.gameKey);

		if (game != NULL)
		{
			wstring message = L"";
			message.append(L"ID : ").append(player->GetPlayerID());
			Utils::GetInstance()->LogSucc(L"POTION PICKUP", message);
			GamePlayer* gamePlayer = PlayerManager::GetInstance()->GetGamePlayer(player);

			vector<Package*> gamePackages;
			vector<GamePlayer*>& gamePlayers = game->GetPlayers();

			SERVERtoCLIENT_PotionPickupPacketData serverToClientData;
			serverToClientData.WriteData(clientToServerData.playerID, clientToServerData.potionID, clientToServerData.hp);

			for (INT i = 0; i < (INT)gamePlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				serverToClientData.Serialize(packet, PROTOCOL::POTION_PICKUP, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = gamePlayers[i]->GetPlayer();
				gamePackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(gamePackages, player);
		}
	}
}