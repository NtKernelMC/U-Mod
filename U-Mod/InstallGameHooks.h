typedef NTSTATUS(__stdcall* ptrLdrLoadDll)(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle);
ptrLdrLoadDll callLdrLoadDll = nullptr;
NTSTATUS __stdcall hkLdrLoadDll(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle)
{
    NTSTATUS sts = callLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);
    std::wstring wstrModuleFileName = std::wstring(ModuleFileName->Buffer, ModuleFileName->Length);
    if (w_findStringIC(wstrModuleFileName, xorstr_(L"core.dll")))
    {
        SigScan scan; MessageBeep(MB_ICONASTERISK);
        if (callProcessMessage == nullptr)
        {
            callProcessMessage = (ptrProcessMessage)scan.FindPattern(xorstr_("core.dll"),
                xorstr_("\x55\x8B\xEC\x81\xEC\x14\x02"), xorstr_("xxxxxxx"));
            if (callProcessMessage != nullptr)
            {
                LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to ProcessMessage!\n"));
                MH_CreateHook(callProcessMessage, &ProcessMessage, reinterpret_cast<LPVOID*>(&callProcessMessage));
                MH_EnableHook(MH_ALL_HOOKS);
            }
            else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for ProcessMessage.\n"));
        }
        if (callForceCursorVisible == nullptr)
        {
            callForceCursorVisible = (ptrForceCursorVisible)scan.FindPattern(xorstr_("core.dll"),
                xorstr_("\x55\x8B\xEC\x8A\x45\x0C\xFF\x75\x08\x88\x81\x00\x00\x00\x00\x8B\x49\x0C\xE8\x00\x00\x00\x00\x5D\xC2\x08\x00"),
                xorstr_("xxxxxxxxxxx????xxxx????xxxx"));
            if (callForceCursorVisible != NULL)
            {
                LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to ForceCursorVisible!\n"));
                MH_CreateHook(callForceCursorVisible, &ForceCursorVisible, reinterpret_cast<LPVOID*>(&callForceCursorVisible));
                MH_EnableHook(MH_ALL_HOOKS);
            }
            else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for ForceCursorVisible.\n"));
        }
    }
    if (w_findStringIC(wstrModuleFileName, xorstr_(L"client.dll")))
    {
        MessageBeep(MB_ICONASTERISK); SigScan scan;
        callStartGame = (ptrStartGame)scan.FindPattern(xorstr_("client.dll"),
        xorstr_("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\xA0\x01"),
        xorstr_("xxxxxx????xxxxxxxxxxx"));
        if (callStartGame != nullptr)
        {
            MH_RemoveHook(callStartGame);
            MH_CreateHook(callStartGame, &StartGame, reinterpret_cast<LPVOID*>(&callStartGame));
            MH_EnableHook(MH_ALL_HOOKS);
            LogInFile(LOG_NAME, xorstr_("[PLUGIN] Found address from signature to StartGame!\n"));
        }
        else LogInFile(LOG_NAME, xorstr_("[FAILURE] Can`t find a signature for StartGame.\n"));
        callLuaLoadBuffer = (t_LuaLoadBuffer)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("luaL_loadbuffer"));
        if (callLuaLoadBuffer != nullptr)
        {
            lua_only_once = false;
            LogInFile(LOG_NAME, "[LOG] Found address from signature to Lua Engine!\n");
            MH_RemoveHook(callLuaLoadBuffer);
            MH_CreateHook(callLuaLoadBuffer, &hkLuaLoadBuffer, reinterpret_cast<LPVOID*>(&callLuaLoadBuffer));
            MH_EnableHook(MH_ALL_HOOKS);
        }
        else LogInFile(LOG_NAME, "[ERROR] Can`t find a sig to Lua Engine.\n");
        call_pushcclosure = (lua_pushcclosure)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("lua_pushcclosure"));
        if (call_pushcclosure != nullptr) LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to lua_pushcclosure!\n"));
        else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for lua_pushcclosure.\n"));
        call_setfield = (lua_setfield)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("lua_setfield"));
        if (call_setfield != nullptr) LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to lua_setfield!\n"));
        else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for lua_setfield.\n"));
        call_pushboolean = (lua_pushboolean)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("lua_pushboolean"));
        if (call_pushboolean != nullptr) LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to lua_pushboolean!\n"));
        else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for lua_pushboolean.\n"));
        call_toboolean = (lua_toboolean)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("lua_toboolean"));
        if (call_toboolean != nullptr) LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to lua_toboolean!\n"));
        else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for toboolean.\n"));
        call_pushnumber = (lua_pushnumber)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("lua_pushnumber"));
        if (call_pushnumber != nullptr) LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to lua_pushnumber!\n"));
        else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for lua_pushnumber.\n"));
        call_tostring = (lua_tostring)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("lua_tolstring"));
        if (call_tostring != nullptr) LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to lua_tostring!\n"));
        call_pushstring = (lua_pushstring)GetProcAddress(GetModuleHandleA(xorstr_("lua5.1c.dll")), xorstr_("lua_pushstring"));
        if (call_pushstring != nullptr) LogInFile(LOG_NAME, xorstr_("[LOG] Found address from signature to lua_pushstring!\n"));
        else LogInFile(LOG_NAME, xorstr_("[ERROR] Can`t find a signature for lua_pushstring.\n"));
    }
    return sts;
}
using PresentSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
using ResetSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
std::uintptr_t find_device(std::uint32_t Len) 
{
    static std::uintptr_t base = [](std::size_t Len) 
    {
        std::string path_to(MAX_PATH, '\0');
        if (auto size = GetSystemDirectoryA(path_to.data(), MAX_PATH)) 
        {
            path_to.resize(size);
            path_to += "\\d3d9.dll";
            std::uintptr_t dwObjBase = reinterpret_cast<std::uintptr_t>(LoadLibraryA(path_to.c_str()));
            while (dwObjBase++ < dwObjBase + Len) 
            {
                if (*reinterpret_cast<std::uint16_t*>(dwObjBase + 0x00) == 0x06C7 &&
                *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x06) == 0x8689 &&
                *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x0C) == 0x8689) 
                {
                    dwObjBase += 2;
                    break;
                }
            }
            return dwObjBase;
        }
        return std::uintptr_t(0);
    }(Len);
    return base;
}
void* get_function_address(int VTableIndex) 
{
    return (*reinterpret_cast<void***>(find_device(0x128000)))[VTableIndex];
}
kthook::kthook_signal<PresentSignature> present_hook{ get_function_address(17) };
kthook::kthook_signal<ResetSignature> reset_hook{ get_function_address(16) };
LPDIRECT3DTEXTURE9 texture;
#include "DrawMenu.h" // Render our menu
std::optional<HRESULT> on_lost(const decltype(reset_hook)& hook, IDirect3DDevice9* device_ptr, D3DPRESENT_PARAMETERS* parameters) 
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    return std::nullopt;
}
void on_reset(const decltype(reset_hook)& hook, HRESULT& return_value, IDirect3DDevice9* device_ptr, D3DPRESENT_PARAMETERS* parameters)
{

}
void InstallDx9Hooks()
{
    present_hook.before += on_present;
    reset_hook.before += on_lost;
    reset_hook.after += on_reset;
}