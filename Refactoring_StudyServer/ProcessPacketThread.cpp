#include "ProcessPacketThread.h"
#include "ProtocolManager.h"

ProcessPacketThread::ProcessPacketThread()
{
	ThreadAttribute* attribute = new ThreadAttribute();
	attribute->SetName(L"NetworkRunning");
	attribute->SetStackSize(0);
	this->SetAttribute(attribute);	
}

VOID ProcessPacketThread::Initialize(Locker* locker, queue<Package*>* packageQueue)
{
	this->locker = locker;
	this->packageQueue = packageQueue;
	Utils::GetInstance()->LogTrace(L"PROCESS PACKET THREAD", L"쓰레드 생성 완료");
}

VOID ProcessPacketThread::Process()
{
	while (TRUE)
	{
		if (!this->isRun)
		{
			SetEvent(this->exitWaitEvent);
			return;
		}

		if (!this->packageQueue->empty())
		{
			this->locker->Lock();
			INT queueSize = (INT)this->packageQueue->size();

			for (INT i = 0; i < queueSize; i++)
			{
				Package* package = this->packageQueue->front();
				this->packageQueue->pop();
				INT protocol = package->packet->header.protocol;
				ProtocolManager::GetInstance()->Execute(protocol, package->player, package->packet);
				SAFE_DELETE(package);
			}
			this->locker->Unlock();
		}		
		Sleep(10);
	}
}