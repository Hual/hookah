#include <stdio.h>
#include <hookah.h>

typedef int (WINAPI *GetWindowTextA_t)(HWND hWnd, LPTSTR lpString, int nMaxCount);

GetWindowTextA_t GetWindowTextA_Orig = NULL;

int WINAPI GetWindowTextA_Hook(HWND hWnd, LPTSTR lpString, int nMaxCount)
{
#ifdef _M_IX86

	printf(
		"GetWindowTextA arguments:\r\n\t"
		"hWnd=0x%08X\r\n\t"
		"lpString=0x%08X\r\n\t"
		"nMaxCount=%u\r\n",
	hWnd, lpString, nMaxCount);

#else
#ifdef _M_X64

	printf(
		"GetWindowTextA arguments:\r\n\t"
		"hWnd=0x%I64X\r\n\t"
		"lpString=0x%I64X\r\n\t"
		"nMaxCount=%u\r\n",
		hWnd, lpString, nMaxCount);

#endif
#endif

	return GetWindowTextA_Orig(hWnd, lpString, nMaxCount);
}

int main()
{
	hookah::hook& hook = hookah::hook::install("GetWindowTextA", &GetWindowTextA_Hook, "User32.dll");
	GetWindowTextA_Orig = reinterpret_cast<GetWindowTextA_t>(hook.activate());

	char title[MAX_PATH];
	GetWindowTextA(GetConsoleWindow(), title, MAX_PATH);

	hook.uninstall();

	printf(
		"\r\n"
		"Window Title: %s"
		"\r\n",
	title);

	return 0;
}