#include "HeartBeatCheckThread.h"
#include "PlayerManager.h"
#include "PacketFactory.h"
#include "GamePacketData.h"
#include "Lobby.h"

HeartBeatCheckThread::HeartBeatCheckThread()
{
	ThreadAttribute* attribute = new ThreadAttribute();
	attribute->SetName(L"HeartBeat Check Thread");
	attribute->SetStackSize(0);
	this->SetAttribute(attribute);	

	Utils::GetInstance()->LogTrace(L"HEARTBEAT THREAD", L"쓰레드 생성 완료");
}

VOID HeartBeatCheckThread::Process()
{
	while (true)
	{
		if (!this->isRun)
		{
			SetEvent(this->exitWaitEvent);
			return;
		}

		map<wstring, Player*>& players = PlayerManager::GetInstance()->GetPlayers();
		map<wstring, Player*>::iterator iter;

		for (iter = players.begin(); iter != players.end(); iter++)
		{
			TcpPacket* sendPacket = PacketFactory::GetInstance()->GetPacketPool()->Pop();
			SERVERtoCLIENT_HeartBeatPacketData data;
			data.Serialize(sendPacket, PROTOCOL::HEART_BEAT, EXTRA::SUCCESS);

			NetworkSession* session = iter->second->GetNetworkSession();

			if (!session->IsRecvHeartBeat())
			{
				PlayerManager::GetInstance()->RemovePlayer(iter->second);
				Lobby::GetInstance()->RemovePlayer(iter->second);
			}
			else
			{
				session->ResetHeartBeat();
				session->PushSendPacketToPacketQueue(sendPacket);
			}
		}		
		Sleep(5000);		
	}
}