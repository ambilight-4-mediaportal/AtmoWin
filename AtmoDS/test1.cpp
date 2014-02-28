// test1.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
//

#include "stdafx.h"

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
  	return DllEntryPoint((HINSTANCE)(hModule), ul_reason_for_call, lpReserved);
}

