/*
* AtmoTools.cpp: Collection of tool and helperfunction
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id: 45dfca6a1929a43f6514eceda43096061a929399 $
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AtmoTools.h"
#include "AtmoDynData.h"
#include "AtmoLiveView.h"
#include "AtmoClassicConnection.h"
#include "AtmoDmxSerialConnection.h"
#include "AtmoEntecSerialConnection.h"
#include "AtmoMultiConnection.h"
#include "MoMoConnection.h"
#include "FnordlichtConnection.h"
#include "AtmoExternalCaptureInput.h"
#include <math.h>
#include "Language.h"

#if !defined(_ATMO_VLC_PLUGIN_)
#   include "AtmoColorChanger.h"
#   include "AtmoLeftRightColorChanger.h"

#   include "AtmoDummyConnection.h"
#   include "AtmoNulConnection.h"
#   include "MondolightConnection.h"

#   include "AtmoGdiDisplayCaptureInput.h"
#endif


CAtmoTools::CAtmoTools(void)
{
}

CAtmoTools::~CAtmoTools(void)
{
}

void CAtmoTools::ShowShutdownColor(CAtmoDynData *pDynData)
{
	pDynData->LockCriticalSection();


	CAtmoConnection *atmoConnection = pDynData->getAtmoConnection();
	CAtmoConfig *atmoConfig = pDynData->getAtmoConfig();
	if((atmoConnection != NULL) && (atmoConfig!=NULL) && atmoConfig->isSetShutdownColor()) 
	{
		int i;
		pColorPacket packet;
		AllocColorPacket(packet, atmoConfig->getZoneCount());

		// set a special color? on shutdown of the software? mostly may use black or so ...
		// if this function ist disabled ... atmo will continuing to show the last color...
		for(i = 0; i < packet->numColors; i++) 
		{
			packet->zone[i].r = atmoConfig->getShutdownColor_Red();
			packet->zone[i].g = atmoConfig->getShutdownColor_Green();
			packet->zone[i].b = atmoConfig->getShutdownColor_Blue();
		}

		packet = CAtmoTools::ApplyGamma(atmoConfig, packet);

		if(atmoConfig->isUseSoftwareWhiteAdj())
			packet = CAtmoTools::WhiteCalibration(atmoConfig, packet);

		atmoConnection->SendData(packet);

		delete (char *)packet;

	}

	pDynData->UnLockCriticalSection();
}

EffectMode CAtmoTools::SwitchEffect(CAtmoDynData *pDynData, EffectMode newEffectMode)
{
	// may need a critical section??
	if(pDynData == NULL) 
	{
		return emUndefined;
	}
	pDynData->LockCriticalSection();

	CAtmoConfig *atmoConfig = pDynData->getAtmoConfig();
	if(atmoConfig == NULL) 
	{
		pDynData->UnLockCriticalSection();
		return emUndefined;
	}
	CAtmoConnection *atmoConnection = pDynData->getAtmoConnection();

	EffectMode oldEffectMode = atmoConfig->getEffectMode();
	CThread *currentEffect = pDynData->getEffectThread();
	CAtmoInput *currentInput = pDynData->getLiveInput();
	CAtmoPacketQueue *currentPacketQueue = pDynData->getLivePacketQueue();


	if(oldEffectMode == emLivePicture)
	{
		/* in case of disabling the live mode
		first we have to stop the input
		then the effect thread!
		*/
		if(currentInput != NULL) 
		{
			pDynData->setLiveInput( NULL );
			currentInput->Close();
			delete currentInput;
			currentInput = NULL;
		}
	}

	// stop and delete/cleanup current Effect Thread...
	pDynData->setEffectThread( NULL );
	if(currentEffect != NULL)
	{
		currentEffect->Terminate();
		delete currentEffect;
		currentEffect = NULL;
	}

	if(oldEffectMode == emLivePicture) 
	{
		/*
		and last we kill the PacketQueue used for communication between the threads
		*/
		pDynData->setLivePacketQueue( NULL );
		delete currentPacketQueue;
		currentPacketQueue = NULL;
	}

	if((atmoConnection!=NULL) && (atmoConnection->isOpen()==ATMO_TRUE))
	{
		// neuen EffectThread nur mit aktiver Connection starten...

		switch(newEffectMode) 
		{
		case emUndefined: // do nothing also in that case (avoid compiler warning)
			break;
		case emDisabled:
			{
				// get values from config - and put them to all channels?
				pColorPacket packet;
				AllocColorPacket(packet, atmoConfig->getZoneCount());

				for(int i=0; i < packet->numColors; i++)
				{
					packet->zone[i].r = 0;
					packet->zone[i].g = 0;
					packet->zone[i].b = 0;
				}

				atmoConnection->SendData( packet );
				atmoConnection->SendData( packet );

				delete (char *)packet;

				break;
			}


		case emStaticColor: 
			{
				// get values from config - and put them to all channels?
				pColorPacket packet;
				AllocColorPacket(packet, atmoConfig->getZoneCount());
				for(int i=0; i < packet->numColors; i++){
					packet->zone[i].r = atmoConfig->getStaticColor_Red();
					packet->zone[i].g = atmoConfig->getStaticColor_Green();
					packet->zone[i].b = atmoConfig->getStaticColor_Blue();
				}

				packet = CAtmoTools::ApplyGamma( atmoConfig, packet );

				if(atmoConfig->isUseSoftwareWhiteAdj())
					packet = CAtmoTools::WhiteCalibration(atmoConfig, packet);

				atmoConnection->SendData( packet );
				atmoConnection->SendData( packet );

				delete (char *)packet;

				break;
			}

		case emLivePicture: 
			{
				currentEffect = new CAtmoLiveView(pDynData);

#if !defined(_ATMO_VLC_PLUGIN_)
				CAtmoPacketQueueStatus *packetMon = NULL;
				if(atmoConfig->getShow_statistics())
				{
					packetMon = new CAtmoPacketQueueStatus(pDynData->getAppHinstance(), (HWND)NULL);
					packetMon->createWindow();
					packetMon->showWindow(SW_SHOW);
				}
				currentPacketQueue = new CAtmoPacketQueue(packetMon);
				pDynData->setLivePictureSource(lpsScreenCapture);
				currentInput = new CAtmoGdiDisplayCaptureInput( pDynData );
#else
				currentPacketQueue = new CAtmoPacketQueue();
				pDynData->setLivePictureSource(lpsExtern);
				currentInput = new CAtmoExternalCaptureInput( pDynData );
#endif
				break;
			}

#if !defined(_ATMO_VLC_PLUGIN_)
		case emColorChange:
			currentEffect = new CAtmoColorChanger(atmoConnection, atmoConfig);
			break;

		case emLrColorChange:
			currentEffect = new CAtmoLeftRightColorChanger(atmoConnection, atmoConfig);
			break;
#endif
		}

	}

	atmoConfig->setEffectMode( newEffectMode );

	pDynData->setLivePacketQueue( currentPacketQueue );
	pDynData->setEffectThread( currentEffect );
	pDynData->setLiveInput( currentInput );

	if(currentEffect != NULL)
		currentEffect->Run();
	if(currentInput != NULL)
		currentInput->Open();

	pDynData->UnLockCriticalSection();
	return oldEffectMode;
}

