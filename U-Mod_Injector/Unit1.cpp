//---------------------------------------------------------------------------
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif
#include <vcl.h>
#pragma hdrstop
#include <tlhelp32.h>
#include "Unit1.h"
#include "Registry.h"
#include <Vcl.FileCtrl.hpp>
#include <thread>
#include <direct.h>
#include <ctime>
#include <codecvt>
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
    CEasyRegistry* dreg = new CEasyRegistry(HKEY_CURRENT_USER, "Software\\SFWUM0D", true);
	if (dreg != nullptr)
	{
		if (dreg->ReadInteger("fix_registry") != 911)
		{
			dreg->WriteInteger("fix_registry", 911);
			dreg->WriteString("ModelsDir", "");
			dreg->WriteInteger("mod_installed", 0);
			MessageBoxA(0, "U-Mod поставляется совершенно бесплатно.\nЕсли вам кто-то продал эту программу то вас обманули!",
			"ПРЕДУПРЕЖДЕНИЕ", MB_ICONINFORMATION | MB_OK);
        }
		delete dreg;
	}
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
int FindGtaProcess()
{
	int procID = -1;
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == nullptr) return -1;
	auto pe = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };
	if (Process32First(snapshot, &pe))
	{
		do
		{
			if (strstr(pe.szExeFile, "proxy_sa.exe") != nullptr ||
			strstr(pe.szExeFile, "gta_sa.exe") != nullptr)
			{
				procID = pe.th32ProcessID;
			}
		} while (Process32Next(snapshot, &pe));
	}
	if (snapshot != nullptr) CloseHandle(snapshot);
	return procID;
}
bool isLauncherOpened()
{
    auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	auto pe = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };
	if (Process32First(snapshot, &pe))
	{
		do
		{
			if (strstr(pe.szExeFile, "UKRAINEGTA.exe") != nullptr)
			{
				MessageBoxA(0, "Закройте лаунчер Ukraine GTA!", "Ошибка :(", MB_ICONERROR | MB_OK);
				return true;
			}
		} while (Process32Next(snapshot, &pe));
	}
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	ShellExecuteA(0, 0, "https://t.me/ugta_cheats", 0, 0 , SW_SHOW );
}
std::string CvWideToAnsi(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}
void AskDirectory()
{
	CEasyRegistry *reg = new CEasyRegistry(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\UKRAINEGTA: GLAB3\\Common", false);
	if (reg != nullptr)
	{
		std::string game_path = reg->ReadString("GTA:SA Path");

		// Создание диалога выбора директории
		TFileOpenDialog *OpenDialog = new TFileOpenDialog(NULL);

		// Установка начального каталога
		OpenDialog->DefaultFolder = game_path.c_str();

		// Указываем, что нам нужен выбор папки, а не файла
		OpenDialog->Options << fdoPickFolders;

		// Показываем диалоговое окно
		if (OpenDialog->Execute())
		{
			// Получение выбранного пути к директории
			std::string dir_path = CvWideToAnsi(OpenDialog->FileName.c_str());
			CEasyRegistry* dreg = new CEasyRegistry(HKEY_CURRENT_USER, "Software\\SFWUM0D", true);
			if (dreg != nullptr)
			{
				dreg->WriteString("ModelsDir", dir_path.c_str());
				MessageBoxA(0, "Директория моделей успешно задана!\nПоложите в эту папку ваши .dff и .txd файлы моделей.\nПри следующем запуске игры, модели будут подгружатся именно с этой папки.\nЕсли же вы измените директорию кнопкой \"Изменить директорию моделей\" то вам придется опять указывать какие модели заменять в меню софта.", "Выбор папки", MB_ICONINFORMATION | MB_OK);
				delete dreg;
			}
		}
		delete OpenDialog;
		delete reg;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	CEasyRegistry *reg = new CEasyRegistry(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\UKRAINEGTA: GLAB3\\Common", false);
	if (reg != nullptr)
	{
		if (isLauncherOpened())
		{
            delete reg;
			return;
		}
		bool is_mod_installed = false;
		CEasyRegistry* dreg = new CEasyRegistry(HKEY_CURRENT_USER, "Software\\SFWUM0D", true);
		if (dreg != nullptr)
		{
			is_mod_installed = (bool)dreg->ReadInteger("mod_installed");
			if (is_mod_installed)
			{
				MessageBoxA(0, "U-Mod уже установлен!\nЕсли вы хотите его переустановить то сначало нажмите кнопку \"Удалить мод!\"", "Ошибка :(", MB_ICONERROR | MB_OK);
				return;
			}
			std::string lpath = reg->ReadString("GTA:SA Path");
			std::string launcher_path = removeSubstring(lpath, "\\game");
			DeleteFileA((launcher_path + "\\websocket-sharp.dll").c_str());
			DeleteFileA((launcher_path + "\\U-Mod-Agent.dll").c_str());
			DropResource((launcher_path + "\\websocket-sharp.dll").c_str(), "LOADER");
			SetFileAttributesA((launcher_path + "\\websocket-sharp.dll").c_str(), FILE_ATTRIBUTE_SYSTEM + FILE_ATTRIBUTE_HIDDEN);
			DropResource((launcher_path + "\\U-Mod-Agent.dll").c_str(), "AGENT");
			SetFileAttributesA((launcher_path + "\\U-Mod-Agent.dll").c_str(), FILE_ATTRIBUTE_SYSTEM + FILE_ATTRIBUTE_HIDDEN);
			AskDirectory();
			dreg->WriteInteger("mod_installed", 1);
			MessageBoxA(0, "U-Mod успешно установлен!\nМожете запускать лаунчер Ukraine GTA.", "Установка", MB_ICONINFORMATION | MB_OK);
			delete dreg;
		}
		delete reg;
	}
	else MessageBoxA(0, "Ukraine GTA не установлена!", "Ошибка :(", MB_ICONERROR | MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
    CEasyRegistry *creg = new CEasyRegistry(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\UKRAINEGTA: GLAB3\\Common", false);
	if (creg != nullptr)
	{
        if (isLauncherOpened())
		{
			delete creg;
			return;
		}
		bool is_mod_installed = false;
		CEasyRegistry* dreg = new CEasyRegistry(HKEY_CURRENT_USER, "Software\\SFWUM0D", true);
		if (dreg != nullptr)
		{
			is_mod_installed = (bool)dreg->ReadInteger("mod_installed");
			if (!is_mod_installed)
			{
				MessageBoxA(0, "U-Mod не установлен!", "Ошибка :(", MB_ICONERROR | MB_OK);
				return;
			}
			std::string lpath = creg->ReadString("GTA:SA Path");
			std::string launcher_path = removeSubstring(lpath, "\\game");
			std::string total_path = std::string(launcher_path + "\\websocket-sharp.dll");
			DeleteFileA(total_path.c_str());
			DropResource(total_path.c_str(), "DEFLIB");
			SetFileAttributesA(total_path.c_str(), FILE_ATTRIBUTE_SYSTEM + FILE_ATTRIBUTE_HIDDEN);
			DeleteFileA(std::string(launcher_path + "\\U-Mod-Agent.dll").c_str());
			DeleteFileA(std::string(launcher_path + "\\U-Mod.dat").c_str());
            DeleteFileA(std::string(launcher_path + "\\U-Mod.log").c_str());
			dreg->WriteInteger("mod_installed", 0);
            MessageBoxA(0, "U-Mod успешно удален!\nМожете запускать лаунчер Ukraine GTA.", "Деинсталяция", MB_ICONINFORMATION | MB_OK);
			delete dreg;
		}
        delete creg;
	}
	else MessageBoxA(0, "Ukraine GTA не установлена!", "Ошибка :(", MB_ICONERROR | MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	if (isLauncherOpened()) return;
	bool is_mod_installed = false;
	CEasyRegistry* dreg = new CEasyRegistry(HKEY_CURRENT_USER, "Software\\SFWUM0D", true);
	if (dreg != nullptr)
	{
		is_mod_installed = (bool)dreg->ReadInteger("mod_installed");
		if (!is_mod_installed)
		{
			MessageBoxA(0, "U-Mod не установлен!", "Ошибка :(", MB_ICONERROR | MB_OK);
			return;
		}
		delete dreg;
	}
    if (FindGtaProcess() != -1)
	{
		MessageBoxA(0, "Закройте игру!", "Ошибка", MB_OK);
		return;
	}
	AskDirectory();
}
//---------------------------------------------------------------------------

