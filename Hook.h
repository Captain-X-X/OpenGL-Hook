#pragma once
#include <Windows.h>
#include "Memory.h"


struct GL_Hook
{
	BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len);
	bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len);

	bool bStatus{ false };

	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };
	BYTE* PtrToGatewayFnPtr{ nullptr };
	uintptr_t len{ 0 };
	BYTE originalBytes[10]{ 0 };

	GL_Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len);
	GL_Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len);

	void Enable();
	void Disable();
	void Toggle();
	void Status();
};