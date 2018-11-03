#pragma once
#include "Library.h"
#include "JsonManager.h"

class Settings : public Singleton<Settings>
{
private:
	Json::Value root;

	INT serverPort;
	INT maxUser;

public:
	VOID Initialize();
	INT GetServerPort();
	INT GetMaxUser();
};