LivePictureSource CAtmoTools::SwitchLiveSource(CAtmoDynData *pDynData, LivePictureSource newLiveSource)
{
	LivePictureSource oldSource;
	pDynData->LockCriticalSection();

	oldSource = pDynData->getLivePictureSource();
	pDynData->setLivePictureSource( newLiveSource );

	if ((pDynData->getAtmoConfig()->getEffectMode() == emLivePicture) &&
		(pDynData->getEffectThread() != NULL) &&
		(pDynData->getLivePacketQueue() != NULL))
	{
		CAtmoInput *input = pDynData->getLiveInput();
		pDynData->setLiveInput( NULL );
		if(input != NULL)
		{
			input->Close();
			delete input;
			input = NULL;
		}

		switch(pDynData->getLivePictureSource())
		{
		case lpsDisabled: // do nothing in that case - avoid compiler warning
			break;
#if !defined(_ATMO_VLC_PLUGIN_)
		case lpsScreenCapture:
			input = new CAtmoGdiDisplayCaptureInput( pDynData );
			break;
#endif
		case lpsExtern:
			input = new CAtmoExternalCaptureInput( pDynData );
			break;
		}

		pDynData->setLiveInput( input );
		if(input != NULL)
			input->Open();
	}

	pDynData->UnLockCriticalSection();
	return oldSource;
}

