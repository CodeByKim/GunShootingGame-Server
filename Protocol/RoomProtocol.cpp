#include "Protocol.h"
#include "UserDB.h"
#include "Player.h"
#include "PlayerManager.h"
#include "RoomManager.h"
#include "Lobby.h"
#include "Utils.h"
#include "Network.h"
#include "PacketFactory.h"
#include "RoomPacketData.h"
#include "LobbyPacketData.h"
#include "Game.h"
#include "GameManager.h"

VOID CREATE_ROOM(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_CreateRoomPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		//������ ��û�� ���� �̸��� ���� �κ� �ִ� ��� ���� �̸��� �ִ°�? Ȯ�� �ʿ�
		if (!RoomManager::GetInstance()->ExistRoomName(clientToServerData.roomName))
		{			
			player->SetState(PLAYER_STATE::IN_ROOM);
			Room* newRoom = new Room(clientToServerData.roomName, player);
			RoomManager::GetInstance()->AddRoom(newRoom);
			Lobby::GetInstance()->RemovePlayer(player);
			
			TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			SERVERtoCLIENT_CreateRoomPacketData serverToClientData;
			serverToClientData.WriteData(newRoom->GetRoomID(), newRoom->GetName());
			serverToClientData.Serialize(sendPacket, PROTOCOL::CREATE_ROOM, EXTRA::SUCCESS);
			player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

			wstring message = L"";
			message.append(L"�� �̸� : ").append(clientToServerData.roomName).append(L", ���� : ").append(newRoom->GetLeader()->GetPlayer()->GetPlayerID());
			Utils::GetInstance()->LogSucc(L"CREATE ROOM", message);

			vector<Package*> packages;
			vector<Player*>& players = Lobby::GetInstance()->GetPlayers();
			SERVERtoCLIENT_CreateRoomOtherPlayerPacketData broadcastData;
			broadcastData.WriteData(newRoom->GetName(), newRoom->GetRoomID(), player->GetPlayerID());

			for (INT i = 0; i < (INT)players.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				broadcastData.Serialize(packet, PROTOCOL::CREATE_ROOM_OTHER_PLAYER, EXTRA::FOR_JOIN_ROOM);
				Package* package = new Package();
				package->packet = packet;
				package->player = players[i];
				packages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(packages, player);
		}
		else
		{
			//�̹� ���� �� ������ ����...
			TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			SERVERtoCLIENT_CreateRoomPacketData serverToClientData;
			serverToClientData.Serialize(sendPacket,PROTOCOL::CREATE_ROOM, EXTRA::FAIL);
			player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

			Utils::GetInstance()->LogFail(L"CREATE ROOM", L"�̹� �����ϴ� �̸����� �� ������ �õ���");
		}
	}
}

VOID JOIN_ROOM(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_JoinRoomPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{		
		Room* room = RoomManager::GetInstance()->FindRoom(clientToServerData.roomID);
		if (room != NULL)
		{
			if (room->GetJoinedPlayerCount() < MAX_ROOM_USER)
			{
				room->Join(player);
				Lobby::GetInstance()->RemovePlayer(player);

				TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				SERVERtoCLIENT_JoinRoomPacketData serverToClientData;
				serverToClientData.WriteData(room->GetRoomID(), room->GetName(), room->GetJoinedPlayerCount(), room->GetJoinedPlayers());
				serverToClientData.Serialize(sendPacket, PROTOCOL::JOIN_ROOM, EXTRA::SUCCESS);				
				player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);				

				wstring message = L"";
				message.append(L"�� �̸� : ").append(room->GetName()).append(L", ������ : ").append(player->GetPlayerID());
				Utils::GetInstance()->LogSucc(L"JOIN ROOM", message);

				vector<Package*> lobbyPackages;
				vector<Player*>& lobbyPlayers = Lobby::GetInstance()->GetPlayers();

				vector<Package*> roomPackages;
				vector<RoomPlayer*>& roomPlayers = room->GetJoinedPlayers();

				SERVERtoCLIENT_JoinRoomOtherPlayerPacketData broadcastData;
				broadcastData.WriteData(player->GetPlayerID(), room->GetRoomID());

				for (INT i = 0; i < (INT)lobbyPlayers.size(); i++)
				{
					TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					broadcastData.Serialize(packet, PROTOCOL::JOIN_ROOM_OTHER_PLAYER, EXTRA::SUCCESS);
					Package* package = new Package();
					package->packet = packet;
					package->player = lobbyPlayers[i];
					lobbyPackages.push_back(package);
				}

				for (INT i = 0; i < (INT)roomPlayers.size(); i++)
				{
					TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					broadcastData.Serialize(packet, PROTOCOL::JOIN_ROOM_OTHER_PLAYER, EXTRA::SUCCESS);
					Package* package = new Package();
					package->packet = packet;
					package->player = roomPlayers[i]->GetPlayer();
					roomPackages.push_back(package);
				}

				Network::GetInstance()->BroadCastPacket(lobbyPackages, player);
				Network::GetInstance()->BroadCastPacket(roomPackages, player);
			}
		}
	}
}

