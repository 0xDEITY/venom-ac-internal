#include <iostream>
#include <Windows.h>

#include "venom/venom.hpp"

DWORD WINAPI MainThread(HMODULE hModule)
{
	venom::init(hModule);

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) MainThread, hModule, 0, nullptr);

		if (hThread) CloseHandle(hThread);
	}

	return TRUE;
}