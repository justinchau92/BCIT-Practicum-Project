#pragma once
#include "includes.h"

cTrigger *pTrigger = new cTrigger();

cTrigger::cTrigger()
{

}

cTrigger::~cTrigger()
{

}

DWORD cTrigger::getTriggerKey()
{
	return triggerKey;
}

bool TriggerToggled = false;

void cTrigger::Trigger()
{

	//Retrieve player informations
	DWORD LocalPlayer_Base = Mem->Read<DWORD>(Mem->ClientDLL_Base + playerBase); //LOCALPLAYER
	int LocalPlayer_inCross = Mem->Read<int>(LocalPlayer_Base + iCrosshairID); //CROSSHAIRID
	int LocalPlayer_Team = Mem->Read<int>(LocalPlayer_Base + teamOffset); //TEAMNUM


	//Retrieve the EntityBase, using dwEntityList
	DWORD Trigger_EntityBase = Mem->Read<DWORD>(Mem->ClientDLL_Base + entityBase + ((LocalPlayer_inCross - 1) * 0x10));
	int Trigger_EntityTeam = Mem->Read<int>(Trigger_EntityBase + teamOffset);

	//bDormant is a boolean value, the offset is 0xE9
	bool Trigger_EntityDormant = Mem->Read<bool>(Trigger_EntityBase + Dormant); //HARDCODED DORMANT

	if ((LocalPlayer_inCross > 0 && LocalPlayer_inCross <= 64) && (Trigger_EntityBase != NULL) && (Trigger_EntityTeam != LocalPlayer_Team) && (!Trigger_EntityDormant))
	{
	
		Sleep(10); //Delay before shooting
		Mem->Write<int>(Mem->ClientDLL_Base + ForceAttack, 5); //FORCEATTACK
		Sleep(10); //Delay between shots
		Mem->Write<int>(Mem->ClientDLL_Base + ForceAttack, 4); //FORCEATTACK 
		Sleep(10); //Delay after shooting
	}
	

}

