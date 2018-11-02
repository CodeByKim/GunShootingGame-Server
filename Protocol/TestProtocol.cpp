#include "Protocol.h"
#include "UserDB.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Lobby.h"
#include "Utils.h"
#include "Network.h"
#include "TestPacketData.h"
#include "PacketFactory.h"

VOID KOREAN_STRING_TEST(Player* player, TcpPacket* packet)
{
	CLIENTtoSERVER_KoreanStringPacketData recvData;
	recvData.Deserialize(packet);

	wcout << recvData.testString1 << endl;
	wcout << recvData.testString2 << endl;

	TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
	SERVERtoCLIENT_KoreanStringPacketData sendData;
	sendData.WriteData(L"이것은 응답", L"이것은 다른 응답");
	sendData.Serialize(sendPacket, PROTOCOL::KOREAN_STRING_TEST, EXTRA::SUCCESS);	
	player->GetNetworkSession()->PushSendPacketToPacketQueue(sendPacket);
}