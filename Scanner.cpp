#include "includes.h"

Scanner * nScanner = new Scanner();
Scanner::Scanner()
{
}

Scanner::~Scanner()
{
}

//Internal Pattern Scan
void * Scanner::PatternScan(char* base, size_t size, const  char* pattern, const  char* mask)
{
	unsigned int patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}

		}
		if (found)
		{
			return (void*)(base + i);
		}
	}
	return nullptr;
}

//External Wrapper
void * Scanner::PatternScanEx(HANDLE hProcess, uintptr_t begin, uintptr_t end, const  char* pattern, const  char*  mask)
{
	uintptr_t currentChunk = begin;
	SIZE_T bytesRead;

	char mainBuffer[8192]{};

	while (currentChunk < end)
	{
		char buffer[4096];

		DWORD oldprotect;
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), PAGE_EXECUTE_READWRITE, &oldprotect);
		ReadProcessMemory(hProcess, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);

		memcpy(mainBuffer + sizeof(buffer), buffer, sizeof(buffer));
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), oldprotect, &oldprotect);

		if (bytesRead == 0)
		{
			return nullptr;
		}

		void* internalAddress = PatternScan((char*)&mainBuffer, sizeof(mainBuffer), pattern, mask);

		memcpy(mainBuffer, buffer, sizeof(buffer));
		if (internalAddress != nullptr)
		{
			//calculate from internal to external
			uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)&mainBuffer;
			return (void*)(currentChunk - sizeof(buffer) + offsetFromBuffer);
		}
		else
		{
			//advance to next chunk
			currentChunk = currentChunk + bytesRead;
		}
	}
	Beep(500, 100);
	return nullptr;
}

//Module wrapper for external pattern scan
void * Scanner::PatternScanExModule(HANDLE hProcess, const wchar_t* module, const char* pattern, const char* mask)
{

	MODULEENTRY32W modEntry;

	if (module == L"client.dll")
	{
		modEntry = Mem->ClientDLL;

	}
	else if (module == L"engine.dll")
	{
		modEntry = Mem->EngineDLL;
	}

	uintptr_t begin = (uintptr_t)modEntry.modBaseAddr;
	uintptr_t end = begin + modEntry.modBaseSize;

	cout << "Scan : " << begin << " to " << end << endl;
	return PatternScanEx(hProcess, begin, end, pattern, mask);
}