VOID ROOM_READY(Player* player, TcpPacket* packet)
{	
	CLIENTtoSERVER_ReadyPacketData clientToServerData;
	clientToServerData.Deserialize(packet);
	
	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		RoomPlayer* roomPlayer = PlayerManager::GetInstance()->GetRoomPlayer(player);
		Room* room = RoomManager::GetInstance()->GetJoinRoom(roomPlayer);

		if (room != NULL)
		{						
			if(roomPlayer->GetReadyState())
				room->Ready(roomPlayer, FALSE);
			else
				room->Ready(roomPlayer, TRUE);

			TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			SERVERtoCLIENT_ReadyPacketData serverToClientData; 
			serverToClientData.WriteData(roomPlayer->GetReadyState());
			serverToClientData.Serialize(sendPacket, PROTOCOL::READY, EXTRA::SUCCESS);
			player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

			wstring message = L"";			
			message.append(L"�� �̸� : ").append(room->GetName()).append(L", ������ : ").append(player->GetPlayerID());
			Utils::GetInstance()->LogSucc(L"ROOM READY", message);
			
			vector<Package*> roomPackages;
			vector<RoomPlayer*>& roomPlayers = room->GetJoinedPlayers();

			SERVERtoCLIENT_ReadyOtherPlayerPacketData broadcastData;
			broadcastData.WriteData(player->GetPlayerID(), roomPlayer->GetReadyState());

			for (INT i = 0; i < (INT)roomPlayers.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				broadcastData.Serialize(packet, PROTOCOL::READY_OTHER_PLAYER, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = roomPlayers[i]->GetPlayer();
				roomPackages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(roomPackages, player);
		}
	}
}

VOID ROOM_START(Player* player, TcpPacket* packet)
{	
	CLIENTtoSERVER_RoomStartPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	// 1. ���� ������ �� �ο��� ����������� Ȯ��
	// 2. ��� ������ �� �ο� �������� Start ��Ŷ�� �����ְ�
	// 3. ��� ���� �ƴ϶�� ���� �Ұ��� ��Ŷ�� ���忡�� ������

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		RoomPlayer* roomPlayer = PlayerManager::GetInstance()->GetRoomPlayer(player);
		Room* room = RoomManager::GetInstance()->GetJoinRoom(roomPlayer);

		if (room != NULL)
		{
			if (room->GetLeader()->GetPlayer()->GetKey() == player->GetKey())	//��Ŷ�� ������ ��ü�� �������� Ȯ��
			{
				if (room->IsAllPlayersReady())
				{					
					Game* game = new Game();
					game->SetReturnRoom(room);
					vector<RoomPlayer*>& players = room->GetJoinedPlayers();

					for (INT i = 0; i < (INT)players.size(); i++)
					{
						PlayerManager::GetInstance()->RemoveRoomPlayer(players[i]->GetPlayer());
						GamePlayer* gamePlayer = new GamePlayer(players[i]->GetPlayer());
						
						if (players[i]->IsLeader())
						{
							gamePlayer->SetLeader();
							game->SetLeader(gamePlayer);
						}
							
						PlayerManager::GetInstance()->RegisterGamePlayer(gamePlayer->GetPlayer(), gamePlayer);
						game->AddGamePlayer(gamePlayer);												
					}

					GameManager::GetInstance()->AddGame(game);
					room->SetStartedGame();
					
					Utils::GetInstance()->LogSucc(L"ROOM START", L"���� ���� �غ� �Ϸ�");

					vector<Package*> packages;
					SERVERtoCLIENT_RoomStartPacketData broadcastData;
					broadcastData.WriteData(game->GetKey());

					for (INT i = 0; i < (INT)players.size(); i++)
					{
						TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
						broadcastData.Serialize(packet, PROTOCOL::ROOM_START, EXTRA::SUCCESS);
						Package* package = new Package();
						package->packet = packet;
						package->player = players[i]->GetPlayer();
						packages.push_back(package);
					}

					Network::GetInstance()->BroadCastPacket(packages);
					
				}
				else
				{					
					Utils::GetInstance()->LogFail(L"ROOM START", L"���� ��� �÷��̾ �غ���� ����");

					TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					SERVERtoCLIENT_RoomStartPacketData data;
					data.Serialize(sendPacket, PROTOCOL::ROOM_START, EXTRA::FAIL);
					player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
				}
			}
		}
	}
}

