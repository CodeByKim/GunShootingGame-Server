#pragma once
#include "Library.h"
#include "MemoryStream.h"

/*
������
1. INT : 4����Ʈ
2. FLOAT : 4����Ʈ
3. string : ���ڿ� ���� ����(4����Ʈ) + ���� ���ڿ�
*/

class TcpPacketHeader
{
public:
	INT length;
	INT protocol;
	INT extra;	//SUCC, FAIL, REQUEST, RESPONE ���... 
};

class TcpPacket
{
public:
	TcpPacketHeader header;
	MemoryStream* stream;

	virtual ~TcpPacket();

	VOID Initialize();
	VOID Initialize(CHAR* buffer, INT length);

	VOID SetHeader(INT protocol, INT extra);
	VOID SetHeader(TcpPacketHeader header);

	VOID Reset();
};

class BasePacketData
{
public:
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra) {}
	virtual VOID Deserialize(TcpPacket* packet) {}	
};