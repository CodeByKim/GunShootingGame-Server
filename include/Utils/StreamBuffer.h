#pragma once
#include "Library.h"

class StreamBuffer
{
private:
	INT remainLength;
	CHAR buffer[MAX_BUFFER_LENGTH];

public:
	StreamBuffer();

	VOID PutStream(CHAR* socketBuffer, INT length);
	CHAR* GetBuffer();

	INT GetRemainLength();
	VOID SetRemainLength(INT remainLength);
};