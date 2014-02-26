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
#include <windows.h>

/*
 frei nach ...
 http://blogs.msdn.com/oldnewthing/archive/2005/04/22/410773.aspx
*/
// wird der Empfänger aller Nachrichten rund um das TrayIcon
// ist selbst nicht visible - dann brauch ich nicht den Dialog als Fenster
// offen zu halten!

CTrayIconWindow::CTrayIconWindow(HINSTANCE hInst,CAtmoDynData *pDynData) : CBasicWindow(hInst, (HWND)NULL) {
    this->m_pDynData = pDynData;
}

CTrayIconWindow::~CTrayIconWindow() {
	ShowNotifyIcon(ATMO_FALSE);
}

LPCTSTR CTrayIconWindow::ClassName() {
        return TEXT("AtmoTrayIconWinClass");
}

void CTrayIconWindow::createWindow() {
    CBasicWindow::createWindow();
    char *res_buffer;

	ShowNotifyIcon(ATMO_TRUE);
   	
    this->m_hTrayIconPopupMenu = CreatePopupMenu();

    res_buffer = getResStr(IDS_SETTINGS, "&settings");
    AddMenuItem(this->m_hTrayIconPopupMenu, 0, res_buffer, ATMO_TRUE, ATMO_TRUE, ATMO_FALSE, MENUID_SETTINGS);
    free(res_buffer);

    this->m_hModusSubMenu = CreatePopupMenu();
    res_buffer = getResStr(IDS_MODUS, "mode");
    AddMenuItem(this->m_hTrayIconPopupMenu, this->m_hModusSubMenu, res_buffer, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_EFFECT_SUBMENU);
    free(res_buffer);

    res_buffer = getResStr(IDS_EFFECT_NONE, "no effect");
    AddMenuItem(m_hModusSubMenu, 0, res_buffer,     ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECTS_OFF);
    free(res_buffer);

    res_buffer = getResStr(IDS_EFFECT_LIVE, "Live");
    AddMenuItem(m_hModusSubMenu, 0, res_buffer,        ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_LIVE);
    free(res_buffer);

    res_buffer = getResStr(IDS_EFFECT_STAT_COLOR, "static color");
    AddMenuItem(m_hModusSubMenu, 0, res_buffer, ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_STATIC);
    free(res_buffer);

    res_buffer = getResStr(IDS_EFFECT_COLOR_CHANGE, "color changer");
    AddMenuItem(m_hModusSubMenu, 0, res_buffer,     ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_CCHANGER);
    free(res_buffer);

    res_buffer = getResStr(IDS_EFFECT_COLOR_CHLR, "color changer LR");
    AddMenuItem(m_hModusSubMenu, 0, res_buffer,  ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_EFFECT_LRCHANGER);
    free(res_buffer);

    this->m_hChannelMappingSubMenu = CreatePopupMenu();
    res_buffer = getResStr(IDS_CHANNELASSIGN, "channel mapping");
    AddMenuItem(this->m_hTrayIconPopupMenu, this->m_hChannelMappingSubMenu, res_buffer, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_CHANNELMAPPING);
    free(res_buffer);

    AddMenuSeperator(this->m_hTrayIconPopupMenu);

    res_buffer = getResStr(IDS_INFO, "Info");
    AddMenuItem(this->m_hTrayIconPopupMenu, 0, res_buffer, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_INFO);
    free(res_buffer);

    AddMenuSeperator(this->m_hTrayIconPopupMenu);

    res_buffer = getResStr(IDS_QUIT, "Exit");
    AddMenuItem(this->m_hTrayIconPopupMenu, 0, res_buffer, ATMO_TRUE, ATMO_FALSE, ATMO_FALSE, MENUID_QUITATMO);
    free(res_buffer);
}

