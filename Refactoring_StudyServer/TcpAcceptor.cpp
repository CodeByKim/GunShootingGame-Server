#include "Library.h"
#include "TcpAcceptor.h"
#include "TcpSocket.h"
#include "AcceptThread.h"

TcpAcceptor::TcpAcceptor()
{
	this->socket = new TcpSocket();			
	Utils::GetInstance()->LogTrace(L"ACCEPTOR", L"积己 肯丰");
}

TcpAcceptor::~TcpAcceptor()
{
	this->acceptThread->Stop();	
	SAFE_DELETE(this->acceptThread);	
}

VOID TcpAcceptor::Listen(INT backlog)
{	
	TcpSocket* socket = reinterpret_cast<TcpSocket*>(this->socket);
	socket->Listen(backlog);
	Utils::GetInstance()->LogTrace(L"ACCEPTOR", L"府郊");
}

VOID TcpAcceptor::Run()
{
	this->acceptThread = new AcceptThread();
	TcpSocket* socket = reinterpret_cast<TcpSocket*>(this->socket);
	this->acceptThread->SetListenSocket(socket);
	this->acceptThread->Start();		
	Utils::GetInstance()->LogTrace(L"ACCEPTOR", L"角青");
}