ATMO_BOOL CAtmoTools::RecreateConnection(CAtmoDynData *pDynData)
{
	pDynData->LockCriticalSection();
	CLanguage *Lng = new CLanguage;

	CAtmoConnection *current = pDynData->getAtmoConnection();
	CAtmoConfig *atmoConfig = pDynData->getAtmoConfig();
	AtmoConnectionType act = atmoConfig->getConnectionType();
	pDynData->setAtmoConnection(NULL);
	if(current != NULL) 
	{
		current->CloseConnection();
		delete current;
	}

	switch(act)
	{
	case actClassicAtmo:
		{
			CAtmoClassicConnection *tempConnection = new CAtmoClassicConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE)
			{
#if !defined(_ATMO_VLC_PLUGIN_)
				if(atmoConfig->getIgnoreConnectionErrorOnStartup() == ATMO_FALSE)
				{
					char errorMsgBuf[200];
					sprintf(errorMsgBuf,Lng->sMessagesText[26] + "com%d" + Lng->sMessagesText[26] + "%d (0x%x)",
						pDynData->getAtmoConfig()->getArduComport(),
						tempConnection->getLastError(),
						tempConnection->getLastError()
						);
					MessageBox(0,errorMsgBuf,Lng->sMessagesText[3] ,MB_ICONERROR | MB_OK);
				}
#endif
				pDynData->setAtmoConnection(tempConnection);

				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping(atmoConfig->getChannelAssignment(0));

			CAtmoTools::SetChannelAssignment(pDynData,
				atmoConfig->getCurrentChannelAssignment());

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}

#if !defined(_ATMO_VLC_PLUGIN_)
	case actDummy:
		{
			// actDummy8,actDummy12,actDummy16
			CAtmoDummyConnection *tempConnection = new CAtmoDummyConnection(pDynData->getResHinstance(),
				atmoConfig);
			if(tempConnection->OpenConnection() == ATMO_FALSE) 
			{
				pDynData->setAtmoConnection(tempConnection);
				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping(atmoConfig->getChannelAssignment(0));

			CAtmoTools::SetChannelAssignment(pDynData, pDynData->getAtmoConfig()->getCurrentChannelAssignment());

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}
#endif

	case actDMX: 
		{
			// create here your DMX connections... instead of the dummy....
			CAtmoDmxSerialConnection *tempConnection = new CAtmoDmxSerialConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE)
			{
				pDynData->setAtmoConnection(tempConnection);

				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping(atmoConfig->getChannelAssignment(0));

			CAtmoTools::SetChannelAssignment(pDynData, atmoConfig->getCurrentChannelAssignment());

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}

#if !defined(_ATMO_VLC_PLUGIN_)
	case actNUL: 
		{
			CAtmoNulConnection *tempConnection = new CAtmoNulConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE) 
			{
				pDynData->setAtmoConnection(tempConnection);
				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping(atmoConfig->getChannelAssignment(0));

			CAtmoTools::SetChannelAssignment(pDynData, atmoConfig->getCurrentChannelAssignment());

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}
#endif

	case actMultiAtmo: 
		{
			CAtmoMultiConnection *tempConnection = new CAtmoMultiConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE) 
			{
				pDynData->setAtmoConnection(tempConnection);
				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping(atmoConfig->getChannelAssignment(0));

			CAtmoTools::SetChannelAssignment(pDynData, atmoConfig->getCurrentChannelAssignment());

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}

#if !defined(_ATMO_VLC_PLUGIN_)
	case actMondolight: 
		{
			CMondolightConnection *tempConnection = new CMondolightConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE)
			{
				pDynData->setAtmoConnection(tempConnection);
				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping(atmoConfig->getChannelAssignment(0));

			CAtmoTools::SetChannelAssignment(pDynData, atmoConfig->getCurrentChannelAssignment());

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}
#endif
	case actMoMoLight: 
		{
			CMoMoConnection *tempConnection = new CMoMoConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE)
			{
				pDynData->setAtmoConnection(tempConnection);
				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping( atmoConfig->getChannelAssignment(0) );

			CAtmoTools::SetChannelAssignment(pDynData, atmoConfig->getCurrentChannelAssignment() );

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}

	case actFnordlicht: 
		{
			CFnordlichtConnection *tempConnection = new CFnordlichtConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE)
			{
				pDynData->setAtmoConnection(tempConnection);
				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping( atmoConfig->getChannelAssignment(0) );

			CAtmoTools::SetChannelAssignment(pDynData, atmoConfig->getCurrentChannelAssignment() );

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}

	case actEnttecDMX: 
		{
			CAtmoEntecSerialConnection *tempConnection = new CAtmoEntecSerialConnection( atmoConfig );
			if(tempConnection->OpenConnection() == ATMO_FALSE) 
			{
				pDynData->setAtmoConnection(tempConnection);
				pDynData->UnLockCriticalSection();
				return ATMO_FALSE;
			}
			pDynData->setAtmoConnection(tempConnection);
			pDynData->ReloadZoneDefinitionBitmaps();

			tempConnection->CreateDefaultMapping( atmoConfig->getChannelAssignment(0) );

			CAtmoTools::SetChannelAssignment(pDynData, atmoConfig->getCurrentChannelAssignment() );

			pDynData->UnLockCriticalSection();
			return ATMO_TRUE;
		}


	default: 
		{
			pDynData->UnLockCriticalSection();
			return ATMO_FALSE;
		}
	}
}

pColorPacket CAtmoTools::WhiteCalibration(CAtmoConfig *pAtmoConfig, pColorPacket ColorPacket)
{
	if (pAtmoConfig->isUseSoftware2WhiteAdj())
	{
		for (int i = 0; i < ColorPacket->numColors; i++) 
		{

			ColorPacket->zone[i].r = (unsigned char)(pAtmoConfig->red_whiteAdj[(int)ColorPacket->zone[i].r]);
			ColorPacket->zone[i].g = (unsigned char)(pAtmoConfig->green_whiteAdj[(int)ColorPacket->zone[i].g]);
			ColorPacket->zone[i].b = (unsigned char)(pAtmoConfig->blue_whiteAdj[(int)ColorPacket->zone[i].b]);

		}
	}
	else
	{
		int w_adj_red   = pAtmoConfig->getWhiteAdjustment_Red();
		int w_adj_green = pAtmoConfig->getWhiteAdjustment_Green();
		int w_adj_blue  = pAtmoConfig->getWhiteAdjustment_Blue();

		for (int i = 0; i < ColorPacket->numColors; i++)  
		{
			ColorPacket->zone[i].r = (unsigned char)(((int)w_adj_red   * (int)ColorPacket->zone[i].r) / 255);
			ColorPacket->zone[i].g = (unsigned char)(((int)w_adj_green * (int)ColorPacket->zone[i].g) / 255);
			ColorPacket->zone[i].b = (unsigned char)(((int)w_adj_blue  * (int)ColorPacket->zone[i].b) / 255);
		} 

	}
	return ColorPacket;
}

pColorPacket CAtmoTools::Applyinvert(CAtmoConfig *pAtmoConfig, pColorPacket ColorPacket)
{
	for (int zoneIdx = 0; zoneIdx < ColorPacket->numColors; zoneIdx++)  
	{

		ColorPacket->zone[zoneIdx].r   =  255-ColorPacket->zone[zoneIdx].r;
		ColorPacket->zone[zoneIdx].g   =  255-ColorPacket->zone[zoneIdx].g;
		ColorPacket->zone[zoneIdx].b   =  255-ColorPacket->zone[zoneIdx].b;
	}	
	return ColorPacket;
}

pColorPacket CAtmoTools::ApplyColorK(CAtmoConfig *pAtmoConfig, pColorPacket ColorPacket)
{

	for (int zoneIdx = 0; zoneIdx < ColorPacket->numColors; zoneIdx++)  
	{

		ColorPacket->zone[zoneIdx].r   =  trilinear(pAtmoConfig, ColorPacket->zone[zoneIdx].r,ColorPacket->zone[zoneIdx].g, ColorPacket->zone[zoneIdx].b,0);	
		ColorPacket->zone[zoneIdx].g   =  trilinear(pAtmoConfig, ColorPacket->zone[zoneIdx].r,ColorPacket->zone[zoneIdx].g, ColorPacket->zone[zoneIdx].b,1);
		ColorPacket->zone[zoneIdx].b   =  trilinear(pAtmoConfig, ColorPacket->zone[zoneIdx].r,ColorPacket->zone[zoneIdx].g, ColorPacket->zone[zoneIdx].b,2);
	}	
	return ColorPacket;
}


pColorPacket CAtmoTools::ApplySens(CAtmoConfig *pAtmoConfig, pColorPacket ColorPacket)
{

	for (int zoneIdx = 0; zoneIdx < ColorPacket->numColors; zoneIdx++)  
	{

		int sens = pAtmoConfig->getLiveView_Sensitivity();
		if ((ColorPacket->zone[zoneIdx].r < sens ) &&(ColorPacket->zone[zoneIdx].g < sens)&&(ColorPacket->zone[zoneIdx].b < sens))
		{
			ColorPacket->zone[zoneIdx].r   =  0;
			ColorPacket->zone[zoneIdx].g   =  0;
			ColorPacket->zone[zoneIdx].b   =  0;
		}
	}	
	return ColorPacket;
}

int CAtmoTools::trilinear(CAtmoConfig *pAtmoConfig, int x, int y, int z, int col)
{

	double xd = x/256.0f;
	double yd = y/256.0f;
	double zd = z/256.0f;

	double c00 = 0 + pAtmoConfig->ColorCube[1][0][0][col]*xd;
	double c10 = pAtmoConfig->ColorCube[0][1][0][col]*(1-xd) + pAtmoConfig->ColorCube[1][1][0][col]*xd;
	double c01 = pAtmoConfig->ColorCube[0][0][1][col]*(1-xd) + pAtmoConfig->ColorCube[1][0][1][col]*xd;
	double c11 = pAtmoConfig->ColorCube[0][1][1][col]*(1-xd) + pAtmoConfig->ColorCube[1][1][1][col]*xd;

	double c0 = c00*(1-yd) + c10*yd;
	double c1 = c01*(1-yd) + c11*yd;

	return static_cast<int>(c0*(1-zd) + c1*zd + 0.5f);
};


int CAtmoTools::trilinear3dl(CAtmoConfig *pAtmoConfig,int x, int y, int z, int col)
{

	int x0 = (int)x/17;
	int x1 = x0 + 1;
	if (x1 > 16) {x1 = x0;}
	int y0 = (int)y/17;
	int y1 = y0 + 1;
	if (y1 > 16) {y1 = y0;}
	int z0 = (int)z/17;
	int z1 = z0 + 1;
	if (z1 > 16) {z1 = z0;}

	double xd = (x-x0*17)/15.0f;
	double yd = (y-y0*17)/15.0f;
	double zd = (z-z0*17)/15.0f;

	double c00 = pAtmoConfig->little_ColorCube[x0][y0][z0][col]*(1-xd) + pAtmoConfig->little_ColorCube[x1][y0][z0][col]*xd;
	double c10 = pAtmoConfig->little_ColorCube[x0][y1][z0][col]*(1-xd) + pAtmoConfig->little_ColorCube[x1][y1][z0][col]*xd;
	double c01 = pAtmoConfig->little_ColorCube[x0][y0][z1][col]*(1-xd) + pAtmoConfig->little_ColorCube[x1][y0][z1][col]*xd;
	double c11 = pAtmoConfig->little_ColorCube[x0][y1][z1][col]*(1-xd) + pAtmoConfig->little_ColorCube[x1][y1][z1][col]*xd;

	double c0 = c00*(1-yd) + c10*yd;
	double c1 = c01*(1-yd) + c11*yd;

	return static_cast<int>(c0*(1-zd) + c1*zd + 0.5f);
};

pColorPacket CAtmoTools::Apply3dlut(CAtmoConfig *pAtmoConfig, pColorPacket ColorPacket)
{
	for (int zoneIdx = 0; zoneIdx < ColorPacket->numColors; zoneIdx++)  
	{
		ColorPacket->zone[zoneIdx].r   =  trilinear3dl(pAtmoConfig, ColorPacket->zone[zoneIdx].r,ColorPacket->zone[zoneIdx].g, ColorPacket->zone[zoneIdx].b,0);	
		ColorPacket->zone[zoneIdx].g   =  trilinear3dl(pAtmoConfig, ColorPacket->zone[zoneIdx].r,ColorPacket->zone[zoneIdx].g, ColorPacket->zone[zoneIdx].b,1);
		ColorPacket->zone[zoneIdx].b   =  trilinear3dl(pAtmoConfig, ColorPacket->zone[zoneIdx].r,ColorPacket->zone[zoneIdx].g, ColorPacket->zone[zoneIdx].b,2);
	}	
	return ColorPacket;
}

pColorPacket CAtmoTools::ApplyGamma(CAtmoConfig *pAtmoConfig, pColorPacket ColorPacket)
{
	double v;
	switch(pAtmoConfig->getSoftware_gamma_mode()) 
	{
	case agcNone: break;
	case agcPerColor: 
		{
			double GammaRed   = 100.0 / ((double)pAtmoConfig->getSoftware_gamma_red());
			double GammaGreen = 100.0 / ((double)pAtmoConfig->getSoftware_gamma_green());
			double GammaBlue  = 100.0 / ((double)pAtmoConfig->getSoftware_gamma_blue());
			for (int i = 0; i < ColorPacket->numColors; i++)
			{
				v = ColorPacket->zone[i].r;
				v = (pow( v / 255.0f, 1.0f/GammaRed ) * 255.0f);
				ColorPacket->zone[i].r = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].g;
				v = (pow( v / 255.0f, 1.0f/GammaGreen ) * 255.0f);
				ColorPacket->zone[i].g = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].b;
				v = (pow( v / 255.0f, 1.0f/GammaBlue ) * 255.0f);
				ColorPacket->zone[i].b = ATMO_MIN((int)v, 255);
			}
			break;
		}
	case agcGlobal:   
		{
			double Gamma   = 100.0 / ((double)pAtmoConfig->getSoftware_gamma_global());
			for (int i = 0; i < ColorPacket->numColors; i++)
			{
				v = ColorPacket->zone[i].r;
				v = (pow( v / 255.0f, 1.0f/Gamma ) * 255.0f);
				ColorPacket->zone[i].r = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].g;
				v = (pow( v / 255.0f, 1.0f/Gamma ) * 255.0f);
				ColorPacket->zone[i].g = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].b;
				v = (pow( v / 255.0f, 1.0f/Gamma ) * 255.0f);
				ColorPacket->zone[i].b = ATMO_MIN((int)v, 255);
			}
			break;
		}
	}
	return ColorPacket;
}

