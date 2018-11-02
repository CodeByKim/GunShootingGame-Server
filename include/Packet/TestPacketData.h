#pragma once
#include "Library.h"
#include "TcpPacket.h"

class CLIENTtoSERVER_KoreanStringPacketData : public BasePacketData
{
public:
	wstring testString1;
	wstring testString2;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_KoreanStringPacketData : public BasePacketData
{
public:
	wstring testString1;
	wstring testString2;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
	VOID WriteData(wstring testString1, wstring testString2);
};