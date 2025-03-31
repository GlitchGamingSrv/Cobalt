#pragma once
#include <../includes.h>

namespace Cobalt
{
	class Hook
	{
	private:
		static uintptr_t mpTarget;
		static uintptr_t mpDetour;

		PVOID mhHandle;
		DWORD dwProtect;

	public:
		BOOL bSuccess;
		Hook(uintptr_t pTarget, uintptr_t pDetour);
		~Hook();

	private:
		static LONG WINAPI Handler(EXCEPTION_POINTERS* ExceptionInfo);
		static BOOL IsSamePage(const PBYTE FirstAddr, const PBYTE SecondAddr);
	};
}