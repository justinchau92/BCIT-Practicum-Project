#pragma once
#include "includes.h"
#include <Windows.h>
#include <Tlhelp32.h>
#include <string>


const DWORD clientState = 0x57D894;
const DWORD dwClientState_ViewAngles = 0x4D10;
const DWORD iCrosshairID = 0xB2A4;

//players
const DWORD playerBase = 0xAA6614;
const DWORD entityBase = 0x4A8387C;

const DWORD Flags = 0x100;
const DWORD ForceJump = 0x4F1AAF4;
const DWORD GlowIndex = 0xA310;
const DWORD GlowObject = 0x4FA08E8;
const DWORD Dormant = 0xE9;
const DWORD ActiveWeapon = 0x00002EE8;
const DWORD iWeaponID = 0x000032DC;
const DWORD FlashMaxAlpha = 0x0000A2F4;
const DWORD ForceAttack = 0x2EC5BD8;

const DWORD dwSensitivityPtr = 0xA3FC18;
const DWORD dwSensitivity = 0xA3FC44;

const DWORD teamOffset = 0xF0;
const DWORD healthOffset = 0xFC;
const DWORD EntLoopDist = 0x10;

const DWORD bhopToggleKey = 0x67;
const DWORD triggerToggleKey = 0x68;
const DWORD glowToggleKey = 0x69;

const DWORD m_szArmsModel = 0x38E3;
const DWORD m_iShotsFired = 0x0000A2B0;


//ESP VARS


const DWORD m_vecOrigin = 0x134;
const DWORD m_vecViewOffset = 0x104;

const DWORD dwViewMatrix = 0x4A75294;
const DWORD dwYawPtr = 0xAABAD8;