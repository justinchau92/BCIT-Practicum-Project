
#include "includes.h"

MemoryManager* Mem = new MemoryManager(); //Globally declare memory manager class 

DWORD MemoryManager::GetProcID() { return dwPID; }
HANDLE MemoryManager::GetProchandle() { return hProcess; }