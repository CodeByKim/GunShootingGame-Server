#pragma once
#include "Library.h"
#include "TcpAcceptor.h"
#include "IOCP.h"
#include "TcpSession.h"
#include "Locker.h"
#include "Package.h"
#include "ProcessPacketThread.h"
#include "SenderThread.h"
#include "HeartBeatCheckThread.h"

class Network : public IOCP, public Singleton<Network>
{
private:	
	TcpAcceptor* acceptor;
	queue<Package*> packageQueue;
	Locker locker;
	ProcessPacketThread* networkThread;
	HeartBeatCheckThread* heartBeatCheckThread;
	vector<SenderThread*> senderThreads;
	
public:	
	virtual VOID Initialize();
	virtual VOID Release();

	virtual VOID Run();

	VOID RegisterAcceptSession(TcpSession* acceptSession);
	VOID PutPackage(Package* package);
	VOID BroadCastPacket(vector<Package*>& packages);
	VOID BroadCastPacket(vector<Package*>& packages, Player* exceptPlayer);
};