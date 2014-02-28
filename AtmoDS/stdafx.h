// stdafx.h : Includedatei für Standardsystem-Includedateien,
// oder projektspezifische Includedateien, die häufig benutzt, aber
// in unregelmäßigen Abständen geändert werden.
//

#pragma once


#define WINVER 0x0500
#define _WIN32_WINNT 0x0500


// Insert your headers here
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Windows-Headerdateien:
#include <windows.h>

// TODO: Verweisen Sie hier auf zusätzliche Header, die Ihr Programm erfordert
#include <streams.h>
#include <tchar.h>
#include <measure.h>
#include <Wxutil.h>
#include <Objbase.h>
#include <mmreg.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <initguid.h>
#include <dvdmedia.h>
