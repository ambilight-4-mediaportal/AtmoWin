#include "StdAfx.h"
#include "atmoremotecontrolimplex.h"
#include "atmodefs.h"
#include "AtmoTools.h"
#include "atmoconnection.h"
#include "atmoxmlconfig.h"
#include "ObjectModel.h"

#include "AtmoLiveView.h"


CAtmoRemoteControlImplEx::CAtmoRemoteControlImplEx(CAtmoDynData *pAtmoDynData) : m_RefCounter(1)
{
	this->m_pAtmoDynData = pAtmoDynData;
	m_pLiveViewControl = new CAtmoLiveViewControlImpl(pAtmoDynData);

}

CAtmoRemoteControlImplEx::~CAtmoRemoteControlImplEx(void)
{
	m_pLiveViewControl->Release();
}

STDMETHODIMP CAtmoRemoteControlImplEx::QueryInterface(REFIID Id, void** pPointer)
{
	if (IsEqualIID(Id, IID_IUnknown))  
	{
		*pPointer = this;
	} else if (IsEqualIID(Id, IID_IAtmoRemoteControl)) 
	{
		*pPointer = this;
	} else if (IsEqualIID(Id, IID_IAtmoRemoteControl2)) 
	{
		*pPointer = this;
	} else if (IsEqualIID(Id, IID_IAtmoLiveViewControl) && (m_pAtmoDynData->getAtmoConfig()->getEffectMode() == emLivePicture)) 
	{
		*pPointer = m_pLiveViewControl;
		m_pLiveViewControl->AddRef();
		return S_OK;
	} else
	{
		*pPointer = NULL;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CAtmoRemoteControlImplEx::AddRef()
{
	m_RefCounter++;
	return m_RefCounter;
}

STDMETHODIMP_(ULONG) CAtmoRemoteControlImplEx::Release()
{
	ULONG Temp = --m_RefCounter;
	if (!Temp)
		delete this;
	return Temp;
}


EffectMode CAtmoRemoteControlImplEx::ConvertEffectCom2Atmo(ComEffectMode cem) 
{
	switch(cem) 
	{
	case cemDisabled:    return emDisabled;
	case cemStaticColor: return emStaticColor;
	case cemLivePicture: return emLivePicture;
	case cemColorChange: return emColorChange;
	case cemLrColorChange : return emLrColorChange;
	//case cemColorMode : return emColorMode;

	}
	return emUndefined;
}

ComEffectMode CAtmoRemoteControlImplEx::ConvertEffectAtmo2Com(EffectMode em)
{
	switch(em) 
	{
	case emDisabled:       return cemDisabled;
	case emStaticColor:    return cemStaticColor;
	case emLivePicture:    return cemLivePicture;
	case emColorChange:    return cemColorChange;
	case emLrColorChange : return cemLrColorChange;
	//case emColorMode : return cemColorMode;
	}
	return cemUndefined;
}


STDMETHODIMP CAtmoRemoteControlImplEx::setEffect(enum ComEffectMode dwEffect, enum ComEffectMode *dwOldEffect) 
{
	if (dwEffect==cemColorMode)
	{
		std::string newprofile;
		m_pAtmoDynData->LockCriticalSection();

		CAtmoConnection *connection = this->m_pAtmoDynData->getAtmoConnection();
		if((connection!=NULL) && (connection->isOpen())) 
		{
			CAtmoConfig *pConfig = m_pAtmoDynData->getAtmoConfig();
			CUtils *Utils = new CUtils;
			__int64 count = Utils->profiles.GetCount();

			//modeswitch...
			for (int i=0;i<count;++i)
				if ( Utils->profiles[i]==pConfig->lastprofile) 
				{
					if (i==count-1) 
						newprofile=Utils->profiles[0];
					else 
					newprofile=Utils->profiles[i+1];
				}
				if  ( pConfig->lastprofile=="" && count>0) 
					newprofile=Utils->profiles[0];
				pConfig->lastprofile=newprofile;

			pConfig->LoadSettings(pConfig->lastprofile); 
			CAtmoTools::RecreateConnection(this->m_pAtmoDynData);
			//CAtmoTools::SwitchEffect(this->m_pAtmoDynData, emLivePicture);

		}
		m_pAtmoDynData->UnLockCriticalSection();
		return S_OK;
	}
	else
	{
		EffectMode oldEm = CAtmoTools::SwitchEffect(this->m_pAtmoDynData, ConvertEffectCom2Atmo(dwEffect));
		if(dwOldEffect!=NULL)
			*dwOldEffect = ConvertEffectAtmo2Com(oldEm);

		if(oldEm == emUndefined)
			return S_FALSE;
		else
			return S_OK;
	}
}

STDMETHODIMP CAtmoRemoteControlImplEx::getEffect(enum ComEffectMode *dwEffect) 
{
	m_pAtmoDynData->LockCriticalSection();

	CAtmoConfig *atmoConfig = this->m_pAtmoDynData->getAtmoConfig();
	if(atmoConfig!=NULL)
		*dwEffect = ConvertEffectAtmo2Com(atmoConfig->getEffectMode());

	m_pAtmoDynData->UnLockCriticalSection();

	if(atmoConfig == NULL)
		return S_FALSE;
	else
		return S_OK;
}


STDMETHODIMP CAtmoRemoteControlImplEx::setStaticColor(BYTE red, BYTE green, BYTE blue)
{
	m_pAtmoDynData->LockCriticalSection();

	CAtmoConnection *connection = this->m_pAtmoDynData->getAtmoConnection();
	if((connection!=NULL) && (connection->isOpen()))
	{

		CAtmoConfig *pConfig = m_pAtmoDynData->getAtmoConfig();
		pColorPacket newColors;
		int zoneCount = pConfig->getZoneCount();

		AllocColorPacket(newColors, zoneCount);

		for(int i=0;i<zoneCount;i++) 
		{
			newColors->zone[i].r = red;
			newColors->zone[i].g = green;
			newColors->zone[i].b = blue;
		}

		newColors = CAtmoTools::ApplyGamma(pConfig, newColors);

		if(pConfig->isUseSoftwareWhiteAdj()) 
			newColors = CAtmoTools::WhiteCalibration(pConfig, newColors);

		connection->SendData(newColors);

		delete (char *)newColors;
	}

	this->m_pAtmoDynData->UnLockCriticalSection();
	return S_OK;
}

STDMETHODIMP CAtmoRemoteControlImplEx::setChannelStaticColor(int channel, BYTE red, BYTE green,  BYTE blue)
{
	this->m_pAtmoDynData->LockCriticalSection();

	tRGBColor newColor;
	newColor.r = red;
	newColor.g = green;
	newColor.b = blue;

	CAtmoConnection *connection = this->m_pAtmoDynData->getAtmoConnection();
	if((connection!=NULL) && (connection->isOpen())) 
	{
		connection->setChannelColor(channel, newColor);
	}
	this->m_pAtmoDynData->UnLockCriticalSection();
	return S_OK;
}
STDMETHODIMP CAtmoRemoteControlImplEx::setChannelValues(SAFEARRAY *channel_values)
{
	if(!channel_values) return S_FALSE;
	ATMO_BOOL result = ATMO_FALSE;
	unsigned char *pChannel_Values;

	SafeArrayAccessData(channel_values,(void **)&pChannel_Values);
	int numElements = channel_values->rgsabound[0].cElements;

	this->m_pAtmoDynData->LockCriticalSection();

	CAtmoConnection *connection = this->m_pAtmoDynData->getAtmoConnection();
	if((connection!=NULL) && (connection->isOpen())) {
		result = connection->setChannelValues(numElements, pChannel_Values);
	}

	this->m_pAtmoDynData->UnLockCriticalSection();

	SafeArrayUnaccessData(channel_values);

	if(result == ATMO_TRUE)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CAtmoRemoteControlImplEx::getLiveViewRes(/* [out] */ int *Width, /* [out] */ int *Height)
{
	*Width = CAP_WIDTH;
	*Height = CAP_HEIGHT;
	return S_OK;
}
