/*
* AtmoWinTrayIconHandler.cpp: most secret window of atmo win handling the trayicon and the context menu behind it...
*
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#include "stdafx.h"

#include "resource.h"
#include "atmosettingsdialog.h"
#include "AtmoWinTrayIconHandler.h"
#include "AtmoDmxSerialConnection.h"
#include "AtmoTools.h"
#include "infodialog.h"
#include "AtmoCtrlMessages.h"
#include "AtmoRes.h"
#include "Language.h"

/*
frei nach ...
http://blogs.msdn.com/oldnewthing/archive/2005/04/22/410773.aspx
*/
// wird der Empfänger aller Nachrichten rund um das TrayIcon
// ist selbst nicht visible - dann brauch ich nicht den Dialog als Fenster
// offen zu halten!

int GetFileList(const char *searchkey, std::vector<std::string> &list)
{
	WIN32_FIND_DATA fd;
	HANDLE h = FindFirstFile(searchkey,&fd);

	if(h == INVALID_HANDLE_VALUE)
	{
		return 0; // no files found
	}
	while(1)
	{
		list.push_back(fd.cFileName);
		if(FindNextFile(h, &fd) == FALSE)
			break;
	}
	return list.size();
}

CTrayIconWindow::CTrayIconWindow(HINSTANCE hInst,CAtmoDynData *pDynData) : CBasicWindow(hInst, (HWND)NULL)
{
	this->m_pDynData = pDynData;
}

CTrayIconWindow::~CTrayIconWindow() 
{
	ShowNotifyIcon(ATMO_FALSE);
}

LPCTSTR CTrayIconWindow::ClassName() 
{
	return TEXT("AtmoTrayIconWinClass");
}

