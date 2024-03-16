#pragma once
#pragma warning(disable : 4731)
#define U_DLL_NAME xorstr_("U-Mod.dll")
#define LOG_NAME xorstr_("U-Mod.log")
#define PLUGIN_VER xorstr_("V1.0 BETA")
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <direct.h>
#include <algorithm>
#include <optional>
#include <ctime>
#include <cwctype>
#include <map>
#include <sstream>
#include <codecvt>
#include "xorstr.h"
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Winmm.lib")
void* CoreECX = nullptr;
HWND gameWnd = nullptr;
MODULEINFO lpModule;
typedef int(__cdecl* t_LuaLoadBuffer)(void* L, const char* buff, size_t sz, const char* name);
t_LuaLoadBuffer callLuaLoadBuffer = nullptr;
typedef int(__cdecl* ptrAddDebugHook)(void* luaVM);
ptrAddDebugHook callAddDebugHook = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef void(__thiscall* ptrForceCursorVisible)(void* ECX, bool bVisible, bool bToggleControls);
ptrForceCursorVisible callForceCursorVisible = nullptr;
std::string total_hwid = "";
bool ShowMenu = false;
bool lua_only_once = false;
bool guiData = false;
using lua_CFunction = int(__cdecl*)(void*);
#define LUA_GLOBALSINDEX (-10002)
typedef void(__cdecl* lua_pushcclosure)(void* L, lua_CFunction fn, int n);
lua_pushcclosure call_pushcclosure = nullptr;
typedef void(__cdecl* lua_setfield)(void* L, int idx, const char* k);
lua_setfield call_setfield = nullptr;
typedef void(__cdecl* lua_pushboolean)(void* L, int bValue);
lua_pushboolean call_pushboolean = nullptr;
typedef int(__cdecl* lua_toboolean)(void* L, int idx);
lua_toboolean call_toboolean = nullptr;
typedef int(__cdecl* lua_strlen)(void* L);
lua_strlen call_strlen = nullptr;
typedef const char* (__cdecl* lua_tostring)(void* L, int idx, unsigned int* len);
lua_tostring call_tostring = nullptr;
typedef void(__cdecl* lua_pushnumber)(void* L, long double n);
lua_pushnumber call_pushnumber = nullptr;
typedef void(__cdecl* lua_pushstring)(void* L, const char* s);
lua_pushstring call_pushstring = nullptr;
char LuaCode[50000];
bool InjectAvailable = false;
bool HackInjectAvailable = false;
std::string gLuaCode = "";
std::string gLuaArg = "";
char inp_txt[256];
std::map<std::string, DWORD> CModelsList;
std::string status_message = "STATUS: ---";
char* strdel(char* s, int pos, int n)
{
	char* dst = s + pos, * src = s + pos + n;
	while (*src)
	{
		*dst++ = *src++;
	}
	*dst = 0;
	return s;
}
bool IsFileExist(std::string file_name)
{
	FILE* hFile = fopen(file_name.c_str(), "rb");
	if (hFile != nullptr)
	{
		fclose(hFile);
		return true;
	}
	return false;
}
void removeFirstChar(char* str) {
	if (str == nullptr || strlen(str) == 0) {
		return; // Проверяем, не пустая ли строка или не nullptr
	}

	size_t len = strlen(str);
	for (size_t i = 1; i < len; i++) {
		str[i - 1] = str[i]; // Сдвигаем каждый символ на одну позицию влево
	}
	str[len - 1] = '\0'; // Устанавливаем нулевой символ в конец строки
}
std::string GetGamePath()
{
	std::string game_path = "";
	CEasyRegistry* reg = new CEasyRegistry(HKEY_CURRENT_USER, xorstr_("SOFTWARE\\UKRAINEGTA: GLAB3\\Common"), false);
	if (reg != nullptr)
	{
		game_path = reg->ReadString(xorstr_("GTA:SA Path"));
		game_path += xorstr_("\\mods\\deathmatch\\resources\\ugta_youtube\\Client");
		delete reg;
	}
	return game_path;
}
bool SetDebugPrivileges() 
{
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) return false;

	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValueA(NULL, SE_DEBUG_NAME, &luid)) 
	{
		CloseHandle(hToken);
		return false;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) 
	{
		CloseHandle(hToken);
		return false;
	}

	CloseHandle(hToken);
	return true;
}

