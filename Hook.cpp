#include "Hook.h"
bool GL_Hook::Detour32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	uintptr_t relativeAddress = dst - src - 5;
	*src = 0xE9;
	*(uintptr_t*)(src + 1) = relativeAddress;

	VirtualProtect(src, len, curProtection, &curProtection);
	return true;
}
BYTE* GL_Hook::TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return 0;

	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, len, src, len);
	uintptr_t gateWayRelativeAddr = src - gateway - 5;
	*(gateway + len) = 0xE9;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gateWayRelativeAddr;
	Detour32(src, dst, len);
	return gateway;
}

GL_Hook::GL_Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
{
	this->src = src;
	this->dst = dst;
	this->len = len;
	this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
}
GL_Hook::GL_Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
{
	HMODULE hMod = GetModuleHandleA(modName);

	this->src = (BYTE*)GetProcAddress(hMod, exportName);
	this->dst = dst;
	this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
}

void GL_Hook::Enable()
{
	memcpy(originalBytes, src, len);
	*(uintptr_t*)PtrToGatewayFnPtr = (uintptr_t)TrampHook32(src, dst, len);
	bStatus = true;
}
void GL_Hook::Disable()
{
	Memory_Internal::Patch(src, originalBytes, len);
	bStatus = false;
}
void GL_Hook::Toggle()
{
	if (!bStatus) Enable();
	else Disable();
}
void GL_Hook::Status()
{
	std::cout << "[*] OpenGL Swap Buffers Hook Status: " << bStatus << std::endl;
}