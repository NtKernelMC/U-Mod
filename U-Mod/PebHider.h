#pragma once
#include <Windows.h>
#include "lazy_import.h"
class PebHider
{
public:
    PebHider();
    ~PebHider();
    //===========================================================================
    typedef struct _UNICODE_STRING {
        USHORT Length;
        USHORT MaximumLength;
        PWCH   Buffer;
    } UNICODE_STRING;
    typedef UNICODE_STRING* PUNICODE_STRING;
    //===========================================================================
    typedef struct _PEB_LDR_DATA
    {
        ULONG           Length;
        BOOLEAN         Initialized;
        PVOID           SsHandle;
        LIST_ENTRY      InLoadOrderModuleList;
        LIST_ENTRY      InMemoryOrderModuleList;
        LIST_ENTRY      InInitializationOrderModuleList;
    } PEB_LDR_DATA, * PPEB_LDR_DATA;
    //===========================================================================
    typedef struct _LDR_MODULE
    {
        LIST_ENTRY      InLoadOrderModuleList;
        LIST_ENTRY      InMemoryOrderModuleList;
        LIST_ENTRY      InInitializationOrderModuleList;
        PVOID           BaseAddress;
        PVOID           EntryPoint;
        ULONG           SizeOfImage;
        UNICODE_STRING  FullDllName;
        UNICODE_STRING  BaseDllName;
        ULONG           Flags;
        SHORT           LoadCount;
        SHORT           TlsIndex;
        LIST_ENTRY      HashTableEntry;
        ULONG           TimeDateStamp;
    } LDR_MODULE, * PLDR_MODULE;
    //===========================================================================
    static void UnlinkModule(const char* szModule);
    static void RemovePeHeader(HMODULE GetModuleBase);
};