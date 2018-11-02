#include "Library.h"
#include "IocpWorkerThread.h"
#include "NetworkSession.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Package.h"
#include "Network.h"
#include "Lobby.h"

IocpWorkerThread::IocpWorkerThread()
{
	ThreadAttribute* attribute = new ThreadAttribute();
	attribute->SetName(L"IocpWorkerThread");
	attribute->SetStackSize(0);
	this->SetAttribute(attribute);	
}

VOID IocpWorkerThread::SetIocpHandle(HANDLE iocpHandle)
{
	this->iocpHandle = iocpHandle;
}

VOID IocpWorkerThread::Process()
{
	BOOL successed = FALSE;
	DWORD numberOfByteTransfered = 0;
	VOID* completionKey = NULL;
	Player* player = NULL;
	OVERLAPPED* overlapped = NULL;
	OVERLAPPED_EX* overlappedEx = NULL;
	VOID* object = NULL;

	while (TRUE)
	{
		if (!this->isRun)
		{			
			return;
		}

		successed = GetQueuedCompletionStatus(
			this->iocpHandle,
			&numberOfByteTransfered,
			(ULONG*)&completionKey,
			&overlapped,
			INFINITE);

		overlappedEx = (OVERLAPPED_EX*)overlapped;
		object = overlappedEx->object;
		player = (Player*)completionKey;

		if (!successed || (successed && !numberOfByteTransfered))
		{
			if (overlappedEx->ioType == IO_OPERATION_TYPE::IO_ACCEPT)
			{
				
			}
			else
			{
				
			}
			wcout << L"[Disconnected] : " << player->GetKey() << endl;

			PlayerManager::GetInstance()->RemovePlayer(player);
			Lobby::GetInstance()->RemovePlayer(player);
			continue;
		}

		switch (overlappedEx->ioType)
		{
		case IO_OPERATION_TYPE::IO_RECEIVE:						
			player->GetNetworkSession()->OnReceive(numberOfByteTransfered);
			player->PutPackageToNetwork();
			
			break;
		case IO_OPERATION_TYPE::IO_SEND:
			player->GetNetworkSession()->OnSent();
			break;
		}
	}
}