#include "SenderThread.h"
#include "NetworkSession.h"
#include "PlayerManager.h"

SenderThread::SenderThread(INT index)
{
	ThreadAttribute* attribute = new ThreadAttribute();
	attribute->SetName(L"SenderThread");
	attribute->SetStackSize(0);
	this->SetAttribute(attribute);

	this->index = index;	
}

VOID SenderThread::Process()
{
	while (true)
	{
		if (!this->isRun)
		{
			SetEvent(this->exitWaitEvent);
			return;
		}

		PlayerManager::GetInstance()->SendPacketQueue(this->index);
		Sleep(10);
	}
}