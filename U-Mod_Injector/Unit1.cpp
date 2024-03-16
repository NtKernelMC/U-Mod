//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tlhelp32.h>
#include "Unit1.h"
#include "Registry.h"
#include <thread>
#include <direct.h>
#include <ctime>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
std::string RandomString(int len)
{
   std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   std::string newstr;
   int pos; while(newstr.size() != len)
   {
    pos = ((rand() % (str.size() - 1)));
    newstr += str.substr(pos,1);
   }
   return newstr;
}
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
	srand(time(NULL)); Form1->Caption = RandomString(20).c_str();
}
bool __stdcall IsDirectoryExists(const std::string& dirName_in)
{
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;
    return false;
};
HANDLE DropResource(const char* file_path, const char* RCID)
{
	HRSRC hResource = FindResourceA(NULL, RCID, RT_RCDATA);
	if (hResource == 0) return 0;
	HGLOBAL hFileResource = LoadResource(NULL, hResource);
	if (hFileResource == 0) return 0;
	LPVOID lpFile = LockResource(hFileResource);
	if (lpFile == 0) return 0;
	DWORD dwSize = SizeofResource(NULL, hResource);
	if (dwSize == 0) return 0;
	HANDLE hFile = CreateFileA(file_path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
	CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFilemap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, dwSize, NULL);
	if (hFilemap == 0) { return 0; }
	LPVOID lpBaseAddress = MapViewOfFile(hFilemap, FILE_MAP_WRITE, 0, 0, 0);
	CopyMemory(lpBaseAddress, lpFile, dwSize);
	UnmapViewOfFile(lpBaseAddress); CloseHandle(hFilemap); CloseHandle(hFile);
	return hFile;
}
HHOOK handle = NULL;
BOOL CALLBACK EnumWindowsProcMy(HWND hwnd,LPARAM lParam)
{
	DWORD lpdwProcessId = 0x0; AnsiString selt(Form1->ComboBox1->Text);
	DWORD tid = GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	char tmpID[50]; sprintf(tmpID, "%d", lpdwProcessId);
	if(strstr(selt.c_str(), tmpID) != nullptr)
	{
		if (!IsDirectoryExists("UmodData")) CreateDirectoryA("UmodData", NULL);
		char cwd[356]; _getcwd(cwd, sizeof(cwd));
		char tmpDir[256]; sprintf(tmpDir,
		"%s\\UmodData\\%s", cwd, selt.c_str());
		if (!IsDirectoryExists(tmpDir)) CreateDirectoryA(tmpDir, NULL);
		strcat(tmpDir, "\\"); strcat(tmpDir, "U-Mod.dll");
		DropResource(tmpDir, "UMOD");
		HMODULE dll = LoadLibraryExA(tmpDir, NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (dll == NULL)
		{
			wchar_t tmpErr[256]; swprintf(tmpErr,
			L"Не удалось загрузить библиотеку. Code: %d\n%ls", GetLastError(), tmpDir);
			MessageBoxW(0, tmpErr, L"Ошибка природы :(", MB_OK);
			return FALSE;
		}
		HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "NextHook");
		if (addr == NULL)
		{
			wchar_t tmpErr[256]; swprintf(tmpErr,
			L"Не удалось вытащить экспорт. Code: %d\n%ls", GetLastError(), tmpDir);
			MessageBoxW(0, tmpErr, L"Ошибка природы :(", MB_OK);
			return FALSE;
		}
		handle = SetWindowsHookEx(WH_GETMESSAGE, addr, dll, tid);
		if (handle == nullptr)
		{
			wchar_t tmpErr[256]; swprintf(tmpErr,
			L"Не удалось поставить хук. Code: %d", GetLastError());
			MessageBoxW(0, tmpErr, L"Ошибка природы :(", MB_OK);
			return FALSE;
		}
		PostThreadMessage(tid, WM_NULL, NULL, NULL);
		MessageBoxW(0, L"DLL-библиотека успешно загружена!", L"Успех", MB_OK);
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	EnumWindows(EnumWindowsProcMy, 0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1DropDown(TObject *Sender)
{
	Form1->ComboBox1->Items->Clear();
    auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	auto pe = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };
	if (Process32First(snapshot, &pe))
	{
		do
		{
			if (strstr(pe.szExeFile, "proxy_sa.exe") != nullptr ||
			strstr(pe.szExeFile, "gta_sa.exe") != nullptr)
			{
				char tmpProc[256]; sprintf(tmpProc,
				"PID %d ; %s", pe.th32ProcessID, pe.szExeFile);
				Form1->ComboBox1->Items->Add(tmpProc);
			}
		} while (Process32Next(snapshot, &pe));
	}
	CloseHandle(snapshot);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	ShellExecuteA(0, 0, "https://t.me/ugta_cheats", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