VOID LEAVE_ROOM(Player* player, TcpPacket* packet)
{
	// �� ���� ��û
	// 1. �濡 �� ���� �ٸ� �÷��̾ ���ٸ�?
	// 2. ���� �κ�� ���� �׸��� �κ� �ִ� �÷��̾�� ���� ���Դٰ� �˸�
	// 3. �׸��� �濡 �ƹ��� �����Ƿ� ���� �����ϰ� �κ� ���� �����Ǿ��ٰ� �˸�
	// 4. �濡 ������ �ٸ� �÷��̾ �ִٸ�?
	// 5. �׸��� ���� ���� �����̾��ٸ�?
	// 6. �� ���� ��� �Ǵ� ��������� ���ٸ� 0��° ����� �������� �Ӹ�
	// 7. �濡 ������ �ο����� ������ �ٲ���ٰ� ����
	// 8. ���� �濡�� ���� �����ϰ� ���� ���ŵǾ��ٰ� �濡 �˸�
	// 9. ���� ���� �����Դ� �κ��� ���� ����

	CLIENTtoSERVER_LeaveRoomPacketData clientToServerData;
	clientToServerData.Deserialize(packet);
	
	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		RoomPlayer* roomPlayer = PlayerManager::GetInstance()->GetRoomPlayer(player);
		Room* room = RoomManager::GetInstance()->GetJoinRoom(roomPlayer);

		if (room != NULL)
		{
			if (room->GetJoinedPlayerCount() == 1)
			{
				room->Leave(roomPlayer);

				Lobby::GetInstance()->EnterPlayer(player);
				player->SetState(PLAYER_STATE::LOBBY);

				wstring removeRoomID = room->GetRoomID();
				RoomManager::GetInstance()->RemoveRoom(room);

				TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				SERVERtoCLIENT_LeaveRoomPacketData leaveRoomServerToClientData;
				leaveRoomServerToClientData.WriteData(RoomManager::GetInstance()->GetRoomCount(), RoomManager::GetInstance()->GetRooms(), Lobby::GetInstance()->GetPlayerCount(), Lobby::GetInstance()->GetPlayers());
				leaveRoomServerToClientData.Serialize(sendPacket, PROTOCOL::LEAVE_ROOM, EXTRA::SUCCESS);
				player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

				wstring message = L"";
				message.append(L"�� �̸� : ").append(room->GetName()).append(L", ������ : ").append(player->GetPlayerID());
				Utils::GetInstance()->LogSucc(L"LEAVE ROOM", message);

				vector<Package*> leaveRoomOtherPlayerPackages;
				vector<Package*> removeRoomPackages;
				vector<Player*>& lobbyPlayers = Lobby::GetInstance()->GetPlayers();
				
				SERVERtoCLIENT_LeaveRoomOtherPlayerPacketData leaveRoomOtherPlayerbroadcastData;
				leaveRoomOtherPlayerbroadcastData.WriteData(player->GetPlayerID(), room->GetName(), room->GetRoomID(), room->GetJoinedPlayerCount());
				SERVERtoCLIENT_RemoveRoomPacketData removeRoomBroadcastData;
				removeRoomBroadcastData.WriteData(removeRoomID);

				for (INT i = 0; i < (INT)lobbyPlayers.size(); i++)
				{
					TcpPacket* leaveRoomOtherPlayerPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					leaveRoomOtherPlayerbroadcastData.Serialize(leaveRoomOtherPlayerPacket, PROTOCOL::LEAVE_ROOM_OTHER_PLAYER, EXTRA::SUCCESS);
					Package* leaveRoomOtherPlayerPackage = new Package();
					leaveRoomOtherPlayerPackage->packet = leaveRoomOtherPlayerPacket;
					leaveRoomOtherPlayerPackage->player = lobbyPlayers[i];
					leaveRoomOtherPlayerPackages.push_back(leaveRoomOtherPlayerPackage);

					TcpPacket* removeRoomPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					removeRoomBroadcastData.Serialize(removeRoomPacket, PROTOCOL::REMOVE_ROOM, EXTRA::SUCCESS);
					Package* removeRoomPackage = new Package();
					removeRoomPackage->packet = removeRoomPacket;
					removeRoomPackage->player = lobbyPlayers[i];
					removeRoomPackages.push_back(removeRoomPackage);
				}

				Network::GetInstance()->BroadCastPacket(leaveRoomOtherPlayerPackages, player);
				Network::GetInstance()->BroadCastPacket(removeRoomPackages);
			}
			else if (room->GetJoinedPlayerCount() > 1)
			{
				if (room->IsRoomLeader(player))
				{
					Player* newLeader = room->ChangeLeader();
					RoomPlayer* leaderRoomPlayer = PlayerManager::GetInstance()->GetRoomPlayer(newLeader);
					room->Ready(leaderRoomPlayer, FALSE);

					vector<Package*> packages;
					vector<RoomPlayer*>& players = room->GetJoinedPlayers();
					SERVERtoCLIENT_ChangeRoomLeaderPacketData changeRoomLeaderbroadcastData;
					changeRoomLeaderbroadcastData.WriteData(newLeader->GetKey(), newLeader->GetPlayerID());

					for (INT i = 0; i < (INT)players.size(); i++)
					{
						TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
						changeRoomLeaderbroadcastData.Serialize(packet, PROTOCOL::CHANGE_ROOM_LEADER, EXTRA::SUCCESS);
						Package* package = new Package();
						package->packet = packet;
						package->player = players[i]->GetPlayer();
						packages.push_back(package);
					}

					wstring message = L"";
					message.append(L"���ο� ���� : ").append(leaderRoomPlayer->GetPlayer()->GetPlayerID());
					Utils::GetInstance()->LogSucc(L"CHANGE ROOM LEADER", message);
					Network::GetInstance()->BroadCastPacket(packages, player);
				}
				
				room->Leave(roomPlayer);
				Lobby::GetInstance()->EnterPlayer(player);
				player->SetState(PLAYER_STATE::LOBBY);

				TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				SERVERtoCLIENT_LeaveRoomPacketData leaveRoomServerToClientData;
				leaveRoomServerToClientData.WriteData(RoomManager::GetInstance()->GetRoomCount(), RoomManager::GetInstance()->GetRooms(), Lobby::GetInstance()->GetPlayerCount(), Lobby::GetInstance()->GetPlayers());
				leaveRoomServerToClientData.Serialize(sendPacket, PROTOCOL::LEAVE_ROOM, EXTRA::SUCCESS);
				player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

				vector<Package*> lobbyPackages;
				vector<Player*>& lobbyPlayers = Lobby::GetInstance()->GetPlayers();

				vector<Package*> roomPackages;
				vector<RoomPlayer*>& roomPlayers = room->GetJoinedPlayers();

				SERVERtoCLIENT_LeaveRoomOtherPlayerPacketData leaveRoomOtherPlayerbroadcastData;
				leaveRoomOtherPlayerbroadcastData.WriteData(player->GetPlayerID(), room->GetName(), room->GetRoomID(), room->GetJoinedPlayerCount());

				for (INT i = 0; i < (INT)lobbyPlayers.size(); i++)
				{
					TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					leaveRoomOtherPlayerbroadcastData.Serialize(packet, PROTOCOL::LEAVE_ROOM_OTHER_PLAYER, EXTRA::SUCCESS);
					Package* package = new Package();
					package->packet = packet;
					package->player = lobbyPlayers[i];
					lobbyPackages.push_back(package);
				}

				for (INT i = 0; i < (INT)roomPlayers.size(); i++)
				{
					TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
					leaveRoomOtherPlayerbroadcastData.Serialize(packet, PROTOCOL::LEAVE_ROOM_OTHER_PLAYER, EXTRA::SUCCESS);
					Package* package = new Package();
					package->packet = packet;
					package->player = roomPlayers[i]->GetPlayer();
					roomPackages.push_back(package);
				}

				Network::GetInstance()->BroadCastPacket(lobbyPackages, player);
				Network::GetInstance()->BroadCastPacket(roomPackages, player);
			}
		}
	}
}

VOID ROOM_CHATTING(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_RoomChattingPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		RoomPlayer* roomPlayer = PlayerManager::GetInstance()->GetRoomPlayer(player);
		Room* room = RoomManager::GetInstance()->GetJoinRoom(roomPlayer);

		if (room != NULL)
		{
			vector<Package*> packages;
			vector<RoomPlayer*>& players = room->GetJoinedPlayers();
			SERVERtoCLIENT_RoomChattingPacketData broadcastData;
			broadcastData.WriteData(player->GetPlayerID(), clientToServerData.chatString);

			for (INT i = 0; i < (INT)players.size(); i++)
			{
				TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
				broadcastData.Serialize(packet, PROTOCOL::ROOM_CHATTING, EXTRA::SUCCESS);
				Package* package = new Package();
				package->packet = packet;
				package->player = players[i]->GetPlayer();
				packages.push_back(package);
			}

			Network::GetInstance()->BroadCastPacket(packages);
			
			wstring message = L"";
			message.append(L"ID : ").append(player->GetPlayerID()).append(L", ä�� �޽��� : ").append(clientToServerData.chatString);
			Utils::GetInstance()->LogSucc(L"LOBBY CHATTING", message);
		}
	}
}
