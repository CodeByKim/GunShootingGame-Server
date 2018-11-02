#pragma once
#include "Library.h"
#include "TcpPacket.h"

#pragma region Register
class CLIENTtoSERVER_RegisterUserPacketData : public BasePacketData
{
public:
	wstring id;
	wstring password;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_RegisterUserPacketData : public BasePacketData
{
public:
	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
};
#pragma endregion

#pragma region Login
class CLIENTtoSERVER_LoginPacketData : public BasePacketData
{
public:
	wstring id;
	wstring password;

	virtual VOID Deserialize(TcpPacket* packet);
};

class SERVERtoCLIENT_LoginPacketData : public BasePacketData
{
public:
	wstring playerKey;
	wstring playerID;

	virtual VOID Serialize(TcpPacket* packet, PROTOCOL protocol, EXTRA extra);
			VOID WriteData(wstring playerKey, wstring playerID);
};
#pragma endregion