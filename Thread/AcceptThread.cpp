#include "Library.h"
#include "AcceptThread.h"
#include "Network.h"
#include "TcpSession.h"

AcceptThread::AcceptThread()
{
	ThreadAttribute* attribute = new ThreadAttribute();
	attribute->SetName(L"AcceptThread");
	attribute->SetStackSize(0);
	this->SetAttribute(attribute);
	Utils::GetInstance()->LogTrace(L"ACCEPT THREAD", L"쓰레드 생성 완료");
}

AcceptThread::~AcceptThread()
{	
	SAFE_DELETE(this->socket);
}

VOID AcceptThread::SetListenSocket(TcpSocket* socket)
{
	this->socket = socket;
}

VOID AcceptThread::Process()
{	
	while (TRUE)
	{
		if (!this->isRun)
		{			
			return;
		}

		TcpSocket* acceptedSocket = this->socket->Accept();

		if (acceptedSocket == NULL)	
			return;

		TcpSession* acceptedSession = new TcpSession();
		acceptedSession->SetSocket(acceptedSocket);
		Network::GetInstance()->RegisterAcceptSession(acceptedSession);
	}
}