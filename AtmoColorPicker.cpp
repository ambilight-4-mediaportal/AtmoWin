/*
* AtmoColorPicker.cpp: Dialog for choosing a color by its components (red, green, blue) - with live preview on the hardware
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/
#include "StdAfx.h"
#include "resource.h"
#include "windowsx.h"
#include "atmocolorpicker.h"
#include "AtmoConfig.h"
#include "AtmoConnection.h"
#include "AtmoTools.h"
#include "commctrl.h"
#include "Language.h"

CAtmoColorPicker::CAtmoColorPicker(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int red,int green,int blue):
	CAtmoCustomColorPicker(hInst,parent,IDD_COLORPICKER,pAtmoDynData,red,green,blue)
{
	this->m_pAtmoDynData = pAtmoDynData;
	outputColor(red,green,blue);
	m_iRed   = red;
	m_iGreen = green;
	m_iBlue  = blue;
}


ATMO_BOOL CAtmoColorPicker::InitDialog(WPARAM wParam) 
{
	CLanguage *Lng = new CLanguage;

	HWND hwndCtrl;
	hwndCtrl = this->getDlgItem(IDC_EDT_RED);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = this->getDlgItem(IDC_EDT_GREEN);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = this->getDlgItem(IDC_EDT_BLUE);
	Edit_LimitText(hwndCtrl,3);

	hwndCtrl = this->getDlgItem(IDC_SL_RED);
	SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
	SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
	SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iRed);
	SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

	hwndCtrl = this->getDlgItem(IDC_SL_GREEN);
	SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
	SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
	SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iGreen);
	SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

	hwndCtrl = this->getDlgItem(IDC_SL_BLUE);
	SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
	SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
	SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iBlue);
	SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

	UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

	Lng->szCurrentDir[Lng->SetLngPath()];

	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

	Lng->XMLParse(Lng->szTemp, Lng->sTextCPicker, "ColorPicker");

	SendMessage(getDlgItem(IDC_STATIC22), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sTextCPicker[0]));
	SendMessage(getDlgItem(IDC_STATIC23), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sTextCPicker[1]));
	SendMessage(getDlgItem(IDC_STATIC24), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sTextCPicker[2]));
	SendMessage(this->m_hDialog, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sTextCPicker[3]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sTextCPicker[4]));

	return ATMO_TRUE;

}


void CAtmoColorPicker::outputColor(int red,int green,int blue) 
{
	CAtmoConnection *pAtmoConnection = this->m_pAtmoDynData->getAtmoConnection();
	CAtmoConfig *pAtmoConfig = this->m_pAtmoDynData->getAtmoConfig();

	if((pAtmoConnection==NULL) || (pAtmoConnection->isOpen()==ATMO_FALSE)) return;

	pColorPacket packet;

	AllocColorPacket(packet, pAtmoConfig->getZoneCount());

	for(int i=0;i<packet->numColors;i++) 
	{
		packet->zone[i].r  = red;
		packet->zone[i].g  = green;
		packet->zone[i].b  = blue;
	}

	if(pAtmoConfig->isUseSoftwareWhiteAdj()) 
		packet = CAtmoTools::WhiteCalibration(pAtmoConfig, packet);

	packet = CAtmoTools::ApplyGamma(pAtmoConfig, packet);

	pAtmoConnection->SendData( packet );

	delete (char *)packet;
}

ATMO_BOOL CAtmoColorPicker::ExecuteCommand(HWND hControl,int wmId, int wmEvent)
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

	case IDC_EDT_RED: 
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0)
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=255)) 
					{
						this->m_iRed = value;
						UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
						outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_EDT_GREEN: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=255))
					{
						this->m_iGreen = value;
						UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
						outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_EDT_BLUE: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=255))
					{
						this->m_iBlue = value;
						UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
						outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	default:
		return ATMO_FALSE;
	}
	return ATMO_TRUE;
}

ATMO_BOOL CAtmoColorPicker::Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int &red, int &green, int &blue) 
{
	// so kann ColorPicker sein aktuellen Farbwert ausgeben - ohne etwas zu stören...
	EffectMode oldEffectMode = CAtmoTools::SwitchEffect(pAtmoDynData, emDisabled);

	CAtmoColorPicker *pAtmoColorPicker = new CAtmoColorPicker(hInst, parent, pAtmoDynData, red, green, blue);

	ATMO_BOOL result (pAtmoColorPicker->ShowModal() == IDOK);
	if(result == ATMO_TRUE) 
	{
		red   = pAtmoColorPicker->m_iRed;
		green = pAtmoColorPicker->m_iGreen;
		blue  = pAtmoColorPicker->m_iBlue;
	}
	delete pAtmoColorPicker;

	CAtmoTools::SwitchEffect(pAtmoDynData, oldEffectMode);

	return result;
}

void CAtmoColorPicker::HandleHorzScroll(int code,int position,HWND scrollBarHandle)
{
	// WM_HSCROLL handler!
	//  WM_HSCROLL
	HWND hwndCtrl;

	int dlgItemId = GetDlgCtrlID(scrollBarHandle);
	switch(dlgItemId) 
	{
	case IDC_SL_RED:

		this->m_iRed   = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
		break;

	case IDC_SL_GREEN:
		this->m_iGreen = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
		break;

	case IDC_SL_BLUE:
		this->m_iBlue  = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
		break;
	default:
		return;

	}
	UpdateColorControls(ATMO_FALSE, ATMO_TRUE);


	// if(IDC_RADIO2){
	//      this->m_iRed = 10;
	//this->m_iGreen = 10;
	//this->m_iBlue = 10;
	// }

	hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
	if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
}


CAtmoColorPicker::~CAtmoColorPicker(void)
{
}