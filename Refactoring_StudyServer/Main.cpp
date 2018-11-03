#include "Library.h"
#include "Network.h"

int main()
{			
	Network::GetInstance()->Initialize();
	Network::GetInstance()->Run();
	getchar();
	
	Network::GetInstance()->Release();	
	Network::DeleteInstance();
				
	return 0;
}