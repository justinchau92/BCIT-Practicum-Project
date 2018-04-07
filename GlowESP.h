#pragma once
#include "includes.h"



class Glow
{
public:
	Glow();
	~Glow();
	void GlowESP();

	float TRed = 1.f;
	float TGreen = 0.f;
	float TBlue = 0.f;
	float TAlpha = 1.f;

	float CTRed = 0.f;
	float CTGreen = 0.f;
	float CTBlue = 1.f;
	float CTAlpha = 1.f;

	bool glowActive = true;
	bool glowTeamCheck = true;

};

extern Glow *pGlow;