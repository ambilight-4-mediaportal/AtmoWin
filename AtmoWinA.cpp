/*
* AtmoWinA.cpp: Main program only for starting...
*
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#include "stdafx.h"
#include <mmsystem.h>
#include "ObjectModel.h"
#include "AtmoWinA.h"
#include "AtmoConfig.h"
#include "AtmoXMLConfig.h"
#include "AtmoConnection.h"
#include "AtmoWinTrayIconHandler.h"
#include "AtmoSettingsDialog.h"
#include "AtmoDynData.h"
#include "AtmoTools.h"
#include "AtmoDisplays.h"
#include "atmoremotecontrolimplex.h"
#include "AtmoComRegistry.h"
#include "AtmoRes.h"
#include "Language.h"
#include <string>
#include <Commctrl.h>

using namespace std;

#pragma comment (lib, "Comctl32.lib")

// Do not Change.. Default configuration
char pXML2[] = 
	"<configuration>"
	"<section name='Default'>"
	"<setting name='Arducomport' value='-1' />"			
	"<setting name='comport' value='1' />"			
	"<setting name='comport_1' value='1' />"			
	"<setting name='comport_2' value='1' />"			
	"<setting name='comport_3' value='1' />"			
	"<setting name='ConnectionType' value='0' />"	
	"<setting name='EffectMode' value='0' />"		
	"<setting name='defaultprofile' value='' />"	
	"<setting name='lastprofile' value='' />"
	"<setting name='profiles' value='' />"
	"</section>"
	"<section name='AtmoWinX'>"
	"<setting name='Ardu_BaudrateIndex' value='0' />"			
	"<setting name='ColorChanger_iDelay' value='25' />"			
	"<setting name='ColorChanger_iSteps' value='50' />"		
	"<setting name='DMX_BaseChannels' value='0' />"	
	"<setting name='DMX_BaudrateIndex' value='0' />"				    
	"<setting name='DMX_RGB_Channels' value='5' />"			
	"<setting name='Fnordlicht_Amount' value='2' />"			
	"<setting name='Hardware_contrast_blue' value='100' />"			
	"<setting name='Hardware_contrast_green' value='100' />"			
	"<setting name='Hardware_contrast_red' value='100' />"			
	"<setting name='Hardware_gamma_blue' value='22' />"			
	"<setting name='Hardware_gamma_green' value='22' />"			
	"<setting name='Hardware_gamma_red' value='22' />"			
	"<setting name='Hardware_global_contrast' value='100' />"				    
	"<setting name='Hardware_global_gamma' value='128' />"			
	"<setting name='hAtmoClLeds' value='32' />"			
	"<setting name='isSetShutdownColor' value='1' />"			
	"<setting name='LiveView_BrightCorrect' value='100' />"			
	"<setting name='LiveView_DarknessLimit' value='5' />"	
	"<setting name='LiveView_DisplayNr' value='0' />"			
	"<setting name='LiveView_EdgeWeighting' value='8' />"		
	"<setting name='LiveView_FrameDelay' value='30' />"			
	"<setting name='LiveView_GDI_FrameRate' value='25' />"	
	"<setting name='LiveView_HOverscanBorder' value='0' />"			
	"<setting name='LiveView_HueWinSize' value='3' />"			
	"<setting name='LiveView_invert' value='0' />"			
	"<setting name='LiveView_Overlap' value='2' />"			
	"<setting name='LiveView_RowsPerFrame' value='1' />"			
	"<setting name='LiveView_Saturation' value='100' />"			
	"<setting name='LiveView_SatWinSize' value='3' />"			
	"<setting name='LiveView_Sensitivity' value='0' />"	
	"<setting name='LiveView_VOverscanBorder' value='0' />"	
	"<setting name='LiveView_WidescreenMode' value='0' />"			
	"<setting name='LiveViewFilter_MeanLength' value='300' />"			
	"<setting name='LiveViewFilter_MeanThreshold' value='40' />"			
	"<setting name='LiveViewFilter_PercentNew' value='50' />"	
	"<setting name='LiveViewFilterMode' value='1' />"			
	"<setting name='LrColorChanger_iDelay' value='25' />"		
	"<setting name='LrColorChanger_iSteps' value='50' />"			
	"<setting name='MoMo_Channels' value='3' />"			
	"<setting name='ShutdownColor_blue' value='0' />"			
	"<setting name='ShutdownColor_green' value='0' />"	
	"<setting name='ShutdownColor_red' value='0' />"			
	"<setting name='Software_blue_g_10' value='25' />"			
	"<setting name='Software_blue_g_100' value='255' />"		    
	"<setting name='Software_blue_g_20' value='51' />"			
	"<setting name='Software_blue_g_30' value='76' />"	
	"<setting name='Software_blue_g_40' value='102' />"			
	"<setting name='Software_blue_g_5' value='12' />"				    
	"<setting name='Software_blue_g_50' value='127' />"			
	"<setting name='Software_blue_g_60' value='153' />"	
	"<setting name='Software_blue_g_70' value='178' />"			
	"<setting name='Software_blue_g_80' value='204' />"			
	"<setting name='Software_blue_g_90' value='229' />"	
	"<setting name='Software_ColK_bb' value='255' />"			
	"<setting name='Software_ColK_bg' value='0' />"			
	"<setting name='Software_ColK_br' value='0' />"			
	"<setting name='Software_ColK_cb' value='255' />"			    
	"<setting name='Software_ColK_cg' value='255' />"			
	"<setting name='Software_ColK_cr' value='0' />"			
	"<setting name='Software_ColK_gb' value='0' />"		
	"<setting name='Software_ColK_gg' value='255' />"			
	"<setting name='Software_ColK_gr' value='0' />"			
	"<setting name='Software_ColK_mb' value='255' />"			
	"<setting name='Software_ColK_mg' value='0' />"			
	"<setting name='Software_ColK_mr' value='255' />"			
	"<setting name='Software_ColK_rb' value='0' />"			
	"<setting name='Software_ColK_rr' value='255' />"			
	"<setting name='Software_ColK_rg' value='0' />"			
	"<setting name='Software_ColK_yb' value='0' />"				    
	"<setting name='Software_ColK_yg' value='255' />"			
	"<setting name='Software_ColK_yr' value='255' />"			
	"<setting name='Software_gamma_blue' value='10' />"			
	"<setting name='Software_gamma_global' value='10' />"					  	   
	"<setting name='Software_gamma_green' value='10' />"				    
	"<setting name='Software_gamma_mode' value='0' />"			
	"<setting name='Software_gamma_red' value='10' />"			
	"<setting name='Software_green_g_10' value='25' />"			
	"<setting name='Software_green_g_100' value='255' />"							    
	"<setting name='Software_green_g_20' value='51' />"			
	"<setting name='Software_green_g_30' value='76' />"			
	"<setting name='Software_green_g_40' value='102' />"			
	"<setting name='Software_green_g_5' value='12' />"			
	"<setting name='Software_green_g_50' value='127' />"			
	"<setting name='Software_green_g_60' value='153' />"			
	"<setting name='Software_green_g_70' value='178' />"			
	"<setting name='Software_green_g_80' value='204' />"			
	"<setting name='Software_green_g_90' value='229' />"			
	"<setting name='Software_red_g_10' value='25' />"			
	"<setting name='Software_red_g_100' value='255' />"			    	   
	"<setting name='Software_red_g_20' value='51' />"		
	"<setting name='Software_red_g_30' value='76' />"			
	"<setting name='Software_red_g_40' value='102' />"			
	"<setting name='Software_red_g_5' value='12' />"				    
	"<setting name='Software_red_g_50' value='127' />"		
	"<setting name='Software_red_g_60' value='153' />"			
	"<setting name='Software_red_g_70' value='178' />"			
	"<setting name='Software_red_g_80' value='204' />"			
	"<setting name='Software_red_g_90' value='229' />"			
	"<setting name='StaticColor_Blue' value='255' />"			
	"<setting name='StaticColor_Green' value='192' />"			
	"<setting name='StaticColor_Red' value='127' />"			
	"<setting name='Use3dlut' value='0' />"			
	"<setting name='UseColorK' value='0' />"			
	"<setting name='UsePerChWhiteAdj' value='0' />"			
	"<setting name='UseSoftware2WhiteAdj' value='1' />"			
	"<setting name='UseSoftwareWhiteAdj' value='1' />"			
	"<setting name='WhiteAdjustment_Blue' value='255' />"		
	"<setting name='WhiteAdjustment_Green' value='255' />"			
	"<setting name='WhiteAdjustment_Red' value='255' />"			
	"<setting name='ZonesBottomCount' value='1' />"			
	"<setting name='ZonesLRCount' value='1' />"			
	"<setting name='ZonesTopCount' value='1' />"			
	"<setting name='ZoneSummary' value='-1' />"						
	"<setting name='IgnoreConnectionErrorOnStartup' value='0' />"			
	"</section>"
	"<section name='WhiteAdjPerChannel'>"
	"<setting name='count' value='0' />"	
	"</section>"
	"</configuration>";

int RegistryKeyExists(HKEY mykey, char *path)
{
	HKEY keyHandle;

	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) 
	{
		RegCloseKey(keyHandle);	
		return 1;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int       nCmdShow)
{
	CAtmoDynData  *atmoDynData = NULL;
	CAtmoDisplays *atmoDisplays = NULL;
	HACCEL hAccelTable;
	HINSTANCE hResInstance;
	MSG msg;
	DWORD activeObjectID;
	TIMECAPS tc;
	UINT wTimerRes;
	CLanguage *Lng = new CLanguage;

	HANDLE hMutex =  CreateMutex   (NULL, TRUE, "AtmowinA.AngieMod");
	bool gefunden=FALSE;
	if(GetLastError() == ERROR_ALREADY_EXISTS) gefunden = TRUE;

	if (!gefunden)	
	{
		InitCommonControls();

		CUtils *Utils = new CUtils;

		// GetSpecialFolder
		Utils->szCurrentDir[Utils->GetSpecialFolder(CSIDL_COMMON_APPDATA)];
		if (!Utils->DirectoryExists(Utils->szCurrentDir ))
		{
			CreateDirectory(Utils->szCurrentDir, NULL);
		}

		// Read Buffer from IniFile
		sprintf(Utils->szTemp, "%s\\%s.xml\0", Utils->szCurrentDir, "AtmoWinX");

		// Create Default Xml if not exists
		ifstream FileExists(Utils->szTemp);
		if (Utils->szTemp != "" && FileExists)
		{
			Utils->strConfigFromFile.FromFile(Utils->szTemp);
			SetProfile(new GProfile((const char *)Utils->strConfigFromFile, Utils->strConfigFromFile.Length(), true));
			Utils->firststart = true;
		}
		else
		{
			MyConfiguration O;

			O.FromXML(pXML2); 
			GString strXMLStreamDestinationBuffer = "<?xml version=\"1.0\" encoding='ISO-8859-1'?>\r\n";
			O.ToXML( &strXMLStreamDestinationBuffer);
			strXMLStreamDestinationBuffer.ToFile(Utils->szTemp);
			Utils->strConfigFromFile.FromFile(Utils->szTemp);
			SetProfile(new GProfile((const char *)Utils->strConfigFromFile, Utils->strConfigFromFile.Length(), true));
			Utils->firststart = true;
		}		

		Lng->XMLParse(Lng->szTemp, Lng->sMessagesText, "Messages");

		if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		{
			MessageBox(0,Lng->sMessagesText[14], Lng->sMessagesText[3],MB_OK | MB_ICONERROR);
			return 0;
		}

		// OLE COM System initialisierung? muss in jedem weiteren Thread auch passieren?
		HRESULT r = CoInitialize(NULL);
		if(r != S_OK)
		{
			MessageBox(0, Lng->sMessagesText[15],Lng->sMessagesText[3],MB_OK | MB_ICONERROR);
			return 0;
		}

		bool forceReg = ((strstr(lpCmdLine,"-register")!=NULL) || (strstr(lpCmdLine,"/register")!=NULL));
		CAtmoComRegistry::SaveComSettings( forceReg );
		if( forceReg )
			return 0;

		INITCOMMONCONTROLSEX comCtrlSettings;
		comCtrlSettings.dwSize = sizeof(INITCOMMONCONTROLSEX);
		comCtrlSettings.dwICC  = ICC_BAR_CLASSES | ICC_WIN95_CLASSES; // reicht das?
		if(!InitCommonControlsEx(&comCtrlSettings))
		{
			MessageBox(0,Lng->sMessagesText[16] + "\n\r" + Lng->sMessagesText[17],Lng->sMessagesText[3],MB_OK | MB_ICONERROR);
			return 0;
		}

		wTimerRes = min(max(tc.wPeriodMin, 5), tc.wPeriodMax); // 5ms damit wird das sleep präziser:-)
		timeBeginPeriod(wTimerRes); 

		hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ATMOWIN);


		// this Objects load the configuration of atmoWin from XML, and stores its values back to XML!
		CAtmoXMLConfig *atmoConfig = new CAtmoXMLConfig("AtmoWinX");

		atmoConfig->LoadSettings("startup");

		// try loading Zone images from Bitmaps in the current folder...
		char workdir[MAX_PATH];
		GetModuleFileName(GetModuleHandle(NULL),workdir,MAX_PATH);
		// strip of everything after last "\"
		for(size_t i=(strlen(workdir)-1); i > 1 ; i--) 
		{  /*  c:\*/
			if(workdir[i] == '\\')  
			{
				workdir[i+1] = 0;
				break;
			}
		}

    hResInstance = hInstance;
    hAtmo_ResInstance = hResInstance;
    hAtmo_Instance    = hInstance;

		atmoDisplays = new CAtmoDisplays();
		if(atmoConfig->getLiveView_DisplayNr()>=atmoDisplays->getCount()) 
		{
			MessageBox(0,Lng->sMessagesText[18], Lng->sMessagesText[3],MB_ICONERROR | MB_OK);
			atmoConfig->setShowConfigDialog(1);
		}

		// "Globales" Objekt was Configuration und Dynamische Parameter beinhaltet - zur weitergabe
		// an Threads Config Dialog etc...
		atmoDynData  = new CAtmoDynData(hInstance, hResInstance, atmoConfig, atmoDisplays);
		atmoDynData->setWorkDir(workdir);



		// schnittstelle nur versuchen zu öffnen wenn config auch scheinbar soweit ok.!
		if((atmoConfig->isShowConfigDialog()==0) && (CAtmoTools::RecreateConnection(atmoDynData) == ATMO_FALSE))
		{
			if(atmoConfig->getIgnoreConnectionErrorOnStartup()==ATMO_TRUE) 
			{
				atmoConfig->setConnectionType(actNUL);
				CAtmoTools::RecreateConnection(atmoDynData);                       
			} 
			else 
			{
				// ging nicht zu öffnen...
				CAtmoConnection *acon = atmoDynData->getAtmoConnection();
				if(acon)
				{
					DWORD lastError = acon->getLastError();
					if(lastError) 
					{
						char lerror[500]; 
						sprintf( lerror,Lng->sMessagesText[19] + "0x%x (%d)", lastError, lastError);
						MessageBox(0, lerror,Lng->sMessagesText[20],MB_ICONERROR | MB_OK);
					}
				}
				atmoConfig->setShowConfigDialog(1);
			}
		}


		// the whole configuration is missing? or one parameter seems be invalid? or hardware connection failed?
		// show config dialog...
		if(atmoConfig->isShowConfigDialog()!=0) 
		{
			// irgend ein Parameter hat in Registry gefehlt oder war nicht
			// so wie erwartet - also sollte man jetzt den Config Dialog anzeigen...?
			CAtmoSettingsDialog *pSetupDlg = new CAtmoSettingsDialog(hResInstance, 0, atmoDynData);
			pSetupDlg->ShowModal();
			delete pSetupDlg;
		}

		// Setup And Launch Effect Thread...
		CAtmoTools::SwitchEffect(atmoDynData,atmoConfig->getEffectMode());

		// Handles the TrayIcon - and his Popup Menu...
		CTrayIconWindow *trayIcon = new CTrayIconWindow( hResInstance, atmoDynData );
		trayIcon->createWindow();

		// COM Server Starten und als aktives Object in ROTT eintragen...
		// damit es jemand finden kann?
		CAtmoRemoteControlImplEx *atmoComRemoteControlex = new CAtmoRemoteControlImplEx(atmoDynData);
		HRESULT res = RegisterActiveObject(atmoComRemoteControlex, CLSID_AtmoRemoteControl, ACTIVEOBJECT_STRONG, &activeObjectID);
		if(res != S_OK) 
		{
			MessageBox(0,Lng->sMessagesText[21], Lng->sMessagesText[3],MB_ICONERROR);
		}

		trayIcon->SetRestart(false);

		// Enter Mainmessage Loop.. infitely - will be stopped by the PostQuitmessage call in AtmoWinTrayIconHandler...
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// COM Server aus Rott entfernen...
		RevokeActiveObject(activeObjectID,NULL);
		// hoffentlich letztes Release durchführen?
		atmoComRemoteControlex->Release(); // sollte RefCount auf 0 bringen und das Objekt mittels delete löschen

		// vermeiden das andere Thread speziell die COM Server nochmal zugriff erlangen während der
		// Shutdown von AtmoWin läuft ... würde nur Problem geben!
		atmoDynData->LockCriticalSection();

		// aktiven Effect Thread ermitteln... und zur Terminierung vorbereiten...
		CAtmoInput *input = atmoDynData->getLiveInput();
		atmoDynData->setLiveInput( NULL );
		if(input != NULL) 
		{
			input->Close();
			delete input;
		}

		CThread *effect = atmoDynData->getEffectThread();
		atmoDynData->setEffectThread(NULL);
		if(effect!=NULL) 
		{
			effect->Terminate();
			delete effect;
		}

		CAtmoPacketQueue *queue = atmoDynData->getLivePacketQueue();
		atmoDynData->setLivePacketQueue( NULL );
		if( queue != NULL )
			delete queue;

		// atmoConnection erneut aus atmoDynData auslesen - da während das Programm lieft die Lokale Variable
		// ungültig geworden sein könnte...
		CAtmoTools::ShowShutdownColor(atmoDynData);

		// Verbindung zum Ausgabe Device Schliessen!
		CAtmoConnection *atmoConnection = atmoDynData->getAtmoConnection();
		atmoDynData->setAtmoConnection(NULL);
		if(atmoConnection != NULL) 
		{
			delete atmoConnection;
		}

		atmoDynData->UnLockCriticalSection();

		CoUninitialize(); // Wait For Last Calls... now incoming Com Calls will fail in case of critical section!


		delete atmoDynData;
		delete atmoConfig;
		delete atmoDisplays;

		timeEndPeriod(wTimerRes); 

		if(hResInstance != hInstance)
			FreeLibrary( hResInstance );

		// AutoRestart
		if (trayIcon->GetRestart())
		{
			char buffer[MAX_PATH];
			GetModuleFileName(NULL, buffer, MAX_PATH );
			ShellExecute(NULL, "open", LPCTSTR(buffer), NULL, NULL, SW_SHOW);	
			return 0;
		}
	}

	if(hMutex) 
		ReleaseMutex (hMutex);
	return 0;

}

