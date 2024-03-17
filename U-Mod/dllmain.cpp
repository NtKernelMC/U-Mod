#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif
#pragma warning(disable : 4477)
#pragma warning(disable : 4244)
#pragma warning (disable : 4305)
#pragma warning (disable : 4018)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <Windows.h>
#include <stdio.h>
#include <cmath>
#include <thread>
#include <direct.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <tuple>
#include <mutex>
#include "Registry.h"
#include "xorstr.h"
#include "PebHider.h"
#include "MinHook.h"
#include <winternl.h>
#include "sigscan.h"
#include "resource.h"
#include "kthook/kthook.hpp" //github.com/KiN4StAt/kthook
#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9.lib")
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "PluginUtils.h"
#pragma comment(lib, "libMinHook.x86.lib")
#pragma comment(lib, "Winmm.lib")
#include "GameHooks.h"
#include "InstallGameHooks.h"
extern "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(NULL, code, wParam, lParam);
}
void HackThread()
{
    MH_Initialize();
    LoadCustomModels();
    LogInFile(LOG_NAME, xorstr_("[LOG] U-Mod %s by DroidZero loaded!\n"), PLUGIN_VER);
    InstallDx9Hooks();
    callLdrLoadDll = (ptrLdrLoadDll)GetProcAddress(GetModuleHandleA(xorstr_("ntdll.dll")), xorstr_("LdrLoadDll"));
    if (callLdrLoadDll != nullptr)
    {
        MH_CreateHook(callLdrLoadDll, &hkLdrLoadDll, reinterpret_cast<LPVOID*>(&callLdrLoadDll));
        MH_EnableHook(MH_ALL_HOOKS);
    }
    while (true)
    {
        if (GetAsyncKeyState(VK_F10) && GetModuleHandleA(xorstr_("client.dll")))
        {
            auto ToggleControls = []()
            {
                HackInjectAvailable = true;
                char cmd_str[256]; memset(cmd_str, 0, sizeof(cmd_str));
                sprintf(cmd_str, xorstr_("%d"), 1011);
                gLuaCode = cmd_str;
                gLuaArg = xorstr_("NONE");
            };
            ShowMenu ^= true;
            ToggleControls();
            if (ShowMenu) callForceCursorVisible(CoreECX, true, true);
            else callForceCursorVisible(CoreECX, false, false);
            Sleep(250);
        }
        Sleep(150);
    }
}
class UMod
{
public:
    UMod()
    {
        DeleteFileA(xorstr_("texture.jpg")); DeleteFileA(xorstr_("Shrift.otf"));
        DropResource(xorstr_("texture.jpg"), MAKEINTRESOURCEA(IDR_RCDATA1), RT_RCDATA);
        DropResource(xorstr_("Shrift.otf"), MAKEINTRESOURCEA(IDR_RCDATA2), RT_RCDATA);
        PebHider::RemovePeHeader(GetModuleHandleA(U_DLL_NAME));
        PebHider::UnlinkModule(U_DLL_NAME);
        std::thread th(HackThread);
        th.detach();
    }
} umod;
