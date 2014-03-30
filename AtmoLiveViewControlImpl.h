#ifndef _AtmoLiveViewControlImpl_h_
#define _AtmoLiveViewControlImpl_h_

#include <comdef.h>		
#include "AtmoWin_h.h"
#include "AtmoLiveView.h"
#include "AtmoDynData.h"

class CAtmoLiveViewControlImpl :   public IAtmoLiveViewControl
{
	/* COM related declarations ...*/
private:
	ULONG m_RefCounter;	// Referenzzähler
	CAtmoDynData *m_pAtmoDynData;

public:
	STDMETHODIMP 			QueryInterface(REFIID, void**);
	STDMETHODIMP_(ULONG) 	AddRef(void);
	STDMETHODIMP_(ULONG) 	Release(void);

	STDMETHODIMP setLiveViewSource(enum ComLiveViewSource dwModus);
	STDMETHODIMP getCurrentLiveViewSource(enum ComLiveViewSource *modus);

	STDMETHODIMP setPixelData(SAFEARRAY * bitmapInfoHeader, SAFEARRAY * pixelData);

public:
	CAtmoLiveViewControlImpl(CAtmoDynData *pAtmoDynData);
	~CAtmoLiveViewControlImpl(void);
};

#endif