void CTrayIconWindow::createWindow() 
{
	CBasicWindow::createWindow();

	ShowNotifyIcon(ATMO_TRUE);

	CLanguage *Lng = new CLanguage;

	// GetSpecialFolder
	Lng->szCurrentDir[Lng->GetSpecialFolder(CSIDL_COMMON_APPDATA)];
	if (!Lng->DirectoryExists(Lng->szCurrentDir ))
	{
		CreateDirectory(Lng->szCurrentDir, NULL);
	}

	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	this->m_hLanguageSubMenu = CreatePopupMenu();

	// Find available Language Files
	void *hSearch;
	WIN32_FIND_DATA wfd;
	char szFile[MAX_PATH];

	CString strExtension   = _T("\\*.xml");
	strcat(Lng->szCurrentDir, strExtension);

	hSearch = FindFirstFile(Lng->szCurrentDir, &wfd);
	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	nCurrentLanguage = MENUID_FIRST_LANGUAGE;
	AppendMenu(m_hLanguageSubMenu, MF_STRING, MENUID_FIRST_LANGUAGE, "English");
	nLanguages = 1;

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			strcpy(szFile, wfd.cFileName);
			szFile[strlen(szFile) - 4] = 0;
			if (!strcmp(szFile, "English"))
				continue;

			if (!strcmp(szFile, Lng->szLang))
				nCurrentLanguage = MENUID_FIRST_LANGUAGE + nLanguages;
			AppendMenu(m_hLanguageSubMenu, MF_STRING, MENUID_FIRST_LANGUAGE + nLanguages, szFile);
			nLanguages++;
		}
		while (FindNextFile(hSearch, &wfd));
	}
	FindClose(hSearch);

	// Read Buffer from IniFile
	Lng->szCurrentDir[Lng->GetSpecialFolder(CSIDL_COMMON_APPDATA)];
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

	// Create Default Xml Language if not exists
	ifstream FileExists(Lng->szTemp);
	if (Lng->szTemp != "" && FileExists)
	{
		Lng->XMLParse(Lng->szTemp, Lng->sMenuText, "Menu");
	}
	else
	{
		Lng->CreateDefaultXML(Lng->szTemp, sSection);
		Lng->XMLParse(Lng->szTemp, Lng->sMenuText, "Menu");
	}


	this->m_hTrayIconPopupMenu = CreatePopupMenu();
	lstrcpyn(data, Lng->sMenuText[0], 1023);
	AddMenuItem(this->m_hTrayIconPopupMenu, 0, data, ATMO_TRUE, ATMO_TRUE, ATMO_FALSE, MENUID_SETTINGS);

	this->m_hProfileSubMenu = CreatePopupMenu();
	lstrcpyn(data, Lng->sMenuText[11], 1023);
	AddMenuItem(this->m_hTrayIconPopupMenu, this->m_hProfileSubMenu, data, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_PROFILE_SUBMENU);

	lstrcpyn(data, Lng->sMenuText[1], 1023);
	AddMenuItem(this->m_hTrayIconPopupMenu, this->m_hLanguageSubMenu, data, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_LANGUAGE_SUBMENU);

	lstrcpyn(data, Lng->sMenuText[2], 1023);	
	this->m_hModusSubMenu = CreatePopupMenu();
	AddMenuItem(this->m_hTrayIconPopupMenu, this->m_hModusSubMenu, data, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_EFFECT_SUBMENU);

	lstrcpyn(data, Lng->sMenuText[3], 1023);
	AddMenuItem(m_hModusSubMenu, 0, data, ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECTS_OFF);

	lstrcpyn(data, Lng->sMenuText[4], 1023);
	AddMenuItem(m_hModusSubMenu, 0, data,  ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_LIVE);

	lstrcpyn(data, Lng->sMenuText[5], 1023);
	AddMenuItem(m_hModusSubMenu, 0, data, ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_STATIC);

	lstrcpyn(data, Lng->sMenuText[6], 1023);
	AddMenuItem(m_hModusSubMenu, 0, data, ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_CCHANGER);

	lstrcpyn(data, Lng->sMenuText[7], 1023);
	AddMenuItem(m_hModusSubMenu, 0, data,  ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_LRCHANGER);

	lstrcpyn(data, Lng->sMenuText[8], 1023);
	this->m_hChannelMappingSubMenu = CreatePopupMenu();
	AddMenuItem(this->m_hTrayIconPopupMenu, this->m_hChannelMappingSubMenu, data, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_CHANNELMAPPING);

	AddMenuSeperator(this->m_hTrayIconPopupMenu);

	lstrcpyn(data, Lng->sMenuText[9], 1023);
	AddMenuItem(this->m_hTrayIconPopupMenu, 0, data, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_INFO);

	AddMenuSeperator(this->m_hTrayIconPopupMenu);

	lstrcpyn(data, Lng->sMenuText[10], 1023);
	AddMenuItem(this->m_hTrayIconPopupMenu, 0, data, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_QUITATMO);

}

