
#include "GlowESP.h"

Glow *pGlow = new Glow();

Glow::Glow()
{
}

Glow::~Glow()
{
}


void Glow::GlowESP()
{
	DWORD dwLocalPlayer = Mem->Read<DWORD>(Mem->ClientDLL_Base + playerBase);
	DWORD dwEntityPlayer = Mem->Read<DWORD>(Mem->ClientDLL_Base + entityBase);
	int glowPointer = Mem->Read<DWORD>(Mem->ClientDLL_Base + GlowObject);


	for (int i = 0; i <= 32; i++)
	{
		int glow_currentPlayer = Mem->Read<int>(Mem->ClientDLL_Base + entityBase + i * 0x10);
		bool glow_currentPlayerDormant = Mem->Read<bool>(glow_currentPlayer + Dormant);

		int glow_currentPlayerGlowIndex = Mem->Read<int>(glow_currentPlayer + GlowIndex);
		int EntityPlayerTeamID = Mem->Read<int>(glow_currentPlayer + teamOffset);


		if (glow_currentPlayerDormant == true || EntityPlayerTeamID == false)
			continue;

		else
		{
			if (Mem->Read<int>(dwLocalPlayer + teamOffset) != EntityPlayerTeamID || glowTeamCheck == false)
				switch (EntityPlayerTeamID)
				{
				case 2:
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)), TRed);
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)), TGreen);
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)), TBlue);
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)), TAlpha);
					Mem->Write<bool>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)), true);
					Mem->Write<bool>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)), false);
					break;
				case 3:
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)), CTRed);
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)), CTGreen);
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)), CTBlue);
					Mem->Write<float>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)), CTAlpha);
					Mem->Write<bool>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)), true);
					Mem->Write<bool>((glowPointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)), false);
					break;
				}

		}


	}

}

