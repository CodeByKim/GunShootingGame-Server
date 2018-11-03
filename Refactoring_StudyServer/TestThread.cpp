#include "TestThread.h"

TestThread::TestThread()
{
	ThreadAttribute* attribute = new ThreadAttribute();
	attribute->SetName(L"Test");
	attribute->SetStackSize(0);
	this->SetAttribute(attribute);
}

VOID TestThread::Process()
{
	while (true)
	{
		wcout << this->GetAttribute()->GetName() << endl;
		Sleep(1);
	}
}