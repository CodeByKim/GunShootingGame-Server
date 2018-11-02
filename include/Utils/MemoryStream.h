#pragma once
#include "Library.h"

class MemoryStream
{
private:
	INT offset;
	CHAR* buffer;

public:
	MemoryStream(CHAR* packetBuffer, INT length);
	virtual ~MemoryStream();

	VOID WriteInt(INT data);
	VOID WriteFloat(FLOAT data);
	VOID WriteChar(CHAR data);
	VOID WriteString(wstring data);

	VOID ReadInt(INT* data);
	VOID ReadFloat(FLOAT* data);
	VOID ReadChar(CHAR* data);
	VOID ReadBOOL(BOOL* data);
	VOID ReadString(wstring* data);

	INT GetOffset();
	CHAR* GetBuffer();
};
