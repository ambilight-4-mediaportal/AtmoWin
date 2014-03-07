/*
* AtmoWinTrayIconHandler.h: most secret window of atmo win handling the trayicon and the context menu behind it...
*
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#pragma once

#include "AtmoDynData.h"
#include "AtmoConfig.h"
#include "BasicWindow.h"
#include "atlstr.h"

// Nachrichten ID welche vom trayIcon verwendet wird!
#define WM_TRAYMESSAGE  WM_USER

// ID des TrayIcons (naja mehr als eins wirds wohl nicht werden)
#define TrayIconId        1

#define MENUID_QUITATMO    1
#define MENUID_EFFECTS_OFF      2
#define MENUID_EFFECT_STATIC    3
#define MENUID_EFFECT_LIVE      4
#define MENUID_EFFECT_CCHANGER  5
#define MENUID_EFFECT_LRCHANGER 6
#define MENUID_EFFECT_SUBMENU   7
#define MENUID_SETTINGS         8
#define MENUID_INFO             9

#define MENUID_CHANNELMAPPING       10
#define MENUID_FIRST_CHANNELMAPPING 11
#define MENUID_LAST_CHANNELMAPPING  21
#define MENUID_FIRST_LANGUAGE       22
#define MENUID_LAST_LANGUAGE        32
#define MENUID_LANGUAGE_SUBMENU     33

class CTrayIconWindow : public CBasicWindow 
{
private:
	HMENU m_hTrayIconPopupMenu;
	HMENU m_hLanguageSubMenu;
	HMENU m_hModusSubMenu;
	HMENU m_hChannelMappingSubMenu;
	EffectMode m_eEffectModeBeforeSuspend;

  int nCurrentLanguage;
  int nLanguages;
	TCHAR data[1024];
	CAtmoDynData *m_pDynData;

protected: // --siehe CBasicWindow
	LPCTSTR ClassName();
	LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT HandleWmCommand(HWND control, int wmId, int wmEvent);
	void HandleWmDestroy();

private:
	void CreateTrayIconWindow();
	void ShowNotifyIcon(ATMO_BOOL bAdd);

	ATMO_BOOL AddMenuItem(HMENU toMenu,HMENU subMenu,char *caption,ATMO_BOOL enabled,ATMO_BOOL defaultMenu,ATMO_BOOL radioItem,UINT menuID);
	ATMO_BOOL AddMenuSeperator(HMENU toMenu);
	void UpdatePopupMenu();

	void ShowSettingsDialog();

public:
	bool  AppRestart;

	CTrayIconWindow(HINSTANCE hInst,CAtmoDynData *pDynData);
	virtual ~CTrayIconWindow();

	void createWindow();
  void SetRestart(bool parameter); 
  bool GetRestart(); 

};

