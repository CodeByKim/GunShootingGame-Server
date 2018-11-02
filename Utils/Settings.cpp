#include "Settings.h"

VOID Settings::Initialize()
{
	std::locale::global(std::locale("kor"));	
	JsonManager::GetInstance()->Load(this->root, SETTING_URI);

	this->serverPort = root["PORT"].asInt();
	this->maxUser = root["MAX USER"].asInt();	
	Utils::GetInstance()->LogTrace(L"SETTINGS", L"�ε�, �ʱ�ȭ �Ϸ�");
}

INT Settings::GetServerPort()
{
	return this->serverPort;
}

INT Settings::GetMaxUser()
{
	return this->maxUser;
}