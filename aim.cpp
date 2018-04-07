#pragma once
#include "includes.h"

aim *pAim = new aim();


DWORD ClientState;
Vector myAngles;
int MaxPlayers = 32;

struct MyPlayer_t
{

	DWORD localPlayer;
	int Team;
	int Health;
	Vector View;
	Vector Position;
	float myYaw;

	void ReadPosition()
	{
		localPlayer = Mem->Read<DWORD>(Mem->ClientDLL_Base + playerBase);
		Team = Mem->Read<int>(localPlayer + teamOffset);
		Health = Mem->Read<int>(localPlayer + healthOffset);
		myYaw = Mem->Read<float>(localPlayer + dwYawPtr);
		ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(localPlayer + m_vecOrigin), &Position, sizeof(float[3]), 0);
		ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(localPlayer + m_vecViewOffset), &View, sizeof(float[3]), 0);

	}


}LocalPlayer;


struct targetList_t
{
	float Distance;
	float aimDistance;
	Vector aimAngle;

	targetList_t()
	{

	}

	targetList_t(Vector* AimAngle, Vector* localCoords, Vector* entityCoords, float aimD)
	{
		Distance = getDistances(localCoords, entityCoords);
		aimDistance = aimD;
		aimAngle.x = AimAngle->x;
		aimAngle.y = AimAngle->y;
		aimAngle.z = AimAngle->z;
	}

	float getDistances(Vector* localCoords, Vector* entityCoords)
	{
		return sqrt(
			pow(double(entityCoords->x - localCoords->x), 2.0) +
			pow(double(entityCoords->y - localCoords->y), 2.0) +
			pow(double(entityCoords->z - localCoords->z), 2.0));

	}


};

struct EntityPlayer_t
{
	DWORD entityPlayer;
	int Team;
	int Health;
	Vector aimAngle;
	Vector Position;
	float enemyYaw;

	void ReadPosition(int entity) {
		entityPlayer = Mem->Read<DWORD>(Mem->ClientDLL_Base + entityBase + (entity * 0x10));
		Team = Mem->Read<int>(entityPlayer + teamOffset);
		Health = Mem->Read<int>(entityPlayer + healthOffset);
		enemyYaw = Mem->Read<float>(entityPlayer + dwYawPtr);
		ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(entityPlayer + m_vecOrigin), &Position, sizeof(float[3]), 0);
	}

}EntityPlayer[32];



aim::aim() {

}

aim::~aim()
{

}


void aim::ReadMem()
{
	/*
	localPlayer = Mem->Read<DWORD>(Mem->ClientDLL_Base + playerBase);
	localPos = Mem->Read<float>(localPlayer + m_vecOrigin);
	localView = Mem->Read<float>(localPlayer + m_vecViewOffset);

	Entity = Mem->Read<DWORD>(Mem->ClientDLL_Size + entityBase + 0x50);
	enemyPos = Mem->Read<Vector>(Entity + m_vecOrigin);
	enemyView = Mem->Read<float>(Entity + m_vecViewOffset);

	ClientState = Mem->Read<DWORD>(Mem->EngineDLL_Base + clientState);
	angles = Mem->Read<Vector>(ClientState + dwClientState_ViewAngles);*/

	//Stored in structures instead to get list of targets
	/**
	ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(Mem->ClientDLL_Base + playerBase), &localPlayer, sizeof(DWORD), 0);
	ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(localPlayer + m_vecOrigin), &localPos, sizeof(localPos), 0);
	ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(localPlayer + m_vecViewOffset), &localView, sizeof(localPos), 0);

	ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(Mem->ClientDLL_Base + entityBase + 0x10), &Entity, sizeof(Entity), 0);
	ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(Entity + m_vecOrigin), &enemyPos, sizeof(enemyPos), 0);
	ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(Entity + m_vecViewOffset), &enemyView, sizeof(enemyView), 0);*/

	ReadProcessMemory(Mem->GetProchandle(), (PBYTE*)(Mem->EngineDLL_Base + clientState), &ClientState, sizeof(ClientState), 0);


}

struct SortEnemies_t
{
	bool operator() (targetList_t & lhs, targetList_t & rhs)
	{
		return lhs.aimDistance < rhs.aimDistance;
	}
}SortEnemies;

Vector aim::calcAngle(Vector* src, Vector* dst)
{
	Vector angle;
	Vector newAngle;

	angle.x = dst->x - src->x;
	angle.y = dst->y - src->y;
	angle.z = dst->z - src->z;

	float hyp = sqrt((angle.x * angle.x) + (angle.y * angle.y) + (angle.z * angle.z));

	newAngle.y = atan2(angle.y, angle.x) * 180 / 3.14;
	newAngle.x = -atan2(angle.z, hyp) * 180 / 3.14;
	newAngle.z = 0;



	return newAngle;
}

float aimDistance(Vector * Local, Vector * Enemy)
{
	float Distance;
	float DistanceX;
	float DistanceY;

	DistanceX = (Enemy->x - Local->x) * (Enemy->x - Local->x);
	DistanceY = (Enemy->y - Local->y) * (Enemy->y - Local->y);
	Distance = sqrt(DistanceX + DistanceY);


	return Distance;
}



void aim::aimAssist(float FOV)
{
	float FOVs = FOV;
	targetList_t* TargetList = new targetList_t[MaxPlayers];
	ReadMem();
	LocalPlayer.ReadPosition();
	Vector currentViewAngle;
	float Distance;



	int targetLoop = 0;

	for (int i = 0; i < MaxPlayers; i++)
	{
		EntityPlayer[i].ReadPosition(i); //CORRECT READINGS...

		if (EntityPlayer[i].Team == LocalPlayer.Team)
			continue;

		if (EntityPlayer[i].Health < 2)
			continue;

		EntityPlayer[i].aimAngle = calcAngle(&LocalPlayer.Position, &EntityPlayer[i].Position);
		currentViewAngle.x = Mem->Read<float>(ClientState + dwClientState_ViewAngles);
		currentViewAngle.y = Mem->Read<float>(ClientState + dwClientState_ViewAngles + 0x4);

		Distance = aimDistance(&currentViewAngle, &EntityPlayer[i].aimAngle);



		//cout << currentViewAngle.y << endl;

		TargetList[targetLoop] = targetList_t(&EntityPlayer[i].aimAngle, &LocalPlayer.Position, &EntityPlayer[i].Position, Distance);
		targetLoop++;

	}


	if (targetLoop > 0)
	{
		std::sort(TargetList, TargetList + targetLoop, SortEnemies);
		cout << TargetList[0].aimDistance << endl;

		if (GetAsyncKeyState(0x2))
		{
			if (TargetList[0].aimDistance < FOVs || TargetList[0].aimDistance >(360 - FOV))
			{

				WriteProcessMemory(Mem->GetProchandle(), (PBYTE*)(ClientState + dwClientState_ViewAngles), &TargetList[0].aimAngle.x, sizeof(float), 0);
				WriteProcessMemory(Mem->GetProchandle(), (PBYTE*)(ClientState + dwClientState_ViewAngles + 0x4), &TargetList[0].aimAngle.y, sizeof(float), 0);
			}




		}
	}

	targetLoop = 0;

	delete[] TargetList;


}



