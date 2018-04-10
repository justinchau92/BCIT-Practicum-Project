#pragma once
#include "includes.h"

class bHop
{
public:
	bHop();
	~bHop();
	void autobHop();

private:
	int onGround = 257;
	DWORD bhopKey = 0x20;

};

extern bHop *pBhop;