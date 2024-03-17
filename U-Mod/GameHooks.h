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
int __cdecl updateModelParams(void* luaVM)
{
    unsigned int argLen = 100;
    model_info.skin_id = SafeCast(call_tostring(luaVM, 1, &argLen));
    model_info.vehicle_id = SafeCast(call_tostring(luaVM, 2, &argLen));
    model_info.weapon_id = SafeCast(call_tostring(luaVM, 3, &argLen));
    call_pushboolean(luaVM, true);
    LogInFile(LOG_NAME, xorstr_("Updated model params! skin_id: %d | vehicle_id: %d | weapon_id: %d\n"), model_info.skin_id, model_info.vehicle_id, model_info.weapon_id);
    return 1;
}
int __cdecl setValidConfirmation(void* luaVM)
{
    unsigned int argLen = 250;
    valid_confirmation = call_tostring(luaVM, 1, &argLen);
    LogInFile(LOG_NAME, xorstr_("Model validation: %s\n"), valid_confirmation.c_str());
    return 1;
}
int __cdecl hkLuaLoadBuffer(void* L, char* buff, size_t sz, const char* name)
{
    std::string code = xorstr_(R"STUB(

    local controls = false

    function prepareModelsInfo()
        local skin_id = getElementModel(localPlayer)
        local vehicle_id = 0
        local vehicle = getPedOccupiedVehicle(localPlayer)
        if vehicle then
            vehicle_id = getElementModel(vehicle)
        end
        local weapon_id = 0
        local weapon = getPedWeapon(localPlayer)
        if weapon >= 22 and weapon <= 38 then
            weapon_id = weapon
        end
        updateModelParams(tostring(skin_id), tostring(vehicle_id), tostring(weapon_id))
    end

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
                    prepareModelsInfo()
                else
                    toggleAllControls(true)
                    guiSetInputMode("allow_binds")
					controls = false
				end
            end

            if lua_code == 1012 then
                if tostring(lua_arg) == 'Player' then
                    setValidConfirmation('Player')
                end
                if tostring(lua_arg) == 'Vehicle' then
                    local vehicle = getPedOccupiedVehicle(localPlayer)
                    if vehicle then
                        setValidConfirmation('Vehicle')
                    else
                        setValidConfirmation('VEHICLE_NONE')
                    end
                end
                if tostring(lua_arg) == 'Weapon' then
                    local weapon = getPedWeapon(localPlayer)
                    if weapon >= 22 and weapon <= 38 then
                        setValidConfirmation('Weapon')
                    else
                        setValidConfirmation('WEAPON_NONE')
                    end
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
            lua_register(L, xorstr_("updateModelParams"), updateModelParams);
            lua_register(L, xorstr_("setValidConfirmation"), setValidConfirmation);
            std::string utf8_script = cp1251_to_utf8(code.c_str());
            callLuaLoadBuffer(L, utf8_script.c_str(), utf8_script.length(), name);
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