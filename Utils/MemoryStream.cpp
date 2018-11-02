#include "MemoryStream.h"

MemoryStream::MemoryStream(CHAR* packetBuffer, INT length)
{
	this->buffer = new CHAR[length];
	CopyMemory(this->buffer, packetBuffer, length);
	this->offset = 0;
}

MemoryStream::~MemoryStream()
{
	SAFE_DELETE_ARRAY(this->buffer);
}

VOID MemoryStream::WriteInt(INT data)
{
	CopyMemory(this->buffer + this->offset, &data, sizeof(INT));
	this->offset += sizeof(INT);
}

VOID MemoryStream::WriteFloat(FLOAT data)
{
	CopyMemory(this->buffer + this->offset, &data, sizeof(FLOAT));
	this->offset += sizeof(FLOAT);
}

VOID MemoryStream::WriteChar(CHAR data)
{
	CopyMemory(this->buffer + this->offset, &data, sizeof(CHAR));
	this->offset += sizeof(CHAR);
}

VOID MemoryStream::WriteString(wstring data)
{
	string convertUTF8 = Utils::GetInstance()->UTF16toUTF8(data);
	WriteInt(convertUTF8.length());	

	for (INT i = 0; i < (INT)convertUTF8.length(); i++)
		WriteChar(convertUTF8[i]);
}

VOID MemoryStream::ReadInt(INT* data)
{
	CopyMemory(data, this->buffer + this->offset, sizeof(INT));
	this->offset += sizeof(INT);
}

VOID MemoryStream::ReadFloat(FLOAT* data)
{
	CopyMemory(data, this->buffer + this->offset, sizeof(FLOAT));
	this->offset += sizeof(FLOAT);
}

VOID MemoryStream::ReadChar(CHAR* data)
{
	CopyMemory(data, this->buffer + this->offset, sizeof(CHAR));
	this->offset += sizeof(CHAR);
}

VOID MemoryStream::ReadBOOL(BOOL* data)
{
	bool temp;		//BOOL Ÿ���� 4����Ʈ�� �̷� �۾��� ��...
	CopyMemory(&temp, this->buffer + this->offset, 1);
	*data = temp;
	this->offset += 1;
}

VOID MemoryStream::ReadString(wstring* data)
{
	INT stringLength = 0;
	ReadInt(&stringLength);		//UTF8 ����Ʈ �а�

	// +1 �ϴ� ������ UTF ���ڿ��� �ι��ڸ� �������� ������
	// UTF8toUTF16 �Լ��� ȣ���Ҷ� �ι��ڰ� ���Ե� 
	// C��Ÿ���� ���ڿ��� �ʿ��ϱ� ����
	CHAR* string = new CHAR[stringLength+1];	
	CopyMemory(string, this->buffer + this->offset, stringLength);
	string[stringLength] = '\0';
	wstring convertUTF16 = Utils::GetInstance()->UTF8toUTF16(string);

	data->clear();
	*data = convertUTF16;

	this->offset += stringLength;
	delete[] string;
}

INT MemoryStream::GetOffset()
{
	return this->offset;
}

CHAR* MemoryStream::GetBuffer()
{
	return this->buffer;
}