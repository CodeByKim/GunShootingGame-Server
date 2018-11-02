#include "Protocol.h"
#include "UserDB.h"
#include "Player.h"
#include "PlayerManager.h"
#include "RoomManager.h"
#include "Lobby.h"
#include "Utils.h"
#include "Network.h"
#include "LobbyPacketData.h"
#include "TestPacketData.h"
#include "PacketFactory.h"

VOID ENTER_LOBBY(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_EnterLobbyPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop(); 
		SERVERtoCLIENT_EnterLobbyPacketData serverToClientData;
		serverToClientData.WriteData(RoomManager::GetInstance()->GetRoomCount(), RoomManager::GetInstance()->GetRooms(), Lobby::GetInstance()->GetPlayerCount(), Lobby::GetInstance()->GetPlayers());
		serverToClientData.Serialize(sendPacket, PROTOCOL::ENTER_LOBBY, EXTRA::SUCCESS);
		player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

		wstring message = L"";
		message.append(L"ID : ").append(player->GetPlayerID());
		Utils::GetInstance()->LogSucc(L"ENTER LOBBY", message);

		vector<Package*> packages;
		vector<Player*>& players = Lobby::GetInstance()->GetPlayers();
		SERVERtoCLIENT_EnterLobbyOtherPlayerPacketData broadcastData;
		broadcastData.WriteData(player->GetPlayerID());

		for (INT i = 0; i < (INT)players.size(); i++)
		{
			TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			broadcastData.Serialize(packet, PROTOCOL::ENTER_LOBBY_OTHER_PLAYER, EXTRA::FOR_LOGIN);
			Package* package = new Package();
			package->packet = packet;
			package->player = players[i];
			packages.push_back(package);
		}

		Network::GetInstance()->BroadCastPacket(packages, player);
	}
}

VOID LEAVE_LOBBY(Player* player, TcpPacket* packet)
{		
	CLIENTtoSERVER_LeaveLobbyPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{						
		Lobby::GetInstance()->LeavePlayer(player);		

		TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
		SERVERtoCLIENT_LeaveLobbyPacketData serverToClientData;
		serverToClientData.Serialize(sendPacket, PROTOCOL::LEAVE_LOBBY, EXTRA::SUCCESS);
		player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);

		wstring message = L"";
		message.append(L"ID : ").append(player->GetPlayerID());
		Utils::GetInstance()->LogSucc(L"LEAVE LOBBY", message);

		vector<Package*> packages;
		vector<Player*>& players = Lobby::GetInstance()->GetPlayers();
		SERVERtoCLIENT_LeaveLobbyOtherPlayerPacketData broadcastData;
		broadcastData.WriteData(player->GetPlayerID());

		for (INT i = 0; i < (INT)players.size(); i++)
		{
			TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			broadcastData.Serialize(packet, PROTOCOL::LEAVE_LOBBY_OTHER_PLAYER, EXTRA::FOR_LOGOUT);
			Package* package = new Package();
			package->packet = packet;
			package->player = players[i];
			packages.push_back(package);
		}

		Network::GetInstance()->BroadCastPacket(packages, player);
	}
}

VOID LOBBY_CHATTING(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_LobbyChattingPacketData clientToServerData;
	clientToServerData.Deserialize(packet);

	if (PlayerManager::GetInstance()->FindPlayer(clientToServerData.playerKey))
	{
		wstring message = L"";
		message.append(L"ID : ").append(player->GetPlayerID()).append(L", 채팅 메시지 : ").append(clientToServerData.chatString);
		Utils::GetInstance()->LogSucc(L"LOBBY CHATTING", message);

		vector<Package*> packages;
		vector<Player*>& players = Lobby::GetInstance()->GetPlayers();
		SERVERtoCLIENT_LobbyChattingPacketData broadcastData;
		broadcastData.WriteData(player->GetPlayerID(), clientToServerData.chatString);

		for (INT i = 0; i < (INT)players.size(); i++)
		{
			TcpPacket* packet = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			broadcastData.Serialize(packet, PROTOCOL::LOBBY_CHATTING, EXTRA::SUCCESS);
			Package* package = new Package();
			package->packet = packet;
			package->player = players[i];
			packages.push_back(package);
		}

		Network::GetInstance()->BroadCastPacket(packages);
	}
}