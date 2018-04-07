#pragma once
#include "includes.h"

struct Vector
{
	float x, y, z;

};

class aim
{

public:
	aim();
	~aim();
	void ReadMem();
	void aimAssist(float FOV);
	Vector calcAngle(Vector* src, Vector* dst);

};


extern aim *pAim;