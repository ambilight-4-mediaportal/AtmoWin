#include "stdafx.h"
#include "atmoremotecontrolimpl.h"
#include "AtmoLiveView.h"
#include "AtmoExternalCaptureInput.h"
#include "AtmoDefs.h"

CAtmoRemoteControlImpl::CAtmoRemoteControlImpl(CAtmoDynData *pDynData) : m_RefCounter(1), m_LiveViewSource(lvsGDI) {
   this->m_pAtmoDynData = pDynData;
}

CAtmoRemoteControlImpl::~CAtmoRemoteControlImpl(void) {
}

STDMETHODIMP CAtmoRemoteControlImpl::QueryInterface(REFIID Id, void** pPointer)
{
    if (IsEqualIID(Id, IID_IUnknown))  {
		*pPointer = this;
    } else if (IsEqualIID(Id, IID_IAtmoWinRemoteControl)) {
		*pPointer = this;
    } else
	{
	  *pPointer = NULL;
	  return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CAtmoRemoteControlImpl::AddRef()
{
    m_RefCounter++;
	return m_RefCounter;
}

STDMETHODIMP_(ULONG) CAtmoRemoteControlImpl::Release()
{
	ULONG Temp = --m_RefCounter;
	if (!Temp)
		delete this;
	return Temp;
}

STDMETHODIMP CAtmoRemoteControlImpl::getCurrentLiveViewSource(ComLiveViewSource *modus) {
    CAtmoConfig *cfg = this->m_pAtmoDynData->getAtmoConfig();
    CAtmoLiveView *liveViewThread;
    if(cfg->getEffectMode() == emLivePicture) {
       liveViewThread = (CAtmoLiveView *)m_pAtmoDynData->getEffectThread();
       if(liveViewThread != NULL) {
          LiveEffectSource les = liveViewThread->getLiveEffectSource();
          if(les == lesGDI)
             *modus = lvsGDI;
          if(les == lesExternal)
             *modus = lvsExternal;
       }
    } else {
        *modus = lvsGDI; 
    }
    return S_OK;
}

 
 STDMETHODIMP CAtmoRemoteControlImpl::setLiveViewSource(ComLiveViewSource dwModus) {
     CAtmoConfig *cfg = this->m_pAtmoDynData->getAtmoConfig();
     CAtmoLiveView *liveViewThread;
     switch(dwModus) {
            case lvsGDI:
                 if(cfg->getEffectMode() == emLivePicture) {
                    liveViewThread = (CAtmoLiveView *)m_pAtmoDynData->getEffectThread();
                    liveViewThread->setLiveEffectSource(lesGDI);
                    Sleep(50);
                 }
                 break;
            case lvsExternal:
                 if(cfg->getEffectMode() == emLivePicture) {
                    liveViewThread = (CAtmoLiveView *)m_pAtmoDynData->getEffectThread();
                    liveViewThread->setLiveEffectSource(lesExternal);
                    // weil diese Änderung nicht sofort eintritt - sondern nur alle paar ms darauf reagiert wird wartet ich hier!
                    Sleep(50);
                 }
                 break;
     }
   return S_OK;
}
 
STDMETHODIMP CAtmoRemoteControlImpl::setPixelData(SAFEARRAY *bitmapInfoHeader, SAFEARRAY *pixelData) {
   void *pPixelData;
   BITMAPINFOHEADER *pBitmapInfoHeader;

   CAtmoConfig *cfg = this->m_pAtmoDynData->getAtmoConfig();
   CAtmoLiveView *liveViewThread;


   if(cfg->getEffectMode() == emLivePicture) {
      liveViewThread = (CAtmoLiveView *)m_pAtmoDynData->getEffectThread();
      if(liveViewThread!=NULL) {
         if(liveViewThread->getLiveEffectSource() == lesExternal) {
             CAtmoExternalCaptureInput *input = (CAtmoExternalCaptureInput *)liveViewThread->LockAtmoInput();
             if(input!=NULL) {
                SafeArrayAccessData(pixelData,(void **)&pPixelData);
                SafeArrayAccessData(bitmapInfoHeader,(void **)&pBitmapInfoHeader);

                input->DeliverNewSourceDataPaket(pBitmapInfoHeader,pPixelData);

                SafeArrayUnaccessData(pixelData);
                SafeArrayUnaccessData(bitmapInfoHeader);
             }
             liveViewThread->UnlockAtmoInput();
         }
      }
   }

   return S_OK;
}

