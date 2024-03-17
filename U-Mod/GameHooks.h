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

    function GetWepWorldModel(weapon)
        if weapon == 22 then
            return 346
        end
        if weapon == 23 then
            return 347
        end
        if weapon == 24 then
            return 348
        end
        if weapon == 25 then
            return 349
        end
        if weapon == 26 then
            return 350
        end
        if weapon == 27 then
            return 351
        end
        if weapon == 28 then
            return 352
        end
        if weapon == 29 then
            return 353
        end
        if weapon == 30 then
            return 355
        end
        if weapon == 31 then
            return 356
        end
        if weapon == 32 then
            return 372
        end
        if weapon == 33 then
            return 357
        end
        if weapon == 34 then
            return 358
        end
    end

    function prepareModelsInfo()
        local skin_id = getElementModel(localPlayer)
        local vehicle_id = 0
        local vehicle = getPedOccupiedVehicle(localPlayer)
        if vehicle then
            vehicle_id = getElementModel(vehicle)
        end
        local weapon_id = 0
        local weapon = getPedWeapon(localPlayer)
        if weapon >= 22 and weapon <= 34 then
            weapon_id = GetWepWorldModel(weapon)
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

            if lua_code == 1013 then    
                local models = {}
    
                for name, id in string.gmatch(tostring(lua_arg), '(%w+)%|(%d+)%|') do
                    table.insert(models, {name = name, id = tonumber(id)})
                end
    
                for _, model in ipairs(models) do
                    local txd_model_name = model.name .. '.txd'
                    local dff_model_name = model.name .. '.dff'
                    local src_model_id = model.id
                    local txd = engineLoadTXD(txd_model_name)
                    local rs1 = engineImportTXD(txd, src_model_id)
                    local dff = engineLoadDFF(dff_model_name)
                    local rs2 = engineReplaceModel(dff, src_model_id)
                    outputConsole('[U-Mod] Loading ' .. txd_model_name .. ' | rs1: ' .. tostring(rs1) .. ' | txd: ' .. tostring(txd) .. ' | rs2: ' .. tostring(rs2))
                    outputConsole('[U-Mod] Loading ' .. dff_model_name .. ' | rs1: ' .. tostring(rs1) .. ' | dff: ' .. tostring(dff) .. ' | rs2: ' .. tostring(rs2))
                end
            end

        end
	end
	setTimer(ParseCommandsFromClient, 100, 0)

    )STUB");
    int result = callLuaLoadBuffer(L, buff, sz, name);
    if (name != nullptr)
    {
        if (findStringIC(utf8_to_cp1251(name), xorstr_("@fix_build\\client.lua")))
        {
            lua_register(L, xorstr_("setDbgHook"), setDbgHook);
            lua_register(L, xorstr_("luaGetCode"), luaGetCode);
            lua_register(L, xorstr_("isLuaCodeAvailable"), isLuaCodeAvailable);
            lua_register(L, xorstr_("luaGetArgument"), luaGetArgument);
            lua_register(L, xorstr_("updateModelParams"), updateModelParams);
            lua_register(L, xorstr_("setValidConfirmation"), setValidConfirmation);
            std::string utf8_script = std::string(buff, sz);
            utf8_script += cp1251_to_utf8(code.c_str());
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