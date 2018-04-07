#include "includes.h"


bHop *pBhop = new bHop();

bHop::bHop()
{

}

bHop::~bHop()
{

}

int bHop::inAir()
{
	DWORD dwLocalPlayer = Mem->Read<DWORD>(Mem->ClientDLL_Base + playerBase);
	int mFlag = Mem->Read<int>(dwLocalPlayer + Flags);
	return mFlag == 256 || mFlag == 258 || mFlag == 260 || mFlag == 262;

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


		/**if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
		if (!bHop::inAir())
		{
		Mem->Write<int>(dwLocalPlayer + ForceJump, 5);
		}
		else if (bHop::inAir())
		{
		Mem->Write<int>(dwLocalPlayer + ForceJump, 4);
		Mem->Write<int>(dwLocalPlayer + ForceJump, 5);
		Mem->Write<int>(dwLocalPlayer + ForceJump, 4);
		}
		else
		{
		Mem->Write<int>(dwLocalPlayer + ForceJump, 4);
		}

		}*/



	}

}