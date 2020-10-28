// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "dllMain.h"
#include <iostream>

//возможность экспорта без использования def-файлов (файлы определения модуля)
__declspec(dllexport) void changeString() {
	std::cout << "hello";
}

BOOL APIENTRY DllMain( HMODULE hModule, //дескриптор модуля DLL
                       DWORD  ul_reason_for_call, //код причины вызова точки входа
                       LPVOID lpReserved //резервация
                     )
{
	//система запускает или завершает процесс или поток
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: //динамически: lpReserved = NULL
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH: //неудачная загрузка или успешная: lpReserved = NULL
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

