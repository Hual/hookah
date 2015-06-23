#include "hookah.h"
#include "hookah_util.h"

#ifdef _M_IX86
		#include "HDE\hde32.h"
		#define HDE_DISASM hde32_disasm
		#define HDES hde32s
#else
	#ifdef _M_X64 
		#include "HDE\hde64.h"
		#define HDE_DISASM hde64_disasm
		#define HDES hde64s
	#else
		#error Incompatible architecture
	#endif
#endif

namespace hookah
{
	hook::hook(FARPROC lpAddress, LPVOID lpCallback, bool bActivate)
	{
		/* get the address of the imported subroutine */
		BYTE* address_bytes = reinterpret_cast<BYTE*>(lpAddress);

		/* declare/initialize variables */
		HDES hs;
		trampolineSize = 0;

		/* scan until we have enough space for the jmp patch */
		do
		{
			trampolineSize += HDE_DISASM(address_bytes + trampolineSize, &hs);
		} while (trampolineSize < JMP_PATCH_SIZE);

		/* deprotect the original code */
		unprotectMemory(lpAddress, trampolineSize);

		/* allocate memory on the heap for original function redirection */
		originalFunction = new BYTE[trampolineSize + JMP_PATCH_SIZE];
		originalFunctionJumpback = address_bytes + trampolineSize;
		BYTE* origFuncAddress = reinterpret_cast<BYTE*>(originalFunction);

		/* copy instructions that we're going to replace */
		memcpy(originalFunction, lpAddress, trampolineSize);

		/* unprotect original function proxy memory */
		unprotectMemory(originalFunction, trampolineSize + JMP_PATCH_SIZE);

		/* set patchedFunction to lpAddress */
		patchedFunction = lpAddress;

		/* set callbackFunction to lpCallback */
		callbackFunction = lpCallback;

		/* apply jump patch to original */
		applyJumpPatch(origFuncAddress + trampolineSize, originalFunctionJumpback);

		/* check for initial activation */
		if (bActivate)
			activate();
		else
			deactivate();

	}

	LPVOID hook::activate()
	{
		/* apply jump patch from the patched function to the hook */
		applyJumpPatch(patchedFunction, callbackFunction);
		m_active = true;

		return originalFunction;
	}

	void hook::deactivate()
	{
		/* apply jump patch from the patched function to the original function trampoline*/
		applyJumpPatch(patchedFunction, originalFunction);
		m_active = false;
	}

	void hook::uninstall()
	{
		/* copy the instructions from the trampoline back to the patched function */
		memcpy(patchedFunction, originalFunction, trampolineSize);

		/* free the trampoline memory on the heap */
		delete[] originalFunction;
		m_active = false;

		/* free object memory */
		delete this;
	}
}
