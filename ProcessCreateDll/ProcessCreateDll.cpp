// ProcessCreateDll.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>

#pragma data_seg (".createHook")
HWND	g_hWnd = 0;
HHOOK	g_hHook = 0;
#pragma data_seg ()

#pragma comment(linker,"/SECTION:.createHook,RWS")

HINSTANCE hDll;

// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	hDll = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	if (nCode < 0)
	{
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
}

extern "C" __declspec(dllexport) BOOL initHook(unsigned long threadId) {
	g_hHook = SetWindowsHookEx(WH_CBT, GetMsgProc, hDll, threadId);
	FILE *file;
	fopen_s(&file, "E:/Users/Markus/Desktop/createProcess.txt", "a+");
	if (g_hHook == NULL)
	{
		fprintf(file, "Hooking failed\n");
		fclose(file);
		return false;
	}
	fprintf(file, "Success\n");
	fclose(file);
	return true;
}

extern "C" __declspec(dllexport) BOOL unhook() {
	return UnhookWindowsHookEx(g_hHook);
}