LRESULT CTrayIconWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) 
{
	char *res_buffer1;
	char *res_buffer2;

	switch(message) 
	{
	case WM_TRAYMESSAGE:
		if(wParam == TrayIconId)
		{
			switch(lParam) 
			{
			case WM_RBUTTONDOWN:
				{
					UpdatePopupMenu();
					POINT mouse_cursor_pos;
					GetCursorPos(&mouse_cursor_pos);
					// (Fenster ist zwar nicht sichtbar - aber windows gibt sich damit zufrieden - sonst wird das Menu nicht mehr ausgeblendet?)
					SetForegroundWindow(m_hWindow);
					TrackPopupMenu(m_hTrayIconPopupMenu, TPM_RIGHTALIGN | TPM_LEFTBUTTON, mouse_cursor_pos.x, mouse_cursor_pos.y,  0, m_hWindow, NULL);
					PostMessage(m_hWindow, WM_NULL, 0, 0);
					break;
				}

			case WM_LBUTTONDOWN:
				break;

			case WM_LBUTTONDBLCLK:
				ShowSettingsDialog();
				break;
			}
		}
		break;

	case WM_POWERBROADCAST:
	case WM_POWER:
		{
			// m_eEffectModeBeforeSuspend
			switch(wParam) 
			{
			case PBT_APMSUSPEND:
			case PBT_APMSTANDBY:
			case PWR_SUSPENDREQUEST:
			case PBT_APMQUERYSUSPEND:
				{
					m_eEffectModeBeforeSuspend = CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);
					CAtmoTools::ShowShutdownColor(this->m_pDynData);
					CAtmoConnection *pAtmoConnection = this->m_pDynData->getAtmoConnection();
					if(pAtmoConnection)
						pAtmoConnection->CloseConnection();  
					break;
				}
			case PBT_APMRESUMEAUTOMATIC:
			case PBT_APMRESUMECRITICAL:
			case PBT_APMRESUMESUSPEND:
			case PBT_APMRESUMESTANDBY:
			case PWR_SUSPENDRESUME:
				{
					int i = 0;
					do
					{
						Sleep(2000);
						i++;
					}
					while (!CAtmoTools::RecreateConnection( this->m_pDynData) && (i<30));
					CAtmoTools::SwitchEffect(this->m_pDynData, m_eEffectModeBeforeSuspend);
					break;
				}
			}
			break;
		}

	case WM_DISPLAYCHANGE: 
		{
			CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
      CLanguage *Lng = new CLanguage;

			CAtmoDisplays *pAtmoDisplays = m_pDynData->getAtmoDisplays();
			pAtmoDisplays->ReloadList();

			EffectMode oldEffectMode = pAtmoConfig->getEffectMode();
			if(oldEffectMode == emLivePicture)
			{
				CAtmoTools::SwitchEffect(m_pDynData, emDisabled);
			}

			if(pAtmoConfig->getLiveView_DisplayNr()>=pAtmoDisplays->getCount()) 
			{
				// show Setup.. dialog?
				char *value = 0;
				char *value2 = 0;
				strcpy(value, Lng->sMessagesText[28]);
				res_buffer1 = getResStr( IDS_DISPLAY_SETCHANGED, value);
				strcpy(value2, Lng->sMessagesText[1]);
				res_buffer2 = getResStr( IDS_INFO, value2);

				MessageBox(this->m_hWindow, res_buffer1, res_buffer2 ,MB_ICONWARNING | MB_OK);

				free(res_buffer1);
				free(res_buffer2);

				pAtmoConfig->setLiveView_DisplayNr(0);
				ShowSettingsDialog();
			}

			if((pAtmoConfig->getEffectMode() == emDisabled) && (oldEffectMode == emLivePicture))
			{
				CAtmoTools::SwitchEffect(m_pDynData, oldEffectMode);
			}
			break;
		}

	case WM_QUERYENDSESSION:
		// wenn sich der User abmeldet, oder Windows herunterfährt... das Programm beenden... bevors abgeschossen wird (von Wegen Anwendung reagiert nicht!)
		CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);
		CAtmoTools::ShowShutdownColor(this->m_pDynData);
		DestroyWindow(m_hWindow);
		return ATMO_TRUE;

	case WM_SETEFFECT:
		{
			if((wParam>=0) && (wParam<=4)) 
			{
				EffectMode oldEffectMode = CAtmoTools::SwitchEffect(this->m_pDynData, (EffectMode)wParam);
				return (int)oldEffectMode;
			} else {
				return -1;
			}
		}

	case WM_NEXTEFFECT: 
		{
			EffectMode oldEffectMode = this->m_pDynData->getAtmoConfig()->getEffectMode();
			int i = ((int)oldEffectMode)+1;
			if(i > (int)emLrColorChange) i = (int)emStaticColor;
			CAtmoTools::SwitchEffect(this->m_pDynData, (EffectMode)i);
			return oldEffectMode;
		}
	default:
		return CBasicWindow::HandleMessage(message,wParam,lParam);
	}

	return 0;
}

void CTrayIconWindow::SetRestart(bool parameter) 
{ 
	AppRestart = parameter;
}

bool CTrayIconWindow::GetRestart()
{ 
	return AppRestart; 
}

