#pragma once

#include <comdef.h>		
#include "AtmoWin_h.h"
#include "AtmoDynData.h"
#include "AtmoConfig.h"

class CAtmoRemoteControlImpl : public IAtmoWinRemoteControl
{
private:
    ULONG m_RefCounter;	// Referenzzähler
    CAtmoDynData *m_pAtmoDynData;
    ComLiveViewSource m_LiveViewSource;

public:
    CAtmoRemoteControlImpl(CAtmoDynData *pDynData);
    ~CAtmoRemoteControlImpl(void);

public:
   	// methods for interface IUnknown
    STDMETHODIMP 			QueryInterface(REFIID, void**);
    STDMETHODIMP_(ULONG) 	AddRef(void);
    STDMETHODIMP_(ULONG) 	Release(void);

 
    STDMETHODIMP setLiveViewSource(ComLiveViewSource dwModus);
    STDMETHODIMP getCurrentLiveViewSource(ComLiveViewSource *modus);

    STDMETHODIMP setPixelData(SAFEARRAY *bitmapInfoHeader,SAFEARRAY *pixelData);
};
