#include "StdAfx.h"
#include "FnordlichtConfigDialog.h"
#include "resource.h"
#include "Language.h"

CFnordlichtConfigDialog::CFnordlichtConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
	CBasicDialog(hInst, IDD_CFG_FNORDL_DLG, parent)
{
	this->m_pConfig = pConfig;
}

CFnordlichtConfigDialog::~CFnordlichtConfigDialog(void)
{
}


ATMO_BOOL CFnordlichtConfigDialog::InitDialog(WPARAM wParam) 
{
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

	Lng->XMLParse(Lng->szTemp, Lng->sNordLightSetupText, "FNordLichtsetup");  

	m_hCbxComports = getDlgItem(IDC_COMPORT);
	InitDialog_ComPorts( m_hCbxComports );
	int com = m_pConfig->getComport() - 1;
	if(com < 0) com = 0;
	ComboBox_SetCurSel(m_hCbxComports, com);

	m_hEditNumChannels = getDlgItem(IDC_EDT_FNORDL_CH); 
	this->SetEditInt(m_hEditNumChannels, m_pConfig->getFnordlicht_Amount());

	SendMessage(this->m_hDialog, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sNordLightSetupText[0]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sNordLightSetupText[1]));
	SendMessage(getDlgItem(IDC_STATIC74), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sNordLightSetupText[2]));
	SendMessage(getDlgItem(IDC_STATIC75), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sNordLightSetupText[3]));
	SendMessage(getDlgItem(IDC_STATIC76), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sNordLightSetupText[4]));

	return ATMO_FALSE;
}


ATMO_BOOL CFnordlichtConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	CLanguage *Lng = new CLanguage;

	switch(wmId) 
	{
	case IDOK: 
		{
			int numChannels;
			if(!this->GetEditInt(m_hEditNumChannels,numChannels)) 
			{
				return ATMO_TRUE; 
			}
			if((numChannels < 1) || (numChannels>255)) 
			{
				MessageBox(this->m_hDialog,Lng->sNordLightSetupText[5], Lng->sNordLightSetupText[6],MB_ICONERROR | MB_OK);
				return ATMO_TRUE;
			}
			m_pConfig->setFnordlicht_Amount( numChannels );

			int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));
			m_pConfig->setComport(comportSel + 1);

			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDCANCEL:
		{
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	default:
		return ATMO_FALSE;

	}

	return ATMO_TRUE;
}


