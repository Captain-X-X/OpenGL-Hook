#include <windows.h>
#include <iostream>
#include "Hook.h"
typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
twglSwapBuffers owglSwapBuffers;
twglSwapBuffers wglSwapBuffersGateway;

FILE* fp;

BOOL __stdcall hkglSwapBuffers(HDC hDc)
{
	std::cout << "[*] OpenGL Hooked!" << std::endl;

    return wglSwapBuffersGateway(hDc);
}

BOOL WINAPI HookStart(HMODULE HookModule)
{
    AllocConsole();
    freopen_s(&fp, "CONOUT$", "w", stdout);
    GL_Hook SwapBuffersHook((BYTE*)GetProcAddress(GetModuleHandle("opengl32.dll"), "wglSwapBuffers"), (BYTE*)hkglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);
    SwapBuffersHook.Enable();
    SwapBuffersHook.Status();
    while(!GetAsyncKeyState(VK_END) & 1)
    {
	Sleep(10);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HMOD = hModule;
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HookStart, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
