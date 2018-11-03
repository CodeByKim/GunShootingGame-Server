#include "StreamBuffer.h"

StreamBuffer::StreamBuffer()
{
	this->remainLength = 0;
	ZeroMemory(this->buffer, MAX_BUFFER_LENGTH);
}

CHAR* StreamBuffer::GetBuffer()
{
	return this->buffer;
}

VOID StreamBuffer::PutStream(CHAR* socketBuffer, INT length)
{
	CopyMemory(this->buffer + this->remainLength, socketBuffer, length);
	this->remainLength += length;
}

INT StreamBuffer::GetRemainLength()
{
	return this->remainLength;
}

VOID StreamBuffer::SetRemainLength(INT remainLength)
{
	this->remainLength = remainLength;
}