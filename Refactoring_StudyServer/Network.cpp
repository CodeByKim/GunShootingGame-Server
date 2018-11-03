#include "Network.h"
#include "TcpPacket.h"
#include "ProtocolManager.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Lobby.h"
#include "Settings.h"
#include "UserDB.h"
#include "PacketFactory.h"

VOID Network::Initialize()
{
	Settings::GetInstance()->Initialize();	
	UserDB::GetInstance()->Initialize();	

	IOCP::Initialize();

	this->acceptor = new TcpAcceptor();
	this->acceptor->Bind(INADDR_ANY, Settings::GetInstance()->GetServerPort());
	this->acceptor->Listen(100);	
	this->acceptor->Run();	

	this->locker.Initialize();
	PacketFactory::GetInstance()->Initialize();	
	PlayerManager::GetInstance()->Initialize();
	Lobby::GetInstance()->Initialize();	
	
	for (INT i = 0; i < PROCESSOR_COUNT; i++)
	{
		SenderThread* senderThread = new SenderThread(i);
		this->senderThreads.push_back(senderThread);
		Utils::GetInstance()->LogTrace(L"SENDER THREAD", L"������ ���� �Ϸ�");
	}

	this->networkThread = new ProcessPacketThread();
	this->networkThread->Initialize(&this->locker, &this->packageQueue);

	this->heartBeatCheckThread = new HeartBeatCheckThread();
	
	ProtocolManager::GetInstance()->Initialize();		
	Utils::GetInstance()->LogTrace(L"NETWORK", L"�ʱ�ȭ �Ϸ�");
}

VOID Network::Release()
{	
	IOCP::Release();

	Settings::DeleteInstance();

	UserDB::GetInstance()->Release();
	UserDB::DeleteInstance();
	
	PlayerManager::GetInstance()->Release();
	
	SAFE_DELETE(this->acceptor);
	Utils::GetInstance()->LogTrace(L"ACCEPTOR", L"���� �Ϸ�");

	this->heartBeatCheckThread->Stop();
	this->heartBeatCheckThread->WaitForExit();
	SAFE_DELETE(this->heartBeatCheckThread);
	Utils::GetInstance()->LogTrace(L"HEARTBEAT THREAD", L"������ ���� �Ϸ�");

	this->networkThread->Stop();
	this->networkThread->WaitForExit();
	SAFE_DELETE(this->networkThread);
	Utils::GetInstance()->LogTrace(L"PROCESS PACKET THREAD", L"������ ���� �Ϸ�");

	for (INT i = 0; i < (INT)senderThreads.size(); i++)
	{
		SenderThread* thread = this->senderThreads[i];
		thread->Stop();
		thread->WaitForExit();
		SAFE_DELETE(thread);
		Utils::GetInstance()->LogTrace(L"SENDER THREAD", L"������ ���� �Ϸ�");
	}

	for (INT i = 0; i < (INT)packageQueue.size(); i++)
		packageQueue.pop();
	
	PacketFactory::GetInstance()->Release();
	Utils::GetInstance()->LogTrace(L"NETWORK", L"���� ����");
}

VOID Network::Run()
{
	IOCP::Run();

	for (INT i = 0; i < PROCESSOR_COUNT; i++)
	{
		this->senderThreads[i]->Start();
		Utils::GetInstance()->LogTrace(L"SENDER THREAD", L"������ ����");
	}

	this->networkThread->Start();
	Utils::GetInstance()->LogTrace(L"PROCESS PACKET THREAD", L"������ ����");

	this->heartBeatCheckThread->Start();
	Utils::GetInstance()->LogTrace(L"HEARTBEAT THREAD", L"������ ����");
	Utils::GetInstance()->LogTrace(L"NETWORK", L"���� ����");
}

VOID Network::RegisterAcceptSession(TcpSession* acceptSession)
{
	wstring playerKey = Utils::GetInstance()->GenerateUUID();
	Player* newPlayer = new Player(acceptSession, playerKey);
	PlayerManager::GetInstance()->AddPlayer(newPlayer);
	  
	this->RegisterSessionToIOCP(newPlayer);
	acceptSession->SetConnected();
	Utils::GetInstance()->LogTrace(L"NETWORK", L"���ο� ������ ���� ��");
}

VOID Network::PutPackage(Package* package)
{
	locker.Lock();
	this->packageQueue.push(package);
	locker.Unlock();
}

VOID Network::BroadCastPacket(vector<Package*>& packages)
{
	for (INT i = 0; i < (INT)packages.size(); i++)
	{
		if (packages[i]->player->IsEnterToGame())
		{
			packages[i]->player->GetNetworkSession()->PushSendPacketToPacketQueue(packages[i]->packet);
		}
	}
}

VOID Network::BroadCastPacket(vector<Package*>& packages, Player* exceptPlayer)
{
	for (INT i = 0; i < (INT)packages.size(); i++)
	{
		if (exceptPlayer->GetKey() != packages[i]->player->GetKey())
		{
			if (packages[i]->player->IsEnterToGame())
			{
				packages[i]->player->GetNetworkSession()->PushSendPacketToPacketQueue(packages[i]->packet);
			}
		}
	}
}