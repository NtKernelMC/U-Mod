#define _CRT_SECURE_NO_WARNINGS
#define DLL_AGENT_NAME xorstr_("U-MOD-Agent.dll")
#include <Windows.h>
#include <stdio.h>
#include <thread>
#include <string>
#include <vector>
#include <TlHelp32.h>
#include <direct.h>
#include "xorstr.h"
#include "resource.h"
#include "Registry.h"
HANDLE DropResource(const char* file_path, const char* RCID, LPCSTR resType)
{
	HRSRC hResource = FindResourceA(GetModuleHandleA(DLL_AGENT_NAME), RCID, resType);
	if (hResource == 0) return 0;
	HGLOBAL hFileResource = LoadResource(GetModuleHandleA(DLL_AGENT_NAME), hResource);
	if (hFileResource == 0) return 0;
	LPVOID lpFile = LockResource(hFileResource);
	if (lpFile == 0) return 0;
	DWORD dwSize = SizeofResource(GetModuleHandleA(DLL_AGENT_NAME), hResource);
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
int SearchProcess()
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	auto pe = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };
	if (Process32First(snapshot, &pe))
	{
		do
		{
			if (strstr(pe.szExeFile, xorstr_("proxy_sa.exe")) != nullptr || strstr(pe.szExeFile, xorstr_("gta_sa.exe")) != nullptr)
			{
				return pe.th32ProcessID;
			}
		} while (Process32Next(snapshot, &pe));
	}
	CloseHandle(snapshot);
	return -1;
}
BOOL InjectDLL(DWORD dwProcessId, const char* dllPath) 
{
	// Получаем хендл процесса с необходимыми правами
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
	if (!hProcess) {
		DWORD err = GetLastError();
		char mgr[400]; memset(mgr, 0, sizeof(400));
		return FALSE;
	}

	// Выделяем память в пространстве целевого процесса под путь к DLL
	LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);
	if (!pDllPath) {
		CloseHandle(hProcess);
		return FALSE;
	}

	// Копируем путь к DLL в выделенную память
	if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath,
		strlen(dllPath) + 1, NULL)) {
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	// Получаем адрес функции LoadLibraryA в kernel32.dll
	FARPROC pLoadLibraryA = GetProcAddress(GetModuleHandleA(xorstr_("kernel32.dll")), xorstr_("LoadLibraryA"));
	if (!pLoadLibraryA) {
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	// Создаем удаленный поток, который вызовет LoadLibraryA с путем к DLL
	HANDLE hThread = CreateRemoteThreadEx(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)pLoadLibraryA,
		pDllPath, 0, NULL, NULL);
	if (!hThread) {
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	// Ждем, пока поток завершит работу
	WaitForSingleObject(hThread, INFINITE);

	// Освобождаем ресурсы
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
	CloseHandle(hProcess);

	return TRUE;
}
int lastProcID = -1;
std::string removeSubstring(const std::string& original, const std::string& toRemove)
{
	std::string result = original;
	size_t pos = result.find(toRemove);
	if (pos != std::string::npos)
	{
		result.erase(pos, toRemove.length());
	}
	return result;
}
void AgentThread()
{
	std::string launcher_path = "";
	CEasyRegistry* reg = new CEasyRegistry(HKEY_LOCAL_MACHINE, xorstr_("SOFTWARE\\WOW6432Node\\UKRAINEGTA: GLAB3\\Common"), false);
	if (reg != nullptr)
	{
		std::string lpath = reg->ReadString(xorstr_("GTA:SA Path"));
		launcher_path = removeSubstring(lpath, xorstr_("\\game"));
		launcher_path += xorstr_("\\U-Mod.dll");
		delete reg;
	}
	DeleteFileA(launcher_path.c_str());
	DropResource(launcher_path.c_str(), MAKEINTRESOURCEA(IDR_RCDATA1), RT_RCDATA);
	while (true)
	{
		int procID = SearchProcess();
		if (procID != -1 && lastProcID != procID)
		{
			InjectDLL((DWORD)procID, launcher_path.c_str());
			lastProcID = procID;
		}
		Sleep(1000);
	}
}
__forceinline void __stdcall asyncThread()
{
    std::thread hacker(AgentThread);
    hacker.detach();
}
int __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: 
        asyncThread();
        break;
    }
    return 1;
}

