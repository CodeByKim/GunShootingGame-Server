#include "Utils.h"
#include <fstream>

wstring Utils::GenerateUUID()
{	
	CHAR* uuid = new CHAR[UUID_LENGTH];
	uuid4_generate(uuid);
	wstring convertUUID = AnsiToUTF16(uuid);
	delete[] uuid;
	return convertUUID;
}

VOID Utils::ReadAllText(string* data, wstring uri)
{
	ifstream in(uri);	
	if (in.is_open())
	{
		// 위치 지정자를 파일 끝으로 옮긴다.
		in.seekg(0, ios::end);
		// 그리고 그 위치를 읽는다. (파일의 크기)
		INT size = (INT)in.tellg();
		// 그 크기의 문자열을 할당한다.
		data->resize(size);
		// 위치 지정자를 다시 파일 맨 앞으로 옮긴다.
		in.seekg(0, ios::beg);
		// 파일 전체 내용을 읽어서 문자열에 저장한다.
		in.read(&(*data)[0], size);
	}
}

VOID Utils::SaveAllText(string data, wstring uri)
{
	ofstream out(uri);

	if (out.is_open())
	{
		out << data.c_str();
		out.close();
	}
}

wstring Utils::AnsiToUTF16(string source)
{
	wstring convertString = L"";
	convertString = CA2W(source.c_str());
	return convertString;
}

string Utils::UTF16toAnsi(wstring source)
{	
	string convertString = "";
	convertString = CW2A(source.c_str());	
	return convertString;
}

string Utils::UTF16toUTF8(wstring source)
{
	string convertString = "";	
	convertString = CW2A(source.c_str(), CP_UTF8);
	return convertString;
}

wstring Utils::UTF8toUTF16(string source)
{
	wstring convertString = L"";
	convertString = CA2W(source.c_str(), CP_UTF8);
	return convertString;
}

//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
/*
0 = 검정색,           1 = 파란색,          2 = 초록색
3 = 옥색,             4 = 빨간색,          5 = 자주색
6 = 노란색,           7 = 흰색,            8 = 회색
9 = 연한 파란색,     10 = 연한 초록색,    11 = 연한 옥색
12 = 연한 빨간색,    13 = 연한 자주색,    14 = 연한 노란색
15 = 진한 흰색
*/

VOID Utils::LogTrace(wstring target, wstring message)
{
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	wcout << setw(10) << L"[TRACE]";
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	wcout << setw(30) << target;
	wcout.setf(ios::left);
	wcout << setw(30) << message << endl;
}

VOID Utils::LogError(wstring target, wstring message)
{
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	wcout << setw(10) << L"[ERROR]";
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	wcout << setw(30) << target;
	wcout.setf(ios::left);
	wcout << setw(30) << message << endl;
}

VOID Utils::LogFail(wstring target, wstring message)
{
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	wcout << setw(10) << L"[FAIL]";
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	wcout << setw(30) << target;
	wcout.setf(ios::left);
	wcout << setw(30) << message << endl;
}

VOID Utils::LogSucc(wstring target, wstring message)
{
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	wcout << setw(10) << L"[SUCCESS]";
	wcout.setf(ios::left);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	wcout << setw(30) << target;
	wcout.setf(ios::left);
	wcout << setw(30) << message << endl;
}