LRESULT CTrayIconWindow::HandleWmCommand(HWND control, int wmId, int wmEvent)
{
	if((wmId>=MENUID_FIRST_CHANNELMAPPING) && (wmId<=MENUID_LAST_CHANNELMAPPING)) 
	{
		int index = wmId - MENUID_FIRST_CHANNELMAPPING;
		CAtmoTools::SetChannelAssignment(this->m_pDynData, index);
		return 0;
	}

	if((wmId>=MENUID_FIRST_LANGUAGE) && (wmId<=MENUID_LAST_LANGUAGE)) 
	{
		CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
		pAtmoConfig->SaveSettings(pAtmoConfig->lastprofile);
		CAtmoConnection *pAtmoConnection = this->m_pDynData->getAtmoConnection();
		if(pAtmoConnection) pAtmoConnection->CloseConnection(); 

		int index = wmId - MENUID_FIRST_LANGUAGE;
		GetMenuString(this->m_hLanguageSubMenu, index, data, 1023, MF_BYPOSITION);

		CLanguage *Lng = new CLanguage;		
		WritePrivateProfileString("Common", "Language", data, Lng->szFileINI);
		PostMessage(m_hWindow, WM_QUIT, 0, 0); 				
		ShowNotifyIcon(false);
		SetRestart(true);
		return 0;
	}

	if((wmId>=MENUID_FIRST_PROFILES) && (wmId<=MENUID_LAST_PROFILES)) 
	{
		CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
		int index = wmId - MENUID_FIRST_PROFILES;
		GetMenuString(this->m_hProfileSubMenu, index, data, 1023, MF_BYPOSITION);
		pAtmoConfig->lastprofile = data;
		GetProfile().SetConfig("Default", "lastprofile", data);
		string profile1 = GetProfile().GetStringOrDefault("Default", "profiles", "");	
		// only save any if provile available
		if (profile1 != "")
		{
		  pAtmoConfig->LoadSettings(pAtmoConfig->lastprofile);
			EffectMode backupEffectMode = pAtmoConfig->getEffectMode();
			// Effect Thread Stoppen der gerade läuft...
			CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);

			// schnittstelle neu öffnen... könne ja testweise geändert wurden sein?
			CAtmoTools::RecreateConnection(this->m_pDynData);
			// Effect Programm wieder starten...
			CAtmoTools::SwitchEffect(this->m_pDynData, backupEffectMode);
		}

	}

	switch (wmId) 
	{
	case MENUID_QUITATMO:
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			pAtmoConfig->SaveSettings(pAtmoConfig->lastprofile);
			CAtmoConnection *pAtmoConnection = this->m_pDynData->getAtmoConnection();
			if(pAtmoConnection) pAtmoConnection->CloseConnection();  			 
			PostMessage(m_hWindow, WM_QUIT, 0, 0); 		
			ShowNotifyIcon(false);
			//DestroyWindow(m_hWindow);
			break;											 
		}

	case MENUID_EFFECTS_OFF:
		CAtmoTools::SwitchEffect(this->m_pDynData,emDisabled);
		break;

	case MENUID_EFFECT_STATIC:
		CAtmoTools::SwitchEffect(this->m_pDynData,emStaticColor);
		break;

	case MENUID_EFFECT_LIVE:
		CAtmoTools::SwitchEffect(this->m_pDynData,emLivePicture);
		break;

	case MENUID_EFFECT_CCHANGER:
		CAtmoTools::SwitchEffect(this->m_pDynData,emColorChange);
		break;

	case MENUID_EFFECT_LRCHANGER:
		CAtmoTools::SwitchEffect(this->m_pDynData,emLrColorChange);
		break;

	case MENUID_SETTINGS:
		// setup Dialog zeigen ...
		ShowSettingsDialog();
		break;

	case MENUID_INFO: 
		{
			CInfoDialog *dlg = new CInfoDialog(this->m_hInst, this->m_hWindow);
			dlg->ShowModal();
			delete dlg;
			break;
		}

	default:
		return CBasicWindow::HandleWmCommand(control,wmId,wmEvent);
	}
	return 0;
}

void CTrayIconWindow::HandleWmDestroy()
{
}


