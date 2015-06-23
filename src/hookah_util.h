#pragma once

#include <Windows.h>

#ifdef _M_IX86
		#define JMP_PATCH_SIZE 0x05
#else
	#ifdef _M_X64
		#define JMP_PATCH_SIZE 0x0E
	#else
		#error Incompatible architecture
	#endif
#endif

namespace hookah
{
	void applyJumpPatch(LPVOID lpAddress, LPVOID lpDestination);
	BOOL unprotectMemory(LPVOID lpAddress, size_t unLength, DWORD* pOld = nullptr);
}