void __stdcall LogInFile(std::string log_name, const char* log, ...)
{
	char hack_dir[600]; memset(hack_dir, 0, sizeof(hack_dir));
	_getcwd(hack_dir, sizeof(hack_dir)); char new_dir[600];
	memset(new_dir, 0, sizeof(new_dir));
	sprintf(new_dir, xorstr_("%s\\%s"), hack_dir, log_name.c_str());
	static bool once = false; 
	if (!once)
	{
		DeleteFileA(new_dir);
		once = true;
	}
	FILE* hFile = fopen(new_dir, xorstr_("a+"));
	if (hFile)
	{
		time_t t = std::time(0); tm* now = std::localtime(&t);
		char tmp_stamp[600]; memset(tmp_stamp, 0, sizeof(tmp_stamp));
		sprintf(tmp_stamp, xorstr_("[%d:%d:%d]"), now->tm_hour, now->tm_min, now->tm_sec);
		strcat(tmp_stamp, std::string(" " + std::string(log)).c_str());
		va_list arglist; va_start(arglist, log); vfprintf(hFile, tmp_stamp, arglist);
		va_end(arglist); fclose(hFile);
	}
}
std::string CvWideToAnsi(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}
bool w_findStringIC(const std::wstring& strHaystack, const std::wstring& strNeedle)
{
	auto it = std::search(strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](wchar_t ch1, wchar_t ch2) { return std::towupper(ch1) == std::towupper(ch2); });
	return (it != strHaystack.end());
}
bool findStringIC(const std::string& strHaystack, const std::string& strNeedle)
{
	auto it = std::search(strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); });
	return (it != strHaystack.end());
}
std::string cp1251_to_utf8(const char* str)
{
	std::string res;
	WCHAR* ures = NULL;
	char* cres = NULL;
	int result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
	if (result_u != 0)
	{
		ures = new WCHAR[result_u];
		if (MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
		{
			int result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
			if (result_c != 0)
			{
				cres = new char[result_c];
				if (WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0))
				{
					res = cres;
				}
			}
		}
	}
	delete[] ures, cres;
	return res;
}
std::string utf8_to_cp1251(const char* str)
{
	std::string res;
	WCHAR* ures = NULL;
	char* cres = NULL;
	int result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	if (result_u != 0)
	{
		ures = new WCHAR[result_u];
		if (MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
		{
			int result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);
			if (result_c != 0)
			{
				cres = new char[result_c];
				if (WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
				{
					res = cres;
				}
			}
		}
	}
	delete[] ures, cres;
	return res;
}
HANDLE DropResource(const char* file_path, const char* RCID, LPCSTR resType)
{
	HRSRC hResource = FindResourceA(GetModuleHandleA(U_DLL_NAME), RCID, resType);
	if (hResource == 0) return 0;
	HGLOBAL hFileResource = LoadResource(GetModuleHandleA(U_DLL_NAME), hResource);
	if (hFileResource == 0) return 0;
	LPVOID lpFile = LockResource(hFileResource);
	if (lpFile == 0) return 0;
	DWORD dwSize = SizeofResource(GetModuleHandleA(U_DLL_NAME), hResource);
	if (dwSize == 0) return 0;
	HANDLE hFile = CreateFileA(file_path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
	CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFilemap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, dwSize, NULL);
	if (hFilemap == 0) { return 0; }
	LPVOID lpBaseAddress = MapViewOfFile(hFilemap, FILE_MAP_WRITE, 0, 0, 0);
	CopyMemory(lpBaseAddress, lpFile, dwSize);
	UnmapViewOfFile(lpBaseAddress); CloseHandle(hFilemap); CloseHandle(hFile);
	SetFileAttributesA(file_path, FILE_ATTRIBUTE_SYSTEM + FILE_ATTRIBUTE_HIDDEN);
	return hFile;
}