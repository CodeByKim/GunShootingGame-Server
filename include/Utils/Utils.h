#pragma once
#include "Library.h"
#include <atlstr.h>
#include <locale>

extern "C" {
#include "uuid4.h"
}

class Utils : public Singleton<Utils>
{
public:
	wstring GenerateUUID();
	VOID ReadAllText(string* data, wstring uri);
	VOID SaveAllText(string data, wstring uri);

	wstring AnsiToUTF16(string source);
	string UTF16toAnsi(wstring source);
	string UTF16toUTF8(wstring source);
	wstring UTF8toUTF16(string source);

	VOID LogTrace(wstring target, wstring message);
	VOID LogError(wstring target, wstring message);
	VOID LogFail(wstring target, wstring message);
	VOID LogSucc(wstring target, wstring message);
};