pColorPacket CAtmoTools::ApplyGamma2(AtmoGammaCorrect Software_gamma_mode, int red, int green, int blue, int global, pColorPacket ColorPacket)
{
	double v;
	switch(Software_gamma_mode)
	{
	case agcNone: break;
	case agcPerColor:
		{
			double GammaRed   = 100.0 / ((double)red);
			double GammaGreen = 100.0 / ((double)green);
			double GammaBlue  = 100.0 / ((double)blue);
			for (int i = 0; i < ColorPacket->numColors; i++)
			{
				v = ColorPacket->zone[i].r;
				v = (pow( v / 255.0f, 1.0f/GammaRed ) * 255.0f);
				ColorPacket->zone[i].r = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].g;
				v = (pow( v / 255.0f, 1.0f/GammaGreen ) * 255.0f);
				ColorPacket->zone[i].g = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].b;
				v = (pow( v / 255.0f, 1.0f/GammaBlue ) * 255.0f);
				ColorPacket->zone[i].b = ATMO_MIN((int)v, 255);
			}
			break;
		}
	case agcGlobal:   
		{
			double Gamma   = 100.0 / ((double)global);
			for (int i = 0; i < ColorPacket->numColors; i++)
			{
				v = ColorPacket->zone[i].r;
				v = (pow( v / 255.0f, 1.0f/Gamma ) * 255.0f);
				ColorPacket->zone[i].r = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].g;
				v = (pow( v / 255.0f, 1.0f/Gamma ) * 255.0f);
				ColorPacket->zone[i].g = ATMO_MIN((int)v, 255);

				v = ColorPacket->zone[i].b;
				v = (pow( v / 255.0f, 1.0f/Gamma ) * 255.0f);
				ColorPacket->zone[i].b = ATMO_MIN((int)v, 255);
			}
			break;
		}
	}
	return ColorPacket;
}