void CTrayIconWindow::ShowNotifyIcon(ATMO_BOOL bAdd) // Tray Icon Anzeigen/Ausblenden
{
	NOTIFYICONDATA nid;
	ZeroMemory(&nid,sizeof(nid));

	nid.cbSize   =  sizeof(NOTIFYICONDATA);
	nid.hWnd     = m_hWindow;
	nid.uID      = TrayIconId;
	nid.uFlags   = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYMESSAGE;
	nid.hIcon    = LoadIcon(m_hInst,MAKEINTRESOURCE(IDI_ATMOICON));
	lstrcpy(nid.szTip,TEXT("AtmoWinX"));
	if( bAdd == ATMO_TRUE )
		Shell_NotifyIcon(NIM_ADD,&nid);
	else
		Shell_NotifyIcon(NIM_DELETE,&nid);
}

ATMO_BOOL CTrayIconWindow::AddMenuSeperator(HMENU toMenu) 
{
	MENUITEMINFO menuiteminfo;
	menuiteminfo.cbSize        = sizeof(MENUITEMINFO);
	menuiteminfo.fMask         = MIIM_CHECKMARKS | MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU;
	menuiteminfo.fType         = MFT_SEPARATOR;
	menuiteminfo.fState        = MFS_UNCHECKED | MFS_ENABLED;
	menuiteminfo.wID           = 0;
	menuiteminfo.hSubMenu      = 0;
	menuiteminfo.hbmpChecked   = 0;
	menuiteminfo.hbmpUnchecked = 0;
	menuiteminfo.dwTypeData    = NULL;
	return (InsertMenuItem(toMenu,(DWORD)-1,ATMO_TRUE, &menuiteminfo) != 0);
}

ATMO_BOOL CTrayIconWindow::AddMenuItem(HMENU toMenu,HMENU subMenu,char *caption,ATMO_BOOL enabled,ATMO_BOOL defaultMenu,ATMO_BOOL radioItem,UINT menuID) 
{
	MENUITEMINFO menuiteminfo;
	menuiteminfo.cbSize        = sizeof(MENUITEMINFO);
	menuiteminfo.fMask         = MIIM_CHECKMARKS | MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU;

	if(subMenu!=0)
		menuiteminfo.fMask = menuiteminfo.fMask |  MIIM_SUBMENU;

	menuiteminfo.fType         = MFT_STRING;
	if(radioItem)
		menuiteminfo.fType      = menuiteminfo.fType | MFT_RADIOCHECK;


	menuiteminfo.fState        = MFS_UNCHECKED;
	if(defaultMenu)
		menuiteminfo.fState = menuiteminfo.fState | MFS_DEFAULT;
	if(enabled)
		menuiteminfo.fState = menuiteminfo.fState | MFS_ENABLED;
	else
		menuiteminfo.fState = menuiteminfo.fState | MFS_DISABLED;

	menuiteminfo.wID           = menuID; // MENUID_QUITATMO;
	menuiteminfo.hSubMenu      = subMenu;
	menuiteminfo.hbmpChecked   = 0;
	menuiteminfo.hbmpUnchecked = 0;
	menuiteminfo.dwTypeData    = caption;
	return (InsertMenuItem(toMenu,(DWORD)-1,ATMO_TRUE, &menuiteminfo) != 0);
}

void CTrayIconWindow::ShowSettingsDialog() 
{
	static ATMO_BOOL showing = ATMO_TRUE;

	m_pDynData->LockCriticalSection();
	if(showing == ATMO_FALSE) 
	{ 
		m_pDynData->UnLockCriticalSection(); 
		return; 
	}
	showing = ATMO_FALSE;
	m_pDynData->UnLockCriticalSection();

	CAtmoSettingsDialog *pSetupDlg = new CAtmoSettingsDialog(this->m_hInst,this->m_hWindow, this->m_pDynData);

	pSetupDlg->ShowModal();

	delete pSetupDlg;


	m_pDynData->LockCriticalSection();
	showing = ATMO_TRUE;
	m_pDynData->UnLockCriticalSection();
}



