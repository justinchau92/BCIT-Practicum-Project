#include "includes.h"


bHop *pBhop = new bHop();

bHop::bHop()
{

}

bHop::~bHop()
{

}


void bHop::autobHop()
{

	while (true)
	{
		DWORD dwLocalPlayer = Mem->Read<DWORD>(Mem->ClientDLL_Base + playerBase); // This will hold our local player base address

		BYTE mFlag = Mem->Read<BYTE>(dwLocalPlayer + Flags);

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (mFlag & (1 << 0))
			{
				Mem->Write(Mem->ClientDLL_Base + ForceJump, 6);
			}
		}

	}

}