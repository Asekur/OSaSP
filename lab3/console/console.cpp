// console.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "windows.h"
#include "..\\dll\\dllMain.h"
//описание типа указателя на функцию
typedef void (*Func)();

void dynamicDLL() {
	HMODULE hMod = LoadLibraryA("D:\\Для вот этого мусора\\github\\OSaSP\\lab3\\dll\\Debug\\dll.dll");
	if (hMod != NULL) {
		Func function = (Func)GetProcAddress(hMod, "changeString");
		if (function != NULL) {
			function();
		}
		FreeLibrary(hMod);
	}
}

void staticDLL() {
	changeString();
}

int main()
{
	//dynamicDLL();
	staticDLL();
}
