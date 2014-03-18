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
#include "AtmoWinA.h"
#include "AtmoConfig.h"
#include "AtmoRegistryConfig.h"
#include "AtmoConnection.h"
#include "AtmoWinTrayIconHandler.h"
#include "AtmoSettingsDialog.h"
#include "AtmoDynData.h"
#include "AtmoTools.h"
#include "AtmoDisplays.h"
#include "atmoremotecontrolimplex.h"
#include "AtmoComRegistry.h"
#include "AtmoRes.h"
#include <string>
#include <Commctrl.h>
#include "ObjectModel.h"

#pragma comment (lib, "Comctl32.lib")

char pXML2[] = 
	"<configuration>"
	  "<section name='Default'>"
	    "<profiles>-1</profiles>"
	    "<lastprofile>' '</lastprofile>"
	    "<defaultprofile>' '</defaultprofile>"
	    "<ConnectionType>0</ConnectionType>"
	    "<IgnoreConnectionErrorOnStartup>0</IgnoreConnectionErrorOnStartup>"
	    "<EffectMode>0</EffectMode>"
	    "<comport>-1</comport>"
	    "<Arducomport>-1</Arducomport>"
//	    "<comport_1>-1</comport_1>"
//	    "<comport_2>-1</comport_2>"
//	    "<comport_3>-1</comport_3>" 
	    "<WhiteAdjustment_Red>255</WhiteAdjustment_Red>"
	    "<WhiteAdjustment_Green>255</WhiteAdjustment_Green>"
	    "<WhiteAdjustment_Blue>255</WhiteAdjustment_Blue>"
	    "<UseSoftwareWhiteAdj>1</UseSoftwareWhiteAdj>"
	    "<UseSoftware2WhiteAdj>1</UseSoftware2WhiteAdj>"
	    "<UseColorK>0</UseColorK>"
	    "<Use3dlut>0</Use3dlut>"
	    "<UsePerChWhiteAdj>False</UsePerChWhiteAdj>"
	    "<ColorChanger_iSteps>50</ColorChanger_iSteps>"
	    "<ColorChanger_iDelay>25</ColorChanger_iDelay>"
	    "<LrColorChanger_iSteps>50</LrColorChanger_iSteps>"
	    "<LrColorChanger_iDelay>26</LrColorChanger_iDelay>"
	    "<StaticColor_Red>127</StaticColor_Red>"
	    "<StaticColor_Green>192</StaticColor_Green>"
	    "<StaticColor_Blue>255</StaticColor_Blue>" 
	    "<isSetShutdownColor>1</isSetShutdownColor>"
	    "<ShutdownColor_red>0</ShutdownColor_red>"
	    "<ShutdownColor_green>0</ShutdownColor_green>"
	    "<ShutdownColor_blue>0</ShutdownColor_blue>"
			"<LiveViewFilterMode>1</LiveViewFilterMode>"
	    "<LiveViewFilter_PercentNew>50</LiveViewFilter_PercentNew>"
	    "<LiveViewFilter_MeanLength>300</LiveViewFilter_MeanLength>"
	    "<LiveViewFilter_MeanThreshold>40</LiveViewFilter_MeanThreshold>"
			"<LiveView_EdgeWeighting>8</LiveView_EdgeWeighting>"
	    "<LiveView_RowsPerFrame>1</LiveView_RowsPerFrame>"
	    "<LiveView_BrightCorrect>100</LiveView_BrightCorrect>"
	    "<LiveView_DarknessLimit>5</LiveView_DarknessLimit>"
	    "<LiveView_HueWinSize>3</LiveView_HueWinSize>"
	    "<LiveView_SatWinSize>3</LiveView_SatWinSize>"
	    "<LiveView_Overlap>2</LiveView_Overlap>"
	    "<LiveView_WidescreenMode>0</LiveView_WidescreenMode>"
			"<LiveView_Saturation>100</LiveView_Saturation>"
	    "<LiveView_Sensitivity>0</LiveView_Sensitivity>"
	    "<LiveView_invert>0</LiveView_invert>"
	    "<LiveView_HOverscanBorder>0</LiveView_HOverscanBorder>"
	    "<LiveView_VOverscanBorder>0</LiveView_VOverscanBorder>"
	    "<LiveView_DisplayNr>0</LiveView_DisplayNr>"
	    "<LiveView_FrameDelay>30</LiveView_FrameDelay>"
	    "<LiveView_GDI_FrameRate>25</LiveView_GDI_FrameRate>"
			"<ZonesTopCount>1</ZonesTopCount>"
	    "<ZonesBottomCount>1</ZonesBottomCount>"
	    "<ZonesLRCount>1</ZonesLRCount>"
	    "<ZoneSummary>False</ZoneSummary>"
	    "<Hardware_global_gamma>128</Hardware_global_gamma>"
	    "<Hardware_global_contrast>100</Hardware_global_contrast>"
	    "<Hardware_contrast_red>100</Hardware_contrast_red>"
	    "<Hardware_contrast_green>100</Hardware_contrast_green>"
	    "<Hardware_contrast_blue>100</Hardware_contrast_blue>"
	    "<Hardware_gamma_red>22</Hardware_gamma_red>"
	    "<Hardware_gamma_green>22</Hardware_gamma_green>"
	    "<Hardware_gamma_blue>22</Hardware_gamma_blue>"
	    "<Software_gamma_mode>0</Software_gamma_mode>"
	    "<Software_gamma_red>10</Software_gamma_red>"
	    "<Software_gamma_green>10</Software_gamma_green>"
	    "<Software_gamma_blue>10</Software_gamma_blue>"
	    "<Software_gamma_global>10</Software_gamma_global>"
			"<hAtmoClLeds>32</hAtmoClLeds>"
