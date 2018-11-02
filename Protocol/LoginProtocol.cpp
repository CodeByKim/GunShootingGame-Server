#include "Protocol.h"
#include "UserDB.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Lobby.h"
#include "Utils.h"
#include "Network.h"
#include "LoginPacketData.h"
#include "PacketFactory.h"

VOID REGISTER_USER(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_RegisterUserPacketData recvData;
	recvData.Deserialize(packet);

	TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
	SERVERtoCLIENT_RegisterUserPacketData sendData;

	if (UserDB::GetInstance()->AddUser(recvData.id, recvData.password))
	{
		sendData.Serialize(sendPacket, PROTOCOL::REGISTER_USER, EXTRA::SUCCESS);
		wstring message = L"";		
		message.append(L"ID : ").append(recvData.id).append(L", Password : ").append(recvData.password);
		Utils::GetInstance()->LogSucc(L"REGISTER USER", message);
	}
	else
	{
		sendData.Serialize(sendPacket, PROTOCOL::REGISTER_USER, EXTRA::FAIL);
		Utils::GetInstance()->LogFail(L"REGISTER USER", L"이미 존재하는 아이디로 가입을 요청함");
	}

	player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
}

VOID LOGIN(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_LoginPacketData recvData;
	recvData.Deserialize(packet);

	TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
	SERVERtoCLIENT_LoginPacketData sendData;

	if (UserDB::GetInstance()->FindUser(recvData.id, recvData.password))
	{		
		if (!PlayerManager::GetInstance()->OverlapedLoginCheck(recvData.id))
		{
			Utils::GetInstance()->LogFail(L"LOGIN", L"이미 접속된 아이디");
			sendData.Serialize(sendPacket, PROTOCOL::LOGIN, EXTRA::OVERLAPED_LOGIN);
			player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
			return;
		}

		player->EnterToGame(recvData.id);
		player->SetState(PLAYER_STATE::LOBBY);
		Lobby::GetInstance()->EnterPlayer(player);
		wstring message = L"";
		message.append(L"ID : ").append(recvData.id).append(L", Password : ").append(recvData.password);
		Utils::GetInstance()->LogSucc(L"LOGIN", message);

		sendData.WriteData(player->GetKey(), recvData.id);
		sendData.Serialize(sendPacket, PROTOCOL::LOGIN, EXTRA::SUCCESS);
	}
	else
	{
		Utils::GetInstance()->LogFail(L"LOGIN", L"잘못된 아이디 또는 비밀번호로 로그인 요청");
		sendData.Serialize(sendPacket, PROTOCOL::LOGIN, EXTRA::FAIL);
	}

	player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
}