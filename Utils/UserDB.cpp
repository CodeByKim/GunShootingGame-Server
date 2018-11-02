#include "UserDB.h"

VOID UserDB::Initialize()
{
	JsonManager::GetInstance()->Load(this->root, USER_INFO_URI);	
	Utils::GetInstance()->LogTrace(L"USER DB", L"로딩, 초기화 완료");
}

BOOL UserDB::AddUser(wstring id, wstring password)
{
	if (!this->FindUser(id))
	{		
		Json::Value newUser;
		string convertID = Utils::GetInstance()->UTF16toAnsi(id);
		string convertPassword = Utils::GetInstance()->UTF16toAnsi(password);

		newUser["ID"] = convertID;
		newUser["PASSWORD"] = convertPassword;
		
		this->root["USER INFO"].append(newUser);
		return TRUE;
	}
	return FALSE;
}

BOOL UserDB::RemoveUser(wstring id, wstring password)
{
	for (INT i = 0; i < (INT)this->root["USER INFO"].size(); i++)
	{
		string findID = this->root["USER INFO"][i]["ID"].asString();
		string findPassword = this->root["USER INFO"][i]["PASSWORD"].asString();

		wstring convertFindID = Utils::GetInstance()->AnsiToUTF16(findID);
		wstring convertFindPassword = Utils::GetInstance()->AnsiToUTF16(findPassword);

		if (id.compare(convertFindID) == 0 
			&& password.compare(convertFindPassword) == 0)
		{
			this->root["USER INFO"].removeIndex(i, NULL);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL UserDB::FindUser(wstring id)
{
	for (INT i = 0; i < (INT)this->root["USER INFO"].size(); i++)
	{
		string findID = this->root["USER INFO"][i]["ID"].asString();		
		wstring convertFindID = Utils::GetInstance()->AnsiToUTF16(findID);

		if (id.compare(convertFindID) == 0)
			return TRUE;
	}

	return FALSE;
}

BOOL UserDB::FindUser(wstring id, wstring password)
{
	for (INT i = 0; i < (INT)this->root["USER INFO"].size(); i++)
	{
		string findID = this->root["USER INFO"][i]["ID"].asString();
		string findPassword = this->root["USER INFO"][i]["PASSWORD"].asString();

		wstring convertFindID = Utils::GetInstance()->AnsiToUTF16(findID);
		wstring convertFindPassword = Utils::GetInstance()->AnsiToUTF16(findPassword);

		if (id.compare(convertFindID) == 0 
			&& password.compare(convertFindPassword) == 0)
			return TRUE;
	}

	return FALSE;
}

VOID UserDB::Release()
{
	JsonManager::GetInstance()->Save(this->root, USER_INFO_URI);
}