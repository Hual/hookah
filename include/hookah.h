#pragma once

#include <Windows.h>

namespace hookah
{
	class hook
	{
	private:
		hook(FARPROC lpAddress, LPVOID lpCallback, bool bActivate);

		bool m_active;

	public:
		/// <summary>Hooks a function specified by its name to a callback specified by its address.</summary>
		/// <param name="szfName">The name of the subroutine to be hooked.</param>
		/// <param name="lpCallback">The address of the subroutine that will be called by the hook.</param>
		/// <param name="szModule">The module in which to search for the subroutine. <remarks>The current module will be searched if it's null.</remarks></param>
		/// <param name="bActivate">A boolean that states whether the hook should be activated upon installation</param>
		/// <returns>The hook that was installed</returns>
		static inline hook& install(LPSTR szfName, LPVOID lpCallback, LPSTR szModule = nullptr, bool bActivate = false)
		{
			return *new hook(GetProcAddress(GetModuleHandle(szModule), szfName), lpCallback, bActivate);
		}

		/// <summary>Hooks a function specified by its address to a callback specified by its address.</summary>
		/// <param name="lpAddress">The address of the subroutine to be hooked.</param>
		/// <param name="lpCallback">The address of the subroutine that will be called by the hook.</param>
		/// <param name="bActivate">A boolean that states whether the hook should be activated upon installation</param>
		/// <returns>The hook that was installed</returns>
		static inline hook& install(FARPROC lpAddress, LPVOID lpCallback, bool bActivate = false)
		{
			return *new hook(lpAddress, lpCallback, bActivate);
		}

		/// <summary>Activates the installed hook, allowing flow through the proxy.</summary>
		/// <returns>The address of the original subroutine, to be called without redirecting to the proxy.</returns>
		LPVOID activate();

		/// <summary>Deactivates the installed hook.</summary>
		void deactivate();

		/// <summary>Uninstalls the hook, frees all resources and references, including the hook object.</summary>
		void uninstall();

		/// <summary>Returns information about the hook's state.</summary>
		/// <returns>True if the hook is active, false if it's inactive.</returns>
		inline bool active() { return m_active; }

		/// <summary>The address of the original function trampoline.</summary>
		LPVOID originalFunction;

		/// <summary>The address of the original function jumpback instruction.</summary>
		LPVOID originalFunctionJumpback;

		/// <summary>The address of the patched function beginning.</summary>
		LPVOID patchedFunction;

		/// <summary>The address of the callback function specified by the user.</summary>
		LPVOID callbackFunction;

		/// <summary>The size of the trampoline.</summary>
		size_t trampolineSize;
	};
}