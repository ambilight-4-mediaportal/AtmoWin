/*
* AtmoWhiteAdjHW.cpp: Dialog for doing the white calibration of the LED Stripes in the hardware
* works only if your controllers are flashed with the most up to data software!
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#include "StdAfx.h"
#include "atmowhiteadjhw.h"
#include "AtmoTools.h"
#include "resource.h"
#include "Language.h"

CAtmoWhiteAdjHW::CAtmoWhiteAdjHW(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData) :
	CBasicDialog(hInst,IDD_WHITEADJ_HW,parent)
{
	this->m_pAtmoDynData = pAtmoDynData;

	m_Hardware_global_gamma      = pAtmoDynData->getAtmoConfig()->getHardware_global_gamma();
	m_Hardware_global_contrast   = pAtmoDynData->getAtmoConfig()->getHardware_global_contrast();

	m_Hardware_contrast_red      = pAtmoDynData->getAtmoConfig()->getHardware_contrast_red();
	m_Hardware_contrast_green    = pAtmoDynData->getAtmoConfig()->getHardware_contrast_green();
	m_Hardware_contrast_blue     = pAtmoDynData->getAtmoConfig()->getHardware_contrast_blue();

	m_Hardware_gamma_red         = pAtmoDynData->getAtmoConfig()->getHardware_gamma_red();
	m_Hardware_gamma_green       = pAtmoDynData->getAtmoConfig()->getHardware_gamma_green();
	m_Hardware_gamma_blue        = pAtmoDynData->getAtmoConfig()->getHardware_gamma_blue();

	m_SaveToEeprom = ATMO_FALSE;

	UpdateHardware();
}

CAtmoWhiteAdjHW::~CAtmoWhiteAdjHW(void)
{
}

void CAtmoWhiteAdjHW::UpdateHardware() 
{
	CAtmoConnection *pAtmoConnection = m_pAtmoDynData->getAtmoConnection();
	pAtmoConnection->HardwareWhiteAdjust(m_Hardware_global_gamma, m_Hardware_global_contrast, 
		m_Hardware_contrast_red, m_Hardware_contrast_green, m_Hardware_contrast_blue, m_Hardware_gamma_red, 
		m_Hardware_gamma_green, m_Hardware_gamma_blue, ATMO_FALSE);
}


ATMO_BOOL CAtmoWhiteAdjHW::InitDialog(WPARAM wParam) 
{
	HWND hwndCtrl;
	CLanguage *Lng = new CLanguage;

	hwndCtrl = getDlgItem(IDC_EDT_GLOBALGAMMA);
	Edit_LimitText(hwndCtrl,3);

	hwndCtrl = getDlgItem(IDC_EDT_BRIGHTN_RED);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = getDlgItem(IDC_EDT_BRIGHTN_GREEN);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = getDlgItem(IDC_EDT_BRIGHTN_BLUE);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = getDlgItem(IDC_EDT_BRIGHTN_GLOBAL);
	Edit_LimitText(hwndCtrl,3);

	hwndCtrl = getDlgItem(IDC_EDT_GAMMA_RED);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = getDlgItem(IDC_EDT_GAMMA_GREEN);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = getDlgItem(IDC_EDT_GAMMA_BLUE);
	Edit_LimitText(hwndCtrl,3);

	SetupSliderControl(IDC_SL_GLOBALGAMMA,0,255,m_Hardware_global_gamma,8);
	SetEditInt(IDC_EDT_GLOBALGAMMA, m_Hardware_global_gamma);

	SetupSliderControl(IDC_SL_BRIGHTN_RED,11,199,m_Hardware_contrast_red,8);
	SetEditInt(IDC_EDT_BRIGHTN_RED, m_Hardware_contrast_red);
	SetupSliderControl(IDC_SL_BRIGHTN_GREEN,11,199,m_Hardware_contrast_green,8);
	SetEditInt(IDC_EDT_BRIGHTN_GREEN, m_Hardware_contrast_green);
	SetupSliderControl(IDC_SL_BRIGHTN_BLUE,11,199,m_Hardware_contrast_blue,8);
	SetEditInt(IDC_EDT_BRIGHTN_BLUE, m_Hardware_contrast_blue);

	SetupSliderControl(IDC_SL_BRIGHTN_GLOBAL,11,100,m_Hardware_global_contrast,4);
	SetEditInt(IDC_EDT_BRIGHTN_GLOBAL, m_Hardware_global_contrast);

	SetupSliderControl(IDC_SL_GAMMA_RED,11,35,m_Hardware_gamma_red,1);
	SetEditDouble(IDC_EDT_GAMMA_RED, (double)m_Hardware_gamma_red / 10.0);

	SetupSliderControl(IDC_SL_GAMMA_GREEN,11,35,m_Hardware_gamma_green,1);
	SetEditDouble(IDC_EDT_GAMMA_GREEN, (double)m_Hardware_gamma_green / 10.0);

	SetupSliderControl(IDC_SL_GAMMA_BLUE,11,35,m_Hardware_gamma_blue,1);
	SetEditDouble(IDC_EDT_GAMMA_BLUE, (double)m_Hardware_gamma_blue / 10.0);

  Lng->szCurrentDir[Lng->GetSpecialFolder(CSIDL_COMMON_APPDATA)];	
	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

  Lng->XMLParse(Lng->szTemp, Lng->sTextCPicker, "WhiteSetupHW"); 

	SendMessage(getDlgItem(IDC_SAVE_TO_EEPROM), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[0]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[1]));
	SendMessage(getDlgItem(IDC_STATIC46), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[2]));
	SendMessage(getDlgItem(IDC_STATIC47), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[3]));	
	SendMessage(getDlgItem(IDC_STATIC48), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[4]));
	SendMessage(getDlgItem(IDC_STATIC49), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[5]));
	SendMessage(getDlgItem(IDC_STATIC50), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[6]));	
	SendMessage(getDlgItem(IDC_STATIC51), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[7]));
	SendMessage(getDlgItem(IDC_STATIC52), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[4]));
	SendMessage(getDlgItem(IDC_STATIC53), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[5]));
	SendMessage(getDlgItem(IDC_STATIC54), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[6]));
	SendMessage(getDlgItem(IDC_STATIC55), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sWhiteSetupHWText[8]));

	return ATMO_FALSE;
}

void CAtmoWhiteAdjHW::HandleHorzScroll(int code,int position,HWND scrollBarHandle)
{
	int dlgItemId = GetDlgCtrlID(scrollBarHandle);

	int pos   = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);

	switch(dlgItemId) 
	{
	case IDC_SL_GLOBALGAMMA:
		m_Hardware_global_gamma = pos;
		SetEditInt(IDC_EDT_GLOBALGAMMA, m_Hardware_global_gamma);
		break;

	case IDC_SL_BRIGHTN_RED:
		m_Hardware_contrast_red = pos;
		SetEditInt(IDC_EDT_BRIGHTN_RED, m_Hardware_contrast_red);
		break;

	case IDC_SL_BRIGHTN_GREEN:
		m_Hardware_contrast_green = pos;
		SetEditInt(IDC_EDT_BRIGHTN_GREEN, m_Hardware_contrast_green);
		break;

	case IDC_SL_BRIGHTN_BLUE:
		m_Hardware_contrast_blue = pos;
		SetEditInt(IDC_EDT_BRIGHTN_BLUE, m_Hardware_contrast_blue);
		break;

	case IDC_SL_BRIGHTN_GLOBAL:
		m_Hardware_global_contrast = pos;
		SetEditInt(IDC_EDT_BRIGHTN_GLOBAL, m_Hardware_global_contrast);
		break;

	case IDC_SL_GAMMA_RED:
		m_Hardware_gamma_red = pos;
		SetEditDouble(IDC_EDT_GAMMA_RED, (double)m_Hardware_gamma_red / 10.0);
		break;

	case IDC_SL_GAMMA_GREEN:
		m_Hardware_gamma_green = pos;
		SetEditDouble(IDC_EDT_GAMMA_GREEN, (double)m_Hardware_gamma_green / 10.0);
		break;

	case IDC_SL_GAMMA_BLUE:
		m_Hardware_gamma_blue = pos;
		SetEditDouble(IDC_EDT_GAMMA_BLUE, (double)m_Hardware_gamma_blue / 10.0);
		break;

	default:
		return;
	}

	UpdateHardware();
}

ATMO_BOOL CAtmoWhiteAdjHW::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	switch(wmId) 
	{
	case IDOK:
		{
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDCANCEL: 
		{
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDC_EDT_GLOBALGAMMA: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				int value;
				if(this->GetEditInt(hControl,value) && (value>=0) && (value<=255))
				{
					m_Hardware_global_gamma = value;
					SendMessage(getDlgItem(IDC_SL_GLOBALGAMMA), TBM_SETPOS, 1, value);
					UpdateHardware();
				} 
				else 
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_BRIGHTN_RED: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				int value; // 11,199
				if(this->GetEditInt(hControl,value) && (value>=11) && (value<=199)) 
				{
					m_Hardware_contrast_red = value;
					SendMessage(getDlgItem(IDC_SL_BRIGHTN_RED), TBM_SETPOS, 1, value);
					UpdateHardware();
				} 
				else 
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_BRIGHTN_GREEN: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				int value; // 11,199
				if(this->GetEditInt(hControl,value) && (value>=11) && (value<=199)) 
				{
					m_Hardware_contrast_green = value;
					SendMessage(getDlgItem(IDC_SL_BRIGHTN_GREEN), TBM_SETPOS, 1, value);
					UpdateHardware();
				} 
				else 
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_BRIGHTN_BLUE:
		{
			if(wmEvent == EN_CHANGE) 
			{
				int value; // 11,199
				if(this->GetEditInt(hControl,value) && (value>=11) && (value<=199)) 
				{
					m_Hardware_contrast_blue = value;
					SendMessage(getDlgItem(IDC_SL_BRIGHTN_BLUE), TBM_SETPOS, 1, value);
					UpdateHardware();
				} 
				else
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_BRIGHTN_GLOBAL:
		{
			if(wmEvent == EN_CHANGE) 
			{
				int value; // 11,199
				if(this->GetEditInt(hControl,value) && (value>=11) && (value<=100))
				{
					m_Hardware_global_contrast = value;
					SendMessage(getDlgItem(IDC_SL_BRIGHTN_GLOBAL), TBM_SETPOS, 1, value);
					UpdateHardware();
				} else {
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_GAMMA_RED: 
		{
			if(wmEvent == EN_CHANGE)
			{
				double value; // 1.1 3.5
				if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f)) 
				{
					int iValue = (int)(value*10.0);
					m_Hardware_gamma_red = iValue;
					SendMessage(getDlgItem(IDC_SL_GAMMA_RED), TBM_SETPOS, 1, iValue);
					UpdateHardware();
				} 
				else 
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_GAMMA_GREEN:
		{
			if(wmEvent == EN_CHANGE) 
			{
				double value; // 1.1 3.5
				if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f))
				{
					int iValue = (int)(value*10);
					m_Hardware_gamma_green = iValue;
					SendMessage(getDlgItem(IDC_SL_GAMMA_GREEN), TBM_SETPOS, 1, iValue);
					UpdateHardware();
				} 
				else 
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_GAMMA_BLUE:
		{
			if(wmEvent == EN_CHANGE)
			{
				double value; // 1.1 3.5
				if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f))
				{
					int iValue = (int)(value*10);
					m_Hardware_gamma_blue = iValue;
					SendMessage(getDlgItem(IDC_SL_GAMMA_BLUE), TBM_SETPOS, 1, iValue);
					UpdateHardware();
				} 
				else 
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_SAVE_TO_EEPROM:
		{
			if(Button_GetCheck(hControl) == BST_CHECKED)
				m_SaveToEeprom = ATMO_TRUE;
			else
				m_SaveToEeprom = ATMO_FALSE;
			break;
		}

	default:
		return ATMO_FALSE;
	}
	return ATMO_TRUE;
}

ATMO_BOOL CAtmoWhiteAdjHW::Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, ATMO_BOOL &saveLaterToHardware) 
{
	CLanguage *Lng = new CLanguage;

	// so kann ColorPicker sein aktuellen Farbwert ausgeben - ohne etwas zu stören...
	CAtmoConnection *pAtmoConnection = pAtmoDynData->getAtmoConnection();
	CAtmoConfig *pAtmoConfig = pAtmoDynData->getAtmoConfig();
	if(!pAtmoConnection || !pAtmoConnection->isOpen())
	{
		MessageBox(parent,Lng->sWhiteSetupHWText[9],Lng->sWhiteSetupHWText[10],MB_ICONERROR | MB_OK);
		return ATMO_FALSE;
	}

	EffectMode backupEffectMode = CAtmoTools::SwitchEffect(pAtmoDynData, emDisabled);

	CAtmoWhiteAdjHW *pAtmoWhiteAdjHW = new CAtmoWhiteAdjHW(hInst, parent, pAtmoDynData);

	ATMO_BOOL result (pAtmoWhiteAdjHW->ShowModal() == IDOK);
	if(result == ATMO_TRUE) 
	{
		saveLaterToHardware  =  pAtmoWhiteAdjHW->m_SaveToEeprom;

		pAtmoConfig->setHardware_global_gamma( pAtmoWhiteAdjHW->m_Hardware_global_gamma);
		pAtmoConfig->setHardware_global_contrast( pAtmoWhiteAdjHW->m_Hardware_global_contrast );

		pAtmoConfig->setHardware_contrast_red( pAtmoWhiteAdjHW->m_Hardware_contrast_red );
		pAtmoConfig->setHardware_contrast_green( pAtmoWhiteAdjHW->m_Hardware_contrast_green );
		pAtmoConfig->setHardware_contrast_blue( pAtmoWhiteAdjHW->m_Hardware_contrast_blue );

		pAtmoConfig->setHardware_gamma_red( pAtmoWhiteAdjHW->m_Hardware_gamma_red );
		pAtmoConfig->setHardware_gamma_green( pAtmoWhiteAdjHW->m_Hardware_gamma_green );
		pAtmoConfig->setHardware_gamma_blue( pAtmoWhiteAdjHW->m_Hardware_gamma_blue );

	} 
	else 
	{
		// altes Setup wieder Temporär in Controller laden!
		// flackert vermutlich furchtbar...aber wenns dann wirkt... ;-)
		pAtmoConnection->HardwareWhiteAdjust(pAtmoConfig->getHardware_global_gamma(), pAtmoConfig->getHardware_global_contrast(),
			pAtmoConfig->getHardware_contrast_red(),pAtmoConfig->getHardware_contrast_green(),pAtmoConfig->getHardware_contrast_blue(),
			pAtmoConfig->getHardware_gamma_red(),pAtmoConfig->getHardware_gamma_green(),pAtmoConfig->getHardware_contrast_blue(),ATMO_FALSE);
	}
	delete pAtmoWhiteAdjHW;


	CAtmoTools::SwitchEffect(pAtmoDynData, backupEffectMode);
	return result;
}


