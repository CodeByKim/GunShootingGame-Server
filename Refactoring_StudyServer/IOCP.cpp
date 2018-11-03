#include "IOCP.h"

VOID IOCP::Initialize()
{
	this->iocpHandle = NULL;
	this->workerThreadCount = 0;

	WSAStartup(MAKEWORD(2, 2), &this->wsaData);

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	this->workerThreadCount = systemInfo.dwNumberOfProcessors;

	this->iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, this->workerThreadCount);

	for (INT i = 0; i < this->workerThreadCount; i++)
	{
		IocpWorkerThread* workerThread = new IocpWorkerThread();
		workerThread->SetIocpHandle(this->iocpHandle);
		this->workerThreads.push_back(workerThread);
		Utils::GetInstance()->LogTrace(L"IOCP WORKER THREAD", L"쓰레드 생성 완료");
	}

	Utils::GetInstance()->LogTrace(L"IOCP", L"초기화 완료");
}

VOID IOCP::Release()
{
	for (INT i = 0; i < this->workerThreadCount; i++)
	{
		this->workerThreads[i]->Stop();		
		SAFE_DELETE(this->workerThreads[i]);
		Utils::GetInstance()->LogTrace(L"IOCP WORKER THREAD", L"쓰레드 삭제 완료");
	}

	this->workerThreads.clear();
	this->iocpHandle = NULL;
	this->workerThreadCount = 0;
	WSACleanup();
	Utils::GetInstance()->LogTrace(L"IOCP", L"리소스 해제 완료");
}

VOID IOCP::Run()
{
	for (INT i = 0; i < (INT)this->workerThreads.size(); i++)
	{
		this->workerThreads[i]->Start();		
		Utils::GetInstance()->LogTrace(L"IOCP WORKER THREAD", L"쓰레드 실행");
	}
	Utils::GetInstance()->LogTrace(L"IOCP", L"실행");
}

VOID IOCP::RegisterSessionToIOCP(Player* player)
{
	this->iocpHandle = CreateIoCompletionPort((HANDLE)player->GetNetworkSession()->GetSocket()->GetSocketHandle(),
		this->iocpHandle, (ULONG_PTR)player, 0);
}

HANDLE IOCP::GetHandle()
{
	return this->iocpHandle;
}