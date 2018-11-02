#pragma once
#include "Library.h"
#include "JsonManager.h"

class UserDB : public Singleton<UserDB>
{
private:
	Json::Value root;

public:
	VOID Initialize();
	VOID Release();

	BOOL AddUser(wstring id, wstring password);
	BOOL RemoveUser(wstring id, wstring password);
	BOOL FindUser(wstring id);
	BOOL FindUser(wstring id, wstring password);
};