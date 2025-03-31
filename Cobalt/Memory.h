#pragma once
#include <../includes.h>
namespace Cobalt
{
	class Memory
	{
	public:
		Memory();
		~Memory();
		static DWORD FortniteUtilConsole()
		{
			AllocConsole();
			FILE* fortFile;
			freopen_s(&fortFile, "COUNOUT$", "w", stdout);
			return 0;
		}

		static BOOL memory(PVOID mem, LPCSTR pattern, LPCSTR mask)
		{
			for (auto ini = static_cast<PBYTE>(mem); *mask; ++pattern, ++mask, ++ini)
			{
				if (*mask == 'x' && *reinterpret_cast<LPCBYTE>(pattern) != *ini)
					return false;
			}
			return true;
		}

		static PBYTE memorypattern(PVOID base, DWORD dw, LPCSTR pattern, LPCSTR mask)
		{
			dw -= static_cast<DWORD>(strlen(mask));
			for (auto i = 0UL; i < dw; ++i)
			{
				auto add = static_cast<PBYTE>(base) + i;
				if (memory(add, pattern, mask))
				{
					return add;
				}
			}
			return NULL;
		}

		static uintptr_t FortAddress()
		{
			return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
		}

		static PBYTE fortmememory(LPCSTR pattern, LPCSTR mask)
		{
			MODULEINFO info{};
			GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

			PBYTE add = NULL;
			do
			{
				add = memorypattern(info.lpBaseOfDll, info.SizeOfImage, pattern, mask);
				Sleep(50);
			} while (!add);

			return add;
		}
	};
}

namespace FortniteOffsets
{
	class FortOGOffsets
	{
	public:
		FortOGOffsets();
		~FortOGOffsets();
		uintptr_t FortOGWorldOffset = 0x4CAAFC0;
		uintptr_t FortOGInventoryOffset = 0x4ACC00;
		uintptr_t FortOGPawnOffset = 0x5BCF000;
		uintptr_t FortOGAbilityOffset = 0x500FCBA;
		uintptr_t FortOGGraphicsNVIDIAOffset = 0x77ACFFF;
		uintptr_t FortOGGraphicsNVIDIARenderingOffset = 0x77FA000;
		uintptr_t FortOGGraphicsNVIDIAHighRenderingOffset = 0x800CFAA;
		uintptr_t FortOGGraphicsNVIDIALowRenderingOffset = 0x66AF000;
		uintptr_t FortOGGraphicsAMDOffset = 0x74CFA00;
		uintptr_t FortOGGraphicsAMDRenderingOffset = 0x75CAAFF;
		uintptr_t FortOGGraphicsAMDHighRenderingOffset = 0x77C00FF;
		uintptr_t FortOGGraphicsAMDLowRenderingOffset = 0x7100CFF;
		uintptr_t FortOGGraphicsNVIDIAMathRendering = 0x88F0CCA;
		uintptr_t FortOGGraphicsAMDMathRendering = 0x8440CFF;
	};

	class FortC2Offsets
	{
	public:
		FortC2Offsets();
		~FortC2Offsets();
		uintptr_t FortC2WorldOffset = 0x7CAAFC0;
		uintptr_t FortC2InventoryOffset = 0x8ACC00;
		uintptr_t FortC2PawnOffset = 0xF6CF000;
		uintptr_t FortC2AbilityOffset = 0x6FCFCBA;
		uintptr_t FortC2GraphicsNVIDIAOffset = 0x8FC009A;
		uintptr_t FortC2GraphicsNVIDIARenderingOffset = 0x8CCFB00;
		uintptr_t FortC2GraphicsNVIDIAHighRenderingOffset = 0x800CFAA;
		uintptr_t FortC2GraphicsNVIDIALowRenderingOffset = 0x680AADC;
		uintptr_t FortC2GraphicsAMDOffset = 0x82CFA00;
		uintptr_t FortC2GraphicsAMDRenderingOffset = 0x800DCF;
		uintptr_t FortC2GraphicsAMDHighRenderingOffset = 0x811FCCA;
		uintptr_t FortC2GraphicsAMDLowRenderingOffset = 0xAAF00CD;
		uintptr_t FortC2GraphicsNVIDIAMathRendering = 0xA0DACCF;
		uintptr_t FortC2GraphicsAMDMathRendering = 0x88CABC2;
	};

	class FortC3Offsets
	{
	public:
		FortC3Offsets();
		~FortC3Offsets();
		uintptr_t FortC3WorldOffset = 0x8FAFF00;
		//Getting More Fortnite Chapter 3 Offsets Soon...
	};
}