#pragma once
#include "includes.h"

#include <Windows.h>
#include <TlHelp32.h>

class Scanner {
private:

public:
	Scanner();
	~Scanner();
	//Internal Pattern Scan
	void * PatternScan(char* base, size_t size, const char* pattern, const char*mask);

	//External Wrapper
	void * PatternScanEx(HANDLE hPRocess, uintptr_t begin, uintptr_t end, const  char* pattern, const  char*  mask);

	//Module wrapper for external pattern scan
	void * PatternScanExModule(HANDLE hProcess, const  wchar_t* module, const  char* pattern, const  char* mask);

};

extern Scanner * nScanner;

