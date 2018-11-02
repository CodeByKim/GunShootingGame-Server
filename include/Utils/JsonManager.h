#pragma once
#include "Library.h"
#include <json\json.h>
#include <fstream>

class JsonManager : public Singleton<JsonManager>
{
private:
	Json::Reader reader;
	Json::StreamWriterBuilder writer;

public:
	VOID Load(Json::Value& value, wstring uri);
	VOID Save(Json::Value& value, wstring uri);
};