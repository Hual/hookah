#include "hookah_util.h"

namespace hookah
{
	void applyJumpPatch(LPVOID lpAddress, LPVOID lpDestination)
	{
		BYTE* pAddress = reinterpret_cast<BYTE*>(lpAddress);

#ifdef _M_IX86

		/* `JMP lpDestination` */
		pAddress[0x00] = 0xE9;

		/* set the operand to the relative position to jump to, since `JMP <immediate>` is relative */
		*(DWORD*)&pAddress[0x01] = (reinterpret_cast<DWORD>(lpDestination) - reinterpret_cast<DWORD>(lpAddress) - JMP_PATCH_SIZE);

#else
	#ifdef _M_X64

		/* `jmp [RIP]` */
		pAddress[0x00] = 0xFF;
		pAddress[0x01] = 0x25;
		pAddress[0x02] = pAddress[0x03] = pAddress[0x04] = pAddress[0x05] = 0x00;

		/* set the address to jump to; `JMP [RIP]` reads this because RIP is at the end of the previous instruction */
		*reinterpret_cast<LPVOID*>(&pAddress[0x06]) = lpDestination;

	#endif
#endif
	}

	BOOL unprotectMemory(LPVOID lpAddress, size_t unLength, DWORD* pOld)
	{
		DWORD dwOldTmp;

		BOOL result = VirtualProtect(lpAddress, unLength, PAGE_EXECUTE_READWRITE, &dwOldTmp);

		if (pOld)
			*pOld = dwOldTmp;

		return result;
	}
}
