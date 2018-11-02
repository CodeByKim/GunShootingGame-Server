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
		//생성을 요청한 방의 이름이 기존 로비에 있는 방과 같은 이름이 있는가? 확인 필요
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
			message.append(L"방 이름 : ").append(clientToServerData.roomName).append(L", 방장 : ").append(newRoom->GetLeader()->GetPlayer()->GetPlayerID());
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
			//이미 같은 방 제목이 있음...
			TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			SERVERtoCLIENT_CreateRoomPacketData serverToClientData;
			serverToClientData.Serialize(sendPacket,PROTOCOL::CREATE_ROOM, EXTRA::FAIL);
			player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

			Utils::GetInstance()->LogFail(L"CREATE ROOM", L"이미 존재하는 이름으로 방 생성을 시도함");
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
				message.append(L"방 이름 : ").append(room->GetName()).append(L", 참여자 : ").append(player->GetPlayerID());
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
			message.append(L"방 이름 : ").append(room->GetName()).append(L", 실행자 : ").append(player->GetPlayerID());
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

	// 1. 나를 제외한 전 인원이 레디상태인지 확인
	// 2. 모두 레디라면 방 인원 전원에게 Start 패킷을 보내주고
	// 3. 모두 레디가 아니라면 시작 불가능 패킷을 방장에게 보내줌

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		RoomPlayer* roomPlayer = PlayerManager::GetInstance()->GetRoomPlayer(player);
		Room* room = RoomManager::GetInstance()->GetJoinRoom(roomPlayer);

		if (room != NULL)
		{
			if (room->GetLeader()->GetPlayer()->GetKey() == player->GetKey())	//패킷을 보내온 주체가 방장인지 확인
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
					
					Utils::GetInstance()->LogSucc(L"ROOM START", L"게임 시작 준비 완료");

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
					Utils::GetInstance()->LogFail(L"ROOM START", L"아직 모든 플레이어가 준비되지 않음");

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
	// 방 나감 요청
	// 1. 방에 나 말고 다른 플레이어가 없다면?
	// 2. 나는 로비로 나감 그리고 로비에 있는 플레이어에게 내가 나왔다고 알림
	// 3. 그리고 방에 아무도 없으므로 방을 삭제하고 로비에 방이 삭제되었다고 알림
	// 4. 방에 나말고 다른 플레이어가 있다면?
	// 5. 그리고 만약 내가 방장이었다면?
	// 6. 내 다음 사람 또는 다음사람이 없다면 0번째 사람을 방장으로 임명
	// 7. 방에 참가한 인원에게 방장이 바뀌었다고 전송
	// 8. 이제 방에서 나를 제거하고 내가 제거되었다고 방에 알림
	// 9. 방을 나간 나에게는 로비의 정보 전송

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
				message.append(L"방 이름 : ").append(room->GetName()).append(L", 실행자 : ").append(player->GetPlayerID());
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
					message.append(L"새로운 방장 : ").append(leaderRoomPlayer->GetPlayer()->GetPlayerID());
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
			message.append(L"ID : ").append(player->GetPlayerID()).append(L", 채팅 메시지 : ").append(clientToServerData.chatString);
			Utils::GetInstance()->LogSucc(L"LOBBY CHATTING", message);
		}
	}
}
