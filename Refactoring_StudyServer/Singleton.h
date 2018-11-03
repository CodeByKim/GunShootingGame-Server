#pragma once
#include "Library.h"

template<typename T>
class Singleton
{
private:
	static T* instance;

public:
	static T* GetInstance()
	{
		if (Singleton::instance == NULL)
		{
			Singleton::instance = new T();
		}
		return Singleton::instance;
	}

	static VOID DeleteInstance()
	{
		SAFE_DELETE(Singleton::instance);		
	}
};

template<typename T>
T* Singleton<T>::instance = NULL;