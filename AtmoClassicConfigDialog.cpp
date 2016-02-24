#include "StdAfx.h"
#include "atmoclassicconfigdialog.h"
#include "resource.h"
#include "commctrl.h"
#include "Language.h"

CAtmoClassicConfigDialog::CAtmoClassicConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
	CBasicDialog(hInst, IDD_CFG_ATMO_DLG, parent)
{
	this->m_pConfig = pConfig;
}

CAtmoClassicConfigDialog::~CAtmoClassicConfigDialog(void)
{
}


ATMO_BOOL CAtmoClassicConfigDialog::InitDialog(WPARAM wParam) 
{

	m_hCbxComports = getDlgItem(IDC_COMBO_COMPORT);
	InitDialog_ComPorts( m_hCbxComports );

	int com = m_pConfig->getArduComport() - 1;
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports, com);

	char buf[10];
	HWND control;

	m_hCbxBaudrate = getDlgItem(IDC_COMBO2);
	ComboBox_AddString(m_hCbxBaudrate, "115200");
	ComboBox_AddString(m_hCbxBaudrate, "230400");
	ComboBox_AddString(m_hCbxBaudrate, "250000");
	ComboBox_AddString(m_hCbxBaudrate, "345600");
	ComboBox_AddString(m_hCbxBaudrate, "460800");
	ComboBox_AddString(m_hCbxBaudrate, "500000");
	ComboBox_AddString(m_hCbxBaudrate, "576000");
	ComboBox_AddString(m_hCbxBaudrate, "1000000");
	ComboBox_AddString(m_hCbxBaudrate, "2000000");
	ComboBox_AddString(m_hCbxBaudrate, "4000000");
	ComboBox_AddString(m_hCbxBaudrate, "6000000");
	ComboBox_AddString(m_hCbxBaudrate, "8000000");
	ComboBox_AddString(m_hCbxBaudrate, "10000000");
	ComboBox_SetCurSel(m_hCbxBaudrate, m_pConfig->getArdu_BaudrateIndex());

	control = getDlgItem(IDC_EDIT1_led);
	Edit_LimitText(control, 4);
	sprintf(buf, "%d", m_pConfig->getAtmoClLeds());
	Edit_SetText(control, buf);

	CLanguage *Lng = new CLanguage;

	Lng->szCurrentDir[Lng->SetLngPath()];

	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

	Lng->XMLParse(Lng->szTemp, Lng->sAtmoDlgText, "Atmoduinosetup");

	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sAtmoDlgText[0]));
	SendMessage(getDlgItem(IDC_STATIC58), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sAtmoDlgText[1]));
	SendMessage(getDlgItem(IDC_STATIC59), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sAtmoDlgText[2]));
	SendMessage(getDlgItem(IDC_STATIC60), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sAtmoDlgText[3]));
	SendMessage(getDlgItem(IDC_STATIC61), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sAtmoDlgText[4]));

	return ATMO_FALSE;
}


ATMO_BOOL CAtmoClassicConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{

	switch(wmId) 
	{
	case IDOK: 
		{
			int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMBO_COMPORT));

			HWND control = getDlgItem(IDC_EDIT1_led);
			char buf[10];
			Edit_GetText(control, buf, sizeof(buf));
			m_pConfig->setAtmoClLeds(atoi(buf));

			m_pConfig->setArduComport(comportSel + 1);
			m_pConfig->setArdu_BaudrateIndex( ComboBox_GetCurSel(getDlgItem(IDC_COMBO2)) );


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
