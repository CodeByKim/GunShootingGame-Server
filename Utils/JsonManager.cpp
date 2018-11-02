#include "JsonManager.h"
#include "Utils.h"

VOID JsonManager::Load(Json::Value& value, wstring uri)
{	
	string rawJsonData;
	Utils::GetInstance()->ReadAllText(&rawJsonData, uri);		
	reader.parse(rawJsonData, value);	
}

VOID JsonManager::Save(Json::Value& value, wstring uri)
{
	string data = value.toStyledString();
	Utils::GetInstance()->SaveAllText(data, uri);	
}