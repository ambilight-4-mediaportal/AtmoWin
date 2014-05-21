#include "StdAfx.h"
#include "AtmoMultiConfigDialog.h"
#include "resource.h"
#include "Language.h"

CAtmoMultiConfigDialog::CAtmoMultiConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
	CBasicDialog(hInst, IDD_CFG_MULTI_DLG, parent)
{
	this->m_pConfig = pConfig;
}

CAtmoMultiConfigDialog::~CAtmoMultiConfigDialog(void)
{
}


ATMO_BOOL CAtmoMultiConfigDialog::InitDialog(WPARAM wParam) 
{
	int com;
	CLanguage *Lng = new CLanguage;

	Lng->szCurrentDir[Lng->SetLngPath()];

	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

	Lng->XMLParse(Lng->szTemp, Lng->sMultiAtmoLightDlgText, "MultiAtmoLightsetup"); 	 

	m_hCbxComports[0] = getDlgItem(IDC_COMPORT_0);
	ComboBox_AddString(m_hCbxComports[0], Lng->sMultiAtmoLightDlgText[7]);
	InitDialog_ComPorts(m_hCbxComports[0]);
	com = m_pConfig->getComport(0);
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports[0], com);

	m_hCbxComports[1] = getDlgItem(IDC_COMPORT_1);
	ComboBox_AddString(m_hCbxComports[1], Lng->sMultiAtmoLightDlgText[7]);
	InitDialog_ComPorts(m_hCbxComports[1]);
	com = m_pConfig->getComport(1);
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports[1], com);

	m_hCbxComports[2] = getDlgItem(IDC_COMPORT_2);
	ComboBox_AddString(m_hCbxComports[2], Lng->sMultiAtmoLightDlgText[7]);
	InitDialog_ComPorts(m_hCbxComports[2]);
	com = m_pConfig->getComport(2);
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports[2], com);

	m_hCbxComports[3] = getDlgItem(IDC_COMPORT_3);
	ComboBox_AddString(m_hCbxComports[3], Lng->sMultiAtmoLightDlgText[7]);
	InitDialog_ComPorts(m_hCbxComports[3]);
	com = m_pConfig->getComport(3);
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports[3], com);   

	SendMessage(this->m_hDialog, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMultiAtmoLightDlgText[0]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMultiAtmoLightDlgText[1]));
	SendMessage(getDlgItem(IDC_STATIC66), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMultiAtmoLightDlgText[2]));
	SendMessage(getDlgItem(IDC_STATIC67), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMultiAtmoLightDlgText[3]));
	SendMessage(getDlgItem(IDC_STATIC68), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMultiAtmoLightDlgText[4]));
	SendMessage(getDlgItem(IDC_STATIC69), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMultiAtmoLightDlgText[5]));
	SendMessage(getDlgItem(IDC_STATIC70), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMultiAtmoLightDlgText[6]));

	return ATMO_FALSE;
}


ATMO_BOOL CAtmoMultiConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	switch(wmId) 
	{
	case IDOK: 
		{
			for(int i = 0; i < 4; i++ )
			{
				int comportSel = ComboBox_GetCurSel(m_hCbxComports[i]);
				if(comportSel == 0)
					m_pConfig->setComport(i, -1 );
				else
					m_pConfig->setComport(i, comportSel );
			}

			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDCANCEL: 
		{
			// m_pBackupConfig --> wieder herstellen...
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	default:
		return ATMO_FALSE;

	}

	return ATMO_TRUE;
}
