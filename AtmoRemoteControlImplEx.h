#ifndef _AtmoRemoteControlImplEx_h_
#define _AtmoRemoteControlImplEx_h_

#include <comdef.h>		
#include "AtmoWin_h.h"
#include "AtmoDynData.h"
#include "AtmoLiveViewControlImpl.h"

class CAtmoRemoteControlImplEx : public IAtmoRemoteControl2
{
private:
	CAtmoDynData *m_pAtmoDynData;
	CAtmoLiveViewControlImpl *m_pLiveViewControl;

	/* COM related declarations ...*/
private:
	ULONG m_RefCounter;	// Referenzzähler

public:
	STDMETHODIMP 			QueryInterface(REFIID, void**);
	STDMETHODIMP_(ULONG) 	AddRef(void);
	STDMETHODIMP_(ULONG) 	Release(void);

	STDMETHODIMP setEffect(enum ComEffectMode dwEffect, enum ComEffectMode *dwOldEffect);
	STDMETHODIMP getEffect(enum ComEffectMode *dwEffect);
	STDMETHODIMP setStaticColor(BYTE red, BYTE green,  BYTE blue);

	STDMETHODIMP setChannelStaticColor(int channel, BYTE red, BYTE green,  BYTE blue);

	STDMETHODIMP setChannelValues(SAFEARRAY *channel_values);

public: // IAtmoRemoteControl2
	STDMETHODIMP getLiveViewRes(/* [out] */ int *Width, /* [out] */ int *Height);

private:
	EffectMode ConvertEffectCom2Atmo(ComEffectMode cem);
	ComEffectMode ConvertEffectAtmo2Com(EffectMode cem);

public:
	CAtmoRemoteControlImplEx(CAtmoDynData *pAtmoDynData);
	~CAtmoRemoteControlImplEx(void);
};

#endif