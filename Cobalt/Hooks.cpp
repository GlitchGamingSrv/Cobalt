#include <../Hooks.h>

#define XIP Rip

namespace Cobalt {
	uintptr_t Hook::mpTarget = 0;
	uintptr_t Hook::mpDetour = 0;

	Hook::Hook(uintptr_t pTarget, uintptr_t pDetour) {
		Hook::mpTarget = pTarget;
		Hook::mpDetour = pDetour;

		if (!IsSamePage(reinterpret_cast<const PBYTE>(pTarget), reinterpret_cast<const PBYTE>(pDetour))) {
			mhHandle = AddVectoredExceptionHandler(true, static_cast<PVECTORED_EXCEPTION_HANDLER>(Handler));

			if (mhHandle &&
				VirtualProtect(reinterpret_cast<LPVOID>(pTarget), 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwProtect))
			{
				bSuccess = true;
			}
		}
	}
	Hook::~Hook() {
		DWORD dwOldProtect;

		if (mhHandle &&
			VirtualProtect(reinterpret_cast<LPVOID>(mpTarget), 1, dwProtect, &dwOldProtect) &&
			RemoveVectoredExceptionHandler(mhHandle))
		{
			bSuccess = true;
		}
		else {
			bSuccess = false;
		}
	}

	BOOL Hook::IsSamePage(const PBYTE pFirstAddr, const PBYTE pSecondAddr)
	{
		MEMORY_BASIC_INFORMATION mbiFirst{}, mbiSecond{};

		// Query both the first and second address.
		if (!VirtualQuery(pFirstAddr, &mbiFirst, sizeof(mbiFirst))) {
			return true;
		}
		if (!VirtualQuery(pSecondAddr, &mbiSecond, sizeof(mbiSecond))) {
			return true;
		}

		if (mbiFirst.BaseAddress == mbiSecond.BaseAddress) {
			return true;
		}

		return false;
	}

	LONG WINAPI Hook::Handler(EXCEPTION_POINTERS* pException)
	{
		DWORD dwOldProtect;

		switch (pException->ExceptionRecord->ExceptionCode) {
		case STATUS_GUARD_PAGE_VIOLATION:
			if (pException->ContextRecord->XIP == mpTarget) {
				pException->ContextRecord->XIP = mpDetour;
			}

			pException->ContextRecord->EFlags |= 0x100;

			return EXCEPTION_CONTINUE_EXECUTION;

		case STATUS_SINGLE_STEP:
			VirtualProtect(reinterpret_cast<LPVOID>(mpTarget), 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwOldProtect);

			return EXCEPTION_CONTINUE_EXECUTION;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
}