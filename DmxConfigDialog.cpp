#include "StdAfx.h"
#include "dmxconfigdialog.h"
#include "DmxTools.h"
#include "resource.h"
#include "Language.h"

CDmxConfigDialog::CDmxConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
	CBasicDialog(hInst, IDD_CFG_DMX_DLG, parent)
{
	this->m_pConfig = pConfig;

	// m_MaxChannels
	m_MaxChannels = 1;
	if(pConfig->getConnectionType() == actEnttecDMX) 
		m_MaxChannels = 512;

	if(pConfig->getConnectionType() == actDMX) 
		m_MaxChannels = 256;

}

CDmxConfigDialog::~CDmxConfigDialog(void)
{
}


ATMO_BOOL CDmxConfigDialog::InitDialog(WPARAM wParam) 
{
	m_hCbxComports = getDlgItem(IDC_COMPORT);
	InitDialog_ComPorts( m_hCbxComports );

	int com = m_pConfig->getComport() - 1;
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports, com);

	m_hCbxBaudrate = getDlgItem(IDC_BAUDRATE);
	ComboBox_AddString(m_hCbxBaudrate, "115200");
	ComboBox_AddString(m_hCbxBaudrate, "250000");
	ComboBox_SetCurSel(m_hCbxBaudrate, m_pConfig->getDMX_BaudrateIndex());


	char buf[10];

	HWND control;

	control = getDlgItem(IDC_EDT_DMX_BASE);
	Edit_LimitText( control , 256);
	Edit_SetText(control, m_pConfig->getDMX_BaseChannels() );

	control = getDlgItem(IDC_EDT_DMX_RGB);
	Edit_LimitText(control, 3);
	sprintf(buf,"%d",m_pConfig->getDMX_RGB_Channels());
	Edit_SetText(control, buf);

	CLanguage *Lng = new CLanguage;

	TCHAR dest[MAX_PATH];
	Lng->GetThisPath(dest, MAX_PATH);
	CString str = dest;
	str = str + _T("\\Language");
	TCHAR* CurrentPath = NULL;
	CurrentPath = new TCHAR[str.GetLength()+1];
	_tcscpy(CurrentPath, str);	

	sprintf(Lng->szFileINI, "%s\\Language.ini\0", CurrentPath);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", CurrentPath, Lng->szLang);

	Lng->XMLParse(Lng->szTemp, Lng->sDmxDlgText, "Dmxsetup");     

	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sDmxDlgText[0]));
	SendMessage(getDlgItem(IDC_STATIC61), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sDmxDlgText[1]));
	SendMessage(getDlgItem(IDC_STATIC62), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sDmxDlgText[2]));
	SendMessage(getDlgItem(IDC_STATIC63), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sDmxDlgText[3]));
	SendMessage(getDlgItem(IDC_STATIC64), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sDmxDlgText[4]));
	SendMessage(getDlgItem(IDC_STATIC65), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sDmxDlgText[5]));
	SendMessage(this->m_hDialog, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sDmxDlgText[6]));

	return ATMO_FALSE;
}


ATMO_BOOL CDmxConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	HWND control;
	char buf[1000];
	char channels[1000];
	int  rgb;
	int chc;
	CLanguage *Lng = new CLanguage;

	switch(wmId) 
	{
	case IDOK:
		{
			control = getDlgItem(IDC_EDT_DMX_BASE);
			Edit_GetText(control, channels, sizeof(channels));

			control = getDlgItem(IDC_EDT_DMX_RGB);
			Edit_GetText(control, buf, sizeof(buf));
			rgb = atoi(buf);

			if((rgb<1) || (rgb>256))  
			{
				MessageBox(m_hDialog,Lng->sMessagesText[11], Lng->sMessagesText[3],MB_ICONERROR);
				break;
			}

			//chc = IsValidDmxStartString(channels, m_MaxChannels-3);
			//if((chc == -2) || (chc == -3)) {
			//   sprintf(buf,"Ungültiger DMX Kanal gefunden, nicht im Bereich 0..%d",m_MaxChannels-3);
			//   MessageBox(m_hDialog, buf, "Fehler",MB_ICONERROR);
			//   break;
			//}
			//if((chc == -1) || (chc == 0)) {
			//   MessageBox(m_hDialog , "Mindestens ein Startkanal muss angegeben werden!" , "Fehler" , MB_ICONERROR );
			//   break;
			//}
			//if(chc == -4) {
			//   MessageBox(m_hDialog , "Ungültiges Zeichen in der Startkanalliste gefunden.\nEs sind nur Ziffern [0..9], Trenner [,;] und Leerzeichen erlaubt." , "Fehler" , MB_ICONERROR );
			//   break;
			//}

			//if(chc < rgb) {
			//   MessageBox(m_hDialog, "Es wurden weniger Startkanäle angegeben als RGB Kanäle, die fehlende Startkanäle werden auf Basis des letzten Kanals ermittelt.", "Hinweis", MB_ICONINFORMATION);
			//}

			int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));
			m_pConfig->setComport(comportSel + 1);

			m_pConfig->setDMX_BaudrateIndex( ComboBox_GetCurSel(getDlgItem(IDC_BAUDRATE)) );

			m_pConfig->setDMX_BaseChannels( channels );
			m_pConfig->setDMX_RGB_Channels( rgb );

			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDCANCEL: 
		{
			// m_pBackupConfig --> wieder herstellen...
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDC_EDT_DMX_RGB: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value<1) || (value>256)) 
						MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	case IDC_EDT_DMX_BASE: 
		{
			if(wmEvent == EN_CHANGE)
			{
				if(Edit_GetText(hControl,channels,sizeof(channels))>0) 
				{
					chc = IsValidDmxStartString(channels, m_MaxChannels-3);
					if(chc < 0)
						MessageBeep(MB_ICONEXCLAMATION);
				}
			}
			break;
		}

	default:
		return ATMO_FALSE;

	}

	return ATMO_TRUE;
}