LRESULT CTrayIconWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
    char *res_buffer1;
    char *res_buffer2;

    switch(message) {
	    case WM_TRAYMESSAGE:
		   if(wParam == TrayIconId) {
			  switch(lParam) {
		        case WM_RBUTTONDOWN: {
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
                     // MessageBox(0,"WM_LBUTTONDOWN","",0);
					 break;

				case WM_LBUTTONDBLCLK:
                     // MessageBox(0,"WM_LBUTTONDBLCLK","",0);
					 // Show Config Dialog?
                     ShowSettingsDialog();
					 break;
			}
 		}
        break;

        case WM_POWERBROADCAST:
		case WM_POWER:{
            // m_eEffectModeBeforeSuspend
                switch(wParam) {
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

        case WM_DISPLAYCHANGE: {
            CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();

            CAtmoDisplays *pAtmoDisplays = m_pDynData->getAtmoDisplays();
            pAtmoDisplays->ReloadList();

            EffectMode oldEffectMode = pAtmoConfig->getEffectMode();
            if(oldEffectMode == emLivePicture) {
                CAtmoTools::SwitchEffect(m_pDynData, emDisabled);
            }

            if(pAtmoConfig->getLiveView_DisplayNr()>=pAtmoDisplays->getCount()) {
                // show Setup.. dialog?
                res_buffer1 = getResStr( IDS_DISPLAY_SETCHANGED, "Display Einstellungen wurden verändert. Bitte Einstellungen überprüfen.");
                res_buffer2 = getResStr( IDS_INFO, "Info");
                
                MessageBox(this->m_hWindow, res_buffer1, res_buffer2 ,MB_ICONWARNING | MB_OK);
                
                free(res_buffer1);
                free(res_buffer2);

                pAtmoConfig->setLiveView_DisplayNr(0);
                ShowSettingsDialog();
            }

            if((pAtmoConfig->getEffectMode() == emDisabled) && (oldEffectMode == emLivePicture)) {
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

        case WM_SETEFFECT: {
            if((wParam>=0) && (wParam<=4)) {
                EffectMode oldEffectMode = CAtmoTools::SwitchEffect(this->m_pDynData, (EffectMode)wParam);
                return (int)oldEffectMode;
            } else {
                return -1;
            }
        }

        case WM_NEXTEFFECT: {
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

LRESULT CTrayIconWindow::HandleWmCommand(HWND control, int wmId, int wmEvent) {

    if((wmId>=MENUID_FIRST_CHANNELMAPPING) && (wmId<=MENUID_LAST_CHANNELMAPPING)) {
        int index = wmId - MENUID_FIRST_CHANNELMAPPING;
        CAtmoTools::SetChannelAssignment(this->m_pDynData, index);
        return 0;
    }

    switch (wmId) {
		case MENUID_QUITATMO:{
			    CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
				pAtmoConfig->SaveSettings(HKEY_CURRENT_USER, pAtmoConfig->profile);
			    CAtmoConnection *pAtmoConnection = this->m_pDynData->getAtmoConnection();
                if(pAtmoConnection) pAtmoConnection->CloseConnection();  
                DestroyWindow(m_hWindow);
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

        case MENUID_INFO: {
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

void CTrayIconWindow::HandleWmDestroy() {
     PostQuitMessage(0);
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

ATMO_BOOL CTrayIconWindow::AddMenuSeperator(HMENU toMenu) {
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

ATMO_BOOL CTrayIconWindow::AddMenuItem(HMENU toMenu,HMENU subMenu,char *caption,ATMO_BOOL enabled,ATMO_BOOL defaultMenu,ATMO_BOOL radioItem,UINT menuID) {
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

void CTrayIconWindow::ShowSettingsDialog() {
    static ATMO_BOOL showing = ATMO_TRUE;
    
    m_pDynData->LockCriticalSection();
    if(showing == ATMO_FALSE) { m_pDynData->UnLockCriticalSection(); return; }
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
    if((pAtmoConnection == NULL) || (pAtmoConnection->isOpen()==ATMO_FALSE)) {
       // disable change effect submenu!
       EnableMenuItem(this->m_hTrayIconPopupMenu, MENUID_EFFECT_SUBMENU, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
    } else {
      // enable submenu
       EnableMenuItem(this->m_hTrayIconPopupMenu, MENUID_EFFECT_SUBMENU, MF_BYCOMMAND | MF_ENABLED);
    }

    CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECTS_OFF,      MF_BYCOMMAND |  ((pAtmoConfig->getEffectMode()==emDisabled) ? MF_CHECKED : MF_UNCHECKED) );
    CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_STATIC,    MF_BYCOMMAND |  ((pAtmoConfig->getEffectMode()==emStaticColor) ? MF_CHECKED : MF_UNCHECKED) );
    CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_LIVE,      MF_BYCOMMAND |  ((pAtmoConfig->getEffectMode()==emLivePicture) ? MF_CHECKED : MF_UNCHECKED) );
    CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_CCHANGER,  MF_BYCOMMAND |  ((pAtmoConfig->getEffectMode()==emColorChange) ? MF_CHECKED : MF_UNCHECKED) );
    CheckMenuItem(this->m_hModusSubMenu,MENUID_EFFECT_LRCHANGER, MF_BYCOMMAND |  ((pAtmoConfig->getEffectMode()==emLrColorChange) ? MF_CHECKED : MF_UNCHECKED) );

    for(int i=0;i<10;i++)
        if(DeleteMenu(this->m_hChannelMappingSubMenu,MENUID_CHANNELMAPPING+1+i,MF_BYCOMMAND) == ATMO_FALSE) break;

    int currentAssignment = pAtmoConfig->getCurrentChannelAssignment();
    for(int i=0;i<pAtmoConfig->getNumChannelAssignments();i++) {
        CAtmoChannelAssignment *ca = pAtmoConfig->getChannelAssignment(i);
        AddMenuItem(this->m_hChannelMappingSubMenu, 0, ca->getName(), ATMO_TRUE, ATMO_FALSE, ATMO_TRUE, MENUID_CHANNELMAPPING+1+i);
        if(currentAssignment == i) {
           CheckMenuItem(this->m_hChannelMappingSubMenu,MENUID_CHANNELMAPPING+1+i, MF_BYCOMMAND | MF_CHECKED);
        }
    }
}




