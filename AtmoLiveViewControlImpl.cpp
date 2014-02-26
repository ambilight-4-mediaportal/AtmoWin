#include "StdAfx.h"
#include "atmoliveviewcontrolimpl.h"
#include "AtmoExternalCaptureInput.h"
#include "AtmoTools.h"

CAtmoLiveViewControlImpl::CAtmoLiveViewControlImpl(CAtmoDynData *pAtmoDynData) : m_RefCounter(1) {
    this->m_pAtmoDynData = pAtmoDynData;
}

CAtmoLiveViewControlImpl::~CAtmoLiveViewControlImpl(void)
{
}

STDMETHODIMP CAtmoLiveViewControlImpl::QueryInterface(REFIID Id, void** pPointer)
{
    if (IsEqualIID(Id, IID_IUnknown))  {
		*pPointer = this;
    } else if (IsEqualIID(Id, IID_IAtmoLiveViewControl)) {
		*pPointer = this;
    } else
	{
	  *pPointer = NULL;
	  return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CAtmoLiveViewControlImpl::AddRef()
{
    m_RefCounter++;
	return m_RefCounter;
}

STDMETHODIMP_(ULONG) CAtmoLiveViewControlImpl::Release()
{
	ULONG Temp = --m_RefCounter;
	if (!Temp)
		delete this;
	return Temp;
}

STDMETHODIMP CAtmoLiveViewControlImpl::setLiveViewSource(enum ComLiveViewSource dwModus) {
    HRESULT hres = S_FALSE;
    m_pAtmoDynData->LockCriticalSection();

    CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();
    if(config!=NULL) {
       if(config->getEffectMode() == emLivePicture) {
          switch(dwModus) {
               case lvsGDI: {
                    CAtmoTools::SwitchLiveSource(m_pAtmoDynData, lpsScreenCapture ); 
                    hres = S_OK;
                    break;
               }

               case lvsExternal: {
                    CAtmoTools::SwitchLiveSource(m_pAtmoDynData, lpsExtern );
                    hres = S_OK;
                    break;
               }
          }
       } 
    }

    m_pAtmoDynData->UnLockCriticalSection();
    return hres;
}

STDMETHODIMP CAtmoLiveViewControlImpl::getCurrentLiveViewSource(enum ComLiveViewSource *modus) {
    HRESULT hres = S_FALSE;
    m_pAtmoDynData->LockCriticalSection();

    CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();
    if(config!=NULL) {
       if(config->getEffectMode() == emLivePicture) {
           switch(m_pAtmoDynData->getLivePictureSource())
           {
           case lpsDisabled: 
                break; 
           case lpsScreenCapture:
                  *modus = lvsGDI;
                  hres = S_OK;
                break;
           case lpsExtern:
                  *modus = lvsExternal;
                  hres = S_OK;
                break;
           }
       }
    }

    m_pAtmoDynData->UnLockCriticalSection();
    return hres;
}

STDMETHODIMP CAtmoLiveViewControlImpl::setPixelData(SAFEARRAY * bitmapInfoHeader, SAFEARRAY * pixelData) {
    HRESULT hres = S_FALSE;

    CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();
    if(config != NULL) {

      if(config->getEffectMode() == emLivePicture)  {

        CAtmoLiveView *pAtmoLiveView = (CAtmoLiveView *)m_pAtmoDynData->getEffectThread();
        if(pAtmoLiveView!= NULL) {

            if( m_pAtmoDynData->getLivePictureSource() == lpsExtern ) {
                CAtmoExternalCaptureInput *input = (CAtmoExternalCaptureInput *)m_pAtmoDynData->getLiveInput();

                if(input != NULL) {
                    void *pPixelData;
                    BITMAPINFOHEADER *pBitmapInfoHeader;

                    SafeArrayAccessData(pixelData,(void **)&pPixelData);
                    SafeArrayAccessData(bitmapInfoHeader,(void **)&pBitmapInfoHeader);

                    input->DeliverNewSourceDataPaket(pBitmapInfoHeader,pPixelData);

                    SafeArrayUnaccessData(pixelData);
                    SafeArrayUnaccessData(bitmapInfoHeader);
                }

            }
        }
      }
    }

    return hres;
}

