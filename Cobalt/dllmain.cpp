// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <../Memory.h>
#include <../GUI.h>
static FortniteOffsets::FortOGOffsets* fortoffsets;
static FortniteOffsets::FortC2Offsets* fortoffsets2;
static FortniteOffsets::FortC3Offsets* fortOffsets3;
static Cobalt::Memory* CobaltMemoryData;
static Cobalt::GUI* appgui;
DWORD WINAPI Main(LPVOID lpParam)
{
    fortoffsets = new FortniteOffsets::FortOGOffsets;
    fortoffsets2 = new FortniteOffsets::FortC2Offsets;
    fortOffsets3 = new FortniteOffsets::FortC3Offsets;
    CobaltMemoryData = new Cobalt::Memory;
    appgui = new Cobalt::GUI;
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        printf("Starting Cobalt...\n");
        CreateThread(0, 0, Main, hModule, 0, 0);
        appgui->UserLoginPage;
        appgui->DownloadFortnitePage;
        appgui->FortniteGameLibrary;
        appgui->UpdatePage;
        appgui->WelcomeWindow;
        appgui->LanguagePage;
        appgui->LaunchFortnitePage;
        appgui->SettingsPage;
        CobaltMemoryData->FortniteUtilConsole;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

