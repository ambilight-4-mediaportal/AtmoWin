#ifndef _AtmoComRegistry_h_
#define _AtmoComRegistry_h_

#include <comdef.h>		
#include "AtmoWin_h.h"
#include "AtmoDefs.h"
#include "atmoregistry.h"

class CAtmoComRegistry
{
private:
    CAtmoComRegistry(void);
    ~CAtmoComRegistry(void);

public:
    static void SaveComSettings(ATMO_BOOL force);
    static void DeleteComSettings();
};

#endif