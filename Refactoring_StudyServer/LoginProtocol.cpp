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
		Utils::GetInstance()->LogFail(L"REGISTER USER", L"�̹� �����ϴ� ���̵�� ������ ��û��");
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
			Utils::GetInstance()->LogFail(L"LOGIN", L"�̹� ���ӵ� ���̵�");
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
		Utils::GetInstance()->LogFail(L"LOGIN", L"�߸��� ���̵� �Ǵ� ��й�ȣ�� �α��� ��û");
		sendData.Serialize(sendPacket, PROTOCOL::LOGIN, EXTRA::FAIL);
	}

	player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
}