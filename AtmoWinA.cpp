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

#pragma comment (lib, "Comctl32.lib")


int RegistryKeyExists(HKEY mykey, char *path) {
	HKEY keyHandle;
	
	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
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
    if(r != S_OK) {
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
    if(!InitCommonControlsEx(&comCtrlSettings)) {
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
    for(size_t i=(strlen(workdir)-1); i > 1 ; i--) {  /*  c:\*/
        if(workdir[i] == '\\')  {
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
    if(atmoConfig->getLiveView_DisplayNr()>=atmoDisplays->getCount()) {
       MessageBox(0,"Displayseinstellungen prüfen.","Fehler",MB_ICONERROR | MB_OK);
       atmoConfig->setShowConfigDialog(1);
    }

    // "Globales" Objekt was Configuration und Dynamische Parameter beinhaltet - zur weitergabe
    // an Threads Config Dialog etc...
    atmoDynData  = new CAtmoDynData(hInstance, hResInstance, atmoConfig, atmoDisplays);
    atmoDynData->setWorkDir(workdir);
    


    // schnittstelle nur versuchen zu öffnen wenn config auch scheinbar soweit ok.!
    if((atmoConfig->isShowConfigDialog()==0) && (CAtmoTools::RecreateConnection(atmoDynData) == ATMO_FALSE)) {
        if(atmoConfig->getIgnoreConnectionErrorOnStartup()==ATMO_TRUE) 
        {
           atmoConfig->setConnectionType(actNUL);
           CAtmoTools::RecreateConnection(atmoDynData);                       
        } else {
           // ging nicht zu öffnen...
           CAtmoConnection *acon = atmoDynData->getAtmoConnection();
           if(acon)
           {
              DWORD lastError = acon->getLastError();
              if(lastError) {
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
	if(atmoConfig->isShowConfigDialog()!=0) {
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
    if(res != S_OK) {
       MessageBox(0,"RegisterActiveObject failed.","Error",MB_ICONERROR);
    }

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
    if(input != NULL) {
        input->Close();
        delete input;
    }

    CThread *effect = atmoDynData->getEffectThread();
    atmoDynData->setEffectThread(NULL);
    if(effect!=NULL) {
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
    if(atmoConnection != NULL) {
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

	}
  
    if(hMutex) ReleaseMutex        (hMutex);
	return 0;
}