/*
	    "Software_ColK_rr");
	    "Software_ColK_rg");
    	"Software_ColK_rb");
	    "Software_ColK_gr");
	    "Software_ColK_gg");
	    "Software_ColK_gb");
	    "Software_ColK_br");
    	"Software_ColK_bg");
	    "Software_ColK_bb");

	    "Software_ColK_yr");
	    "Software_ColK_yg");
	    "Software_ColK_yb");
	    "Software_ColK_mr");
	    "Software_ColK_mg");
	    "Software_ColK_mb");
	    "Software_ColK_cr");
	    "Software_ColK_cg");
	    "Software_ColK_cb");

	    "Software_red_g_5");
	    "Software_red_g_10");
	    "Software_red_g_20");
	    "Software_red_g_30");
	    "Software_red_g_40");
     	"Software_red_g_50");
    	"Software_red_g_60");
	    "Software_red_g_70");
	    "Software_red_g_80");
	    "Software_red_g_90");
	    "Software_red_g_100");

	    "Software_green_g_5");
	    "Software_green_g_10");
	    "Software_green_g_20");
	    "Software_green_g_30");
    	"Software_green_g_40");
    	"Software_green_g_50");
	    "Software_green_g_60");
	    "Software_green_g_70");
    	"Software_green_g_80");
	    "Software_green_g_90");
	    "Software_green_g_100");

	    "Software_blue_g_5");
	    "Software_blue_g_10");
	    "Software_blue_g_20");
	    "Software_blue_g_30");
	    "Software_blue_g_40");
	    "Software_blue_g_50");
	    "Software_blue_g_60");
	    "Software_blue_g_70");
	    "Software_blue_g_80");
	    "Software_blue_g_90");
	    "Software_blue_g_100");
*/
	    "<DMX_BaseChannels>NULL</DMX_BaseChannels>"
	    "<DMX_RGB_Channels>5</DMX_RGB_Channels>"
	    "<DMX_BaudrateIndex>0</DMX_BaudrateIndex>"  
	    "<Ardu_BaudrateIndex>0</Ardu_BaudrateIndex>"
	    "<MoMo_Channels>3</MoMo_Channels>"
	    "<Fnordlicht_Amount>2</Fnordlicht_Amount>"
	    "<NumChannelAssignments></NumChannelAssignments>"
	    "<CurrentChannelAssignment>0</CurrentChannelAssignment>"
	  "</section>"
	  "<section name='WhiteAdjPerChannel'>"
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

	HANDLE hMutex =  CreateMutex   (NULL, TRUE, "AtmowinA.AngieMod");
	bool gefunden=FALSE;
	if(GetLastError() == ERROR_ALREADY_EXISTS) gefunden = TRUE;

	if (!gefunden)	
	{

		InitCommonControls();

		if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		{
			MessageBox(0,"timeGetDevCaps failed. Terminate.","Error",MB_OK | MB_ICONERROR);
			return 0;
		}

		// OLE COM System initialisierung? muss in jedem weiteren Thread auch passieren?
		HRESULT r = CoInitialize(NULL);
		if(r != S_OK)
		{
			MessageBox(0,"CoInitialize failed. Terminate.","Error",MB_OK | MB_ICONERROR);
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
			MessageBox(0,"Common Controls Initialization failed. Programm will be terminated.","Error",MB_OK | MB_ICONERROR);
			return 0;
		}

		wTimerRes = min(max(tc.wPeriodMin, 5), tc.wPeriodMax); // 5ms damit wird das sleep präziser:-)
		timeBeginPeriod(wTimerRes); 

		hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ATMOWIN);


		// this Objects load the configuration of atmoWin from registry, and stores its values back to registry!
		CAtmoRegistryConfig *atmoConfig = new CAtmoRegistryConfig("SOFTWARE\\AtmoWinX\\");

		// load configuration from registry...
		if ((RegistryKeyExists(HKEY_CURRENT_USER, "SOFTWARE\\AtmoWinX\\" )==0) && (RegistryKeyExists(HKEY_LOCAL_MACHINE, "SOFTWARE\\AtmoWinX\\" )==1))
		{
			//MessageBox(0,"Loading old registry settings...","info",MB_ICONERROR | MB_OK);
			atmoConfig->LoadSettings(HKEY_LOCAL_MACHINE, "startup");
		}
		else atmoConfig->LoadSettings(HKEY_CURRENT_USER, "startup");

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

		char langDll[MAX_PATH];
		sprintf( langDll , "%sAtmoWin_%s.dll", workdir, atmoConfig->getLanguage() );
		hResInstance = LoadLibraryEx( langDll, 0, LOAD_LIBRARY_AS_DATAFILE );
		if(hResInstance == 0)
			hResInstance = hInstance;
		hAtmo_ResInstance = hResInstance;
		hAtmo_Instance    = hInstance;

		/*
		char zone_bitmap[MAX_PATH];
		for(int i=0; i < CAP_MAX_NUM_ZONES; i++) {
		sprintf(zone_bitmap,"%szone_%d.bmp",workdir, i);
		int i_res = atmoConfig->getZoneDefinition(i)->LoadGradientFromBitmap(zone_bitmap);
		if(i_res != ATMO_LOAD_GRADIENT_OK && i_res!=ATMO_LOAD_GRADIENT_FILENOTFOND)
		{
		MessageBox(0,zone_bitmap,"Failed to load, Check Format, Check Size.",MB_ICONERROR);
		}
		}
		*/

		atmoDisplays = new CAtmoDisplays();
		if(atmoConfig->getLiveView_DisplayNr()>=atmoDisplays->getCount()) 
		{
			MessageBox(0,"Displayseinstellungen prüfen.","Fehler",MB_ICONERROR | MB_OK);
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
						sprintf( lerror,"Open Device LastError 0x%x (%d)", lastError, lastError);
						MessageBox(0, lerror,"Failed to open device",MB_ICONERROR | MB_OK);
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
			MessageBox(0,"RegisterActiveObject failed.","Error",MB_ICONERROR);
		}

		trayIcon->SetRestart(false);

		AllocConsole();
	  freopen("CONIN$", "r",stdin);
	  freopen("CONOUT$", "w",stdout);
	  freopen("CONOUT$", "w",stderr);

  	MyConfiguration O;

  	O.FromXML(pXML2); 
  	O.ToXMLFile("Test.xml");
  	printf(O.ToXML());

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

		// save settings finally to registry... must/should move to config dialog!
		// no longer required is done by setup dialog (in case of IDOK)
		// atmoConfig->SaveToRegistry();
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

