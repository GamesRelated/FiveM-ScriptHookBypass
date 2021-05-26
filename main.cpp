#include "pch.h"
#include "CustomAPI.h"
#include <windows.h>
#include <iostream>
#include <thread>

class metadataloader {
public:
    char pad[16];
    __int64 m_st;
};

using namespace std::string_literals;
__int64 found = 0, cslua = 0, runtime = 0;

using dofile_ptr_t = __int64(_fastcall*)(__int64, int, int, int, __int64, __int64);
using LoadFile_call = __int64(*)(__int64 metadataloader, const std::string& filename);
dofile_ptr_t dofile_ptr = nullptr;

void Bitizy() {

    LoadFile_call LoadFile = 0;
    while (!LoadFile)
        LoadFile = (LoadFile_call)(CustomAPI::GetModuleA("citizen-resources-metadata-lua.dll") + 0x81ED0);
    while (!cslua)
        cslua = CustomAPI::GetModuleA("citizen-scripting-lua");
    while (!dofile_ptr)
        dofile_ptr = (dofile_ptr_t)(cslua + 0x15E1E0);

    metadataloader obj;
    metadataloader* metadata_ptr = &obj;

    while (true) {
        if (!found) {
            __int64 gr;
            gr = *(__int64*)(cslua + 0x421630); 
            if (gr != 0)
            {
                metadata_ptr->m_st = *(__int64*)(gr + 72);
                runtime = gr;
                found = 1;
            }
        }

        else if (GetAsyncKeyState(VK_F1)) {

            *(BYTE*)(CustomAPI::GetModuleA("adhesive") + 0x49288C) = 1; 
            *(__int64*)(cslua + 0x63CFA0) = runtime;
            LoadFile((__int64)metadata_ptr, "C:\\Users\\Public\\a.lua");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            dofile_ptr(metadata_ptr->m_st, 0, -1, 0, 0, NULL);
            *(BYTE*)(CustomAPI::GetModuleA("adhesive") + 0x49288C) = 0;
            std::this_thread::sleep_for(std::chrono::seconds(10));

        }

    }

}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Bitizy, 0, 0, 0);
    return TRUE;
}

