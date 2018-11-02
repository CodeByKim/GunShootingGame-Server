#pragma once
#include "Library.h"
#include "MemoryStream.h"

/*
데이터
1. INT : 4바이트
2. FLOAT : 4바이트
3. string : 문자열 길이 정보(4바이트) + 실제 문자열
*/

class TcpPacketHeader
{
public:
	INT length;
	INT protocol;
	INT extra;	//SUCC, FAIL, REQUEST, RESPONE 등등... 
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