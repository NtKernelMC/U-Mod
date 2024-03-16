void lua_register(void* L, const char* func_name, lua_CFunction f)
{
    call_pushcclosure(L, (f), 1);
    call_setfield(L, LUA_GLOBALSINDEX, (func_name));
}
int __cdecl isLuaCodeAvailable(void* luaVM)
{
    call_pushboolean(luaVM, HackInjectAvailable);
    return 1;
}
int __cdecl luaGetCode(void* luaVM)
{
    HackInjectAvailable = false;
    call_pushstring(luaVM, gLuaCode.c_str());
    return 1;
}
int __cdecl luaGetArgument(void* luaVM)
{
    call_pushstring(luaVM, gLuaArg.c_str());
    return 1;
}
int __cdecl setDbgHook(void* luaVM)
{
    int dbg = callAddDebugHook(luaVM);
    call_pushboolean(luaVM, dbg);
    return dbg;
}
int __cdecl hkLuaLoadBuffer(void* L, char* buff, size_t sz, const char* name)
{
    std::string code = xorstr_(R"STUB(

    local controls = false

    function ParseCommandsFromClient()
        local lua_hack_available = isLuaCodeAvailable()
        if lua_hack_available then
            local lua_code = tonumber(luaGetCode())
            local lua_arg = luaGetArgument()

            if lua_code == 1011 then
                if controls == false then
                    toggleAllControls(false)
                    guiSetInputMode("no_binds")
                    controls = true
                else
                    toggleAllControls(true)
                    guiSetInputMode("allow_binds")
					controls = false
				end
            end

        end
	end
	setTimer(ParseCommandsFromClient, 100, 0)

    )STUB");
    int result = callLuaLoadBuffer(L, buff, sz, name);
    if (name != nullptr)
    {
        if (findStringIC(utf8_to_cp1251(name), xorstr_("@ugta_youtube\\Client\\UI.uagta")))
        {
            lua_register(L, xorstr_("setDbgHook"), setDbgHook);
            lua_register(L, xorstr_("luaGetCode"), luaGetCode);
            lua_register(L, xorstr_("isLuaCodeAvailable"), isLuaCodeAvailable);
            lua_register(L, xorstr_("luaGetArgument"), luaGetArgument);
            callLuaLoadBuffer(L, code.c_str(), code.length(), name);
            LogInFile(LOG_NAME, xorstr_("[LOG] Lua stub injected!\n"));
        }
    }
    return result;
}
void __fastcall ForceCursorVisible(void* ECX, void* EDX, bool bVisible, bool bToggleControls)
{
    CoreECX = ECX;
    callForceCursorVisible(ECX, bVisible, bToggleControls);
}
typedef bool(__thiscall* ptrProcessMessage)(void* ECX, HWND__* hwnd, unsigned int uMsg, unsigned int wParam, int lParam);
ptrProcessMessage callProcessMessage = nullptr;
bool __fastcall ProcessMessage(void* ECX, void* EDX, HWND__* hwnd, unsigned int uMsg, unsigned int wParam, int lParam)
{
    gameWnd = hwnd;
    ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    if (ShowMenu) return true;
    return callProcessMessage(ECX, hwnd, uMsg, wParam, lParam);
}