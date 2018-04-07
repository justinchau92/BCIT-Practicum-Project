#pragma once
#include <Windows.h>
#include <Tlhelp32.h>
#include <string>
using namespace std;

class MemoryManager
{
private:
	DWORD dwPID;			//The processID of CSGO
	HANDLE hProcess;		//The handle of CSGO

public:
	MODULEENTRY32 ClientDLL;
	MODULEENTRY32 EngineDLL;
	DWORD ClientDLL_Base, ClientDLL_Size;
	DWORD EngineDLL_Base, EngineDLL_Size;

	//Attaching the ProcessName
	//Return TRUE on success and FALSE on failure
	bool AttachProcess(const char* ProcessName)
	{
		//creates a snapshot of the specified processes including modules
		HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		//check if the snapshot is valid
		if (hPID == INVALID_HANDLE_VALUE) return false;

		//create helper struct that will contain info about process
		PROCESSENTRY32 procEntry;
		//set the dwsize of procEntry
		procEntry.dwSize = sizeof(procEntry);

		//grab the processname and compare it to the list of processes after
		const WCHAR *procNameChar;
		int nChars = MultiByteToWideChar(CP_ACP, 0, ProcessName, -1, NULL, 0);
		procNameChar = new WCHAR[nChars];
		MultiByteToWideChar(CP_ACP, 0, ProcessName, -1, (LPWSTR)procNameChar, nChars);

		do
			//compare the two string
			if (!wcscmp(procEntry.szExeFile, procNameChar))
			{
				this->dwPID = procEntry.th32ProcessID;
				CloseHandle(hPID);
				this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->dwPID);
				return true;
			}
		while (Process32Next(hPID, &procEntry));

		CloseHandle(hPID);
		return false;

	}// End of AttachProcess

	 //Grab a module and 
	MODULEENTRY32 GetModule(const char * ModuleName)
	{
		//creates a snapshot of the specified processes including modules
		HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);

		const WCHAR *modNameChar;
		int nChars = MultiByteToWideChar(CP_ACP, 0, ModuleName, -1, NULL, 0);
		modNameChar = new WCHAR[nChars];
		MultiByteToWideChar(CP_ACP, 0, ModuleName, -1, (LPWSTR)modNameChar, nChars);

		do
			if (!wcscmp(mEntry.szModule, modNameChar))
			{
				CloseHandle(hModule);
				return mEntry;
			}
		while (Module32Next(hModule, &mEntry));

		CloseHandle(hModule);
		mEntry.modBaseAddr = 0x0;
		return mEntry;
	}

	//RPM AND WPM wrappers

	//Reads a value from memory and put in value
	//Returns true on success and false on failure
	template<class c>
	c Read(DWORD dwAddress)
	{
		c val;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &val, sizeof(c), NULL);
		return val;
	}

	//Write a value in memory
	// Returns true on success and false on failure
	template<class c>
	c Write(DWORD dwAddress, c ValueToWrite)
	{
		return WriteProcessMemory(hProcess, (LPVOID)dwAddress, &ValueToWrite, sizeof(c), NULL);
	}

	//Getters for dwPID and hProcess
	DWORD GetProcID();
	HANDLE GetProchandle();
	//Default Constructor 
	MemoryManager()
	{
		this->hProcess = NULL;
		this->dwPID = NULL;

		try {
			if (!AttachProcess("csgo.exe")) throw 1;
			this->ClientDLL = GetModule("client.dll");
			this->EngineDLL = GetModule("engine.dll");
			this->ClientDLL_Base = (DWORD)this->ClientDLL.modBaseAddr;
			this->EngineDLL_Base = (DWORD)this->EngineDLL.modBaseAddr;

			if (this->ClientDLL_Base == 0x0) throw 2;
			if (this->EngineDLL_Base == 0x0) throw 3;

			this->ClientDLL_Size = this->ClientDLL.dwSize;
			this->EngineDLL_Size = this->EngineDLL.dwSize;

		}
		catch (int numErr) {
			switch (numErr)
			{
			case 1: MessageBoxA(NULL, "CS: GO is not running", "ERROR", MB_ICONSTOP | MB_OK); exit(0); break;
			case 2: MessageBoxA(NULL, "Client.dll not found", "ERROR", MB_ICONSTOP | MB_OK); exit(0); break;
			case 3: MessageBoxA(NULL, "Engine.dll is not running", "ERROR", MB_ICONSTOP | MB_OK); exit(0); break;
			}
		}
		catch (...) {
			MessageBoxA(NULL, "Unknown exeception thrown", "ERROR", MB_ICONSTOP | MB_OK);
			exit(0);
		}


	}

	//Deconstructor
	~MemoryManager()
	{
		CloseHandle(this->hProcess);
	}


};

extern MemoryManager* Mem;