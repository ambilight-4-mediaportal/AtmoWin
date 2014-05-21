#include "StdAfx.h"
#include "MoMoConfigDialog.h"
#include "resource.h"
#include "Language.h"

CMoMoConfigDialog::CMoMoConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
	CBasicDialog(hInst, IDD_CFG_MOMO_DLG, parent)
{
	this->m_pConfig = pConfig;
}

CMoMoConfigDialog::~CMoMoConfigDialog(void)
{
}


ATMO_BOOL CMoMoConfigDialog::InitDialog(WPARAM wParam) 
{
	CLanguage *Lng = new CLanguage;

	Lng->szCurrentDir[Lng->SetLngPath()];

	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

	Lng->XMLParse(Lng->szTemp, Lng->sMomoDlgText, "Momosetup");

	m_hCbxComports = getDlgItem(IDC_COMPORT);
	InitDialog_ComPorts( m_hCbxComports );
	int com = m_pConfig->getComport() - 1;
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports, com);

	m_hCbxDeviceType = getDlgItem(IDC_CHANNELS); 
	ComboBox_AddString(m_hCbxDeviceType, Lng->sMomoDlgText[5]);
	ComboBox_AddString(m_hCbxDeviceType, Lng->sMomoDlgText[6]);
	int channels = m_pConfig->getMoMo_Channels();
	if(channels == 4)
		ComboBox_SetCurSel(m_hCbxDeviceType, 1);
	else
		ComboBox_SetCurSel(m_hCbxDeviceType, 0);

	SendMessage(this->m_hDialog, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMomoDlgText[0]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMomoDlgText[1]));
	SendMessage(getDlgItem(IDC_STATIC71), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMomoDlgText[2]));
	SendMessage(getDlgItem(IDC_STATIC72), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMomoDlgText[3]));
	SendMessage(getDlgItem(IDC_STATIC73), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sMomoDlgText[4]));

	return ATMO_FALSE;
}


ATMO_BOOL CMoMoConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	switch(wmId) 
	{
	case IDOK: 
		{
			// ? 3 oder 4 Kanal ?
			int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));
			m_pConfig->setComport(comportSel + 1);

			int chSel = ComboBox_GetCurSel(getDlgItem(IDC_CHANNELS));
			if(chSel == 0)
				m_pConfig->setMoMo_Channels( 3 ); 
			else     
				m_pConfig->setMoMo_Channels( 4 );

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