void CTrayIconWindow::UpdatePopupMenu()
{
	// Update the Enabled and Radio state of the Popupmenu...
	// before show the menu...
	CAtmoConnection *pAtmoConnection = this->m_pDynData->getAtmoConnection();
	CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
	CLanguage *Lng = new CLanguage;
	if((pAtmoConnection == NULL) || (pAtmoConnection->isOpen()==ATMO_FALSE)) 
	{
		// disable change effect submenu!
		EnableMenuItem(this->m_hTrayIconPopupMenu, MENUID_EFFECT_SUBMENU, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
	} 
	else 
	{
		// enable submenu
		EnableMenuItem(this->m_hTrayIconPopupMenu, MENUID_EFFECT_SUBMENU, MF_BYCOMMAND | MF_ENABLED);
	}

	CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECTS_OFF,      MF_BYCOMMAND |  
		((pAtmoConfig->getEffectMode()==emDisabled) ? MF_CHECKED : MF_UNCHECKED) );
	CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_STATIC,    MF_BYCOMMAND |  
		((pAtmoConfig->getEffectMode()==emStaticColor) ? MF_CHECKED : MF_UNCHECKED) );
	CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_LIVE,      MF_BYCOMMAND | 
		((pAtmoConfig->getEffectMode()==emLivePicture) ? MF_CHECKED : MF_UNCHECKED) );
	CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_CCHANGER,  MF_BYCOMMAND |  
		((pAtmoConfig->getEffectMode()==emColorChange) ? MF_CHECKED : MF_UNCHECKED) );
	CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_LRCHANGER, MF_BYCOMMAND |  
		((pAtmoConfig->getEffectMode()==emLrColorChange) ? MF_CHECKED : MF_UNCHECKED) );

	//Default Language Submenu english
	CheckMenuItem(this->m_hLanguageSubMenu, nCurrentLanguage, MF_CHECKED);

	for(int i=0;i<10;i++)
		if(DeleteMenu(this->m_hChannelMappingSubMenu,MENUID_CHANNELMAPPING+1+i,MF_BYCOMMAND) == ATMO_FALSE) break;

	int currentAssignment = pAtmoConfig->getCurrentChannelAssignment();
	for(int i=0;i<pAtmoConfig->getNumChannelAssignments();i++) 
	{
		CAtmoChannelAssignment *ca = pAtmoConfig->getChannelAssignment(i);
		AddMenuItem(this->m_hChannelMappingSubMenu, 0, ca->getName(), ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_CHANNELMAPPING+1+i);
		if(currentAssignment == i) 
		{
			CheckMenuItem(this->m_hChannelMappingSubMenu,MENUID_CHANNELMAPPING+1+i, MF_BYCOMMAND | MF_CHECKED);
		}
	}

	// Submenu Profiles
	// delete all
	for(int i=0;i<10;i++)
		if(DeleteMenu(this->m_hProfileSubMenu, MENUID_PROFILE_SUBMENU + 1 +i, MF_BYCOMMAND) == ATMO_FALSE) break;
	
	// Get active Profile
	CurrentProfile = GetProfile().GetStringOrDefault("Default", "lastprofile", "");

	//get profiles from XML
	string Profile1 = GetProfile().GetStringOrDefault("Default", "profiles", "");
	char *buffer = new char[Profile1.length()];
	strcpy(buffer, Profile1.c_str());

	//serialize the buffer
	GStringList lst("|", buffer);
	__int64 count = lst.GetCount();
	if (!count == 0)
	{
		for (__int64 i=0; i<count;i++)
		{		
			GString rslt = lst.Serialize("|", __int64(i), 0);
			if (rslt != "")
			{
				char *buffer = new char[rslt._len];
				strcpy(buffer, rslt);

				AddMenuItem(this->m_hProfileSubMenu, 0, buffer, ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, __int64(MENUID_PROFILE_SUBMENU) + 1 + i);
			}
			if(rslt == CurrentProfile) 
			{
				CheckMenuItem(this->m_hProfileSubMenu, __int64(MENUID_PROFILE_SUBMENU) + 1 + i, MF_BYCOMMAND | MF_CHECKED);
			}
		}
	}	
	else
	{
		buffer = new char[strlen(Lng->sMenuText[12])];
	  strcpy(buffer, Lng->sMenuText[12]);
		AddMenuItem(this->m_hProfileSubMenu, 0, buffer, ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_PROFILE_SUBMENU+1);
	}
}