int CAtmoTools::SetChannelAssignment(CAtmoDynData *pDynData, int index)
{
	CAtmoConfig *pAtmoConfig = pDynData->getAtmoConfig();
	CAtmoConnection *pAtmoConnection = pDynData->getAtmoConnection();
	int oldIndex = pAtmoConfig->getCurrentChannelAssignment();

	CAtmoChannelAssignment *ca = pAtmoConfig->getChannelAssignment(index);
	if((ca!=NULL) && (pAtmoConnection!=NULL)) 
	{
		pAtmoConnection->SetChannelAssignment(ca);
		pAtmoConfig->setCurrentChannelAssignment(index);
	}
	return oldIndex;
}


#if !defined(_ATMO_VLC_PLUGIN_)

void CAtmoTools::SaveBitmap(HDC hdc,HBITMAP hBmp,char *fileName) 
{
	BITMAPINFO bmpInfo;
	BITMAPFILEHEADER  bmpFileHeader;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

	GetDIBits(hdc,hBmp,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);
	if(bmpInfo.bmiHeader.biSizeImage<=0)
		bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth * abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;
	void *pBuf = malloc(bmpInfo.bmiHeader.biSizeImage);
	bmpInfo.bmiHeader.biCompression=BI_RGB;

	GetDIBits(hdc,hBmp,0,bmpInfo.bmiHeader.biHeight,pBuf, &bmpInfo, DIB_RGB_COLORS);


	bmpFileHeader.bfReserved1=0;
	bmpFileHeader.bfReserved2=0;
	bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
	bmpFileHeader.bfType = MakeIntelWord('M','B');
	bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);


	FILE *fp = NULL;
	fp = fopen(fileName,"wb");
	fwrite(&bmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);
	fwrite(&bmpInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
	fwrite(pBuf,bmpInfo.bmiHeader.biSizeImage,1,fp);
	fclose(fp);
}



#endif

