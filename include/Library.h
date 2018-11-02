#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <iomanip>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <process.h>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

#define MAX_BUFFER_LENGTH 4096
#define MAX_PACKET_COUNT 5000
#define MAX_PACKET_DATA_LENGTH 4096
#define PACKET_HEADER_LENGTH 12
#define MAX_NAME_LENGTH 256
#define MAX_ROOM_USER 4
#define PROCESSOR_COUNT 4

#define SAFE_DELETE(obj)\
	if (obj != NULL)\
		delete obj;\
	obj = NULL

#define SAFE_DELETE_ARRAY(arr)\
	if (arr != NULL)\
		delete[] arr;\
	arr = NULL

#if _DEBUG
#define SETTING_URI L"../Debug/Settings/Settings.json"
#define USER_INFO_URI L"../Debug/DB/UserInfo.json"
#else
#define SETTING_URI L"../Release/Settings/Settings.json"
#define USER_INFO_URI L"../Release/DB/UserInfo.json"
#endif

enum class IO_OPERATION_TYPE
{
	IO_ACCEPT,
	IO_SEND,
	IO_RECEIVE,
	IO_DISCONNECTED
};

struct OVERLAPPED_EX : OVERLAPPED
{
	OVERLAPPED overlapped;
	IO_OPERATION_TYPE ioType;
	VOID* object;
};

#include "Defines.h"
#include "Singleton.h"
#include "Utils.h"

#define UUID_LENGTH UUID4_LEN