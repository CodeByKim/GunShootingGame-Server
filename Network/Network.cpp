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
		Utils::GetInstance()->LogTrace(L"SENDER THREAD", L"쓰레드 생성 완료");
	}

	this->networkThread = new ProcessPacketThread();
	this->networkThread->Initialize(&this->locker, &this->packageQueue);

	this->heartBeatCheckThread = new HeartBeatCheckThread();
	
	ProtocolManager::GetInstance()->Initialize();		
	Utils::GetInstance()->LogTrace(L"NETWORK", L"초기화 완료");
}

VOID Network::Release()
{	
	IOCP::Release();

	Settings::DeleteInstance();

	UserDB::GetInstance()->Release();
	UserDB::DeleteInstance();
	
	PlayerManager::GetInstance()->Release();
	
	SAFE_DELETE(this->acceptor);
	Utils::GetInstance()->LogTrace(L"ACCEPTOR", L"삭제 완료");

	this->heartBeatCheckThread->Stop();
	this->heartBeatCheckThread->WaitForExit();
	SAFE_DELETE(this->heartBeatCheckThread);
	Utils::GetInstance()->LogTrace(L"HEARTBEAT THREAD", L"쓰레드 삭제 완료");

	this->networkThread->Stop();
	this->networkThread->WaitForExit();
	SAFE_DELETE(this->networkThread);
	Utils::GetInstance()->LogTrace(L"PROCESS PACKET THREAD", L"쓰레드 삭제 완료");

	for (INT i = 0; i < (INT)senderThreads.size(); i++)
	{
		SenderThread* thread = this->senderThreads[i];
		thread->Stop();
		thread->WaitForExit();
		SAFE_DELETE(thread);
		Utils::GetInstance()->LogTrace(L"SENDER THREAD", L"쓰레드 삭제 완료");
	}

	for (INT i = 0; i < (INT)packageQueue.size(); i++)
		packageQueue.pop();
	
	PacketFactory::GetInstance()->Release();
	Utils::GetInstance()->LogTrace(L"NETWORK", L"서버 종료");
}

VOID Network::Run()
{
	IOCP::Run();

	for (INT i = 0; i < PROCESSOR_COUNT; i++)
	{
		this->senderThreads[i]->Start();
		Utils::GetInstance()->LogTrace(L"SENDER THREAD", L"쓰레드 실행");
	}

	this->networkThread->Start();
	Utils::GetInstance()->LogTrace(L"PROCESS PACKET THREAD", L"쓰레드 실행");

	this->heartBeatCheckThread->Start();
	Utils::GetInstance()->LogTrace(L"HEARTBEAT THREAD", L"쓰레드 실행");
	Utils::GetInstance()->LogTrace(L"NETWORK", L"서버 시작");
}

VOID Network::RegisterAcceptSession(TcpSession* acceptSession)
{
	wstring playerKey = Utils::GetInstance()->GenerateUUID();
	Player* newPlayer = new Player(acceptSession, playerKey);
	PlayerManager::GetInstance()->AddPlayer(newPlayer);
	  
	this->RegisterSessionToIOCP(newPlayer);
	acceptSession->SetConnected();
	Utils::GetInstance()->LogTrace(L"NETWORK", L"새로운 세션이 접속 함");
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