#include "StdAfx.h"
#include "atmoclassicconfigdialog.h"
#include "resource.h"
#include "commctrl.h"


CAtmoClassicConfigDialog::CAtmoClassicConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
  CBasicDialog(hInst, IDD_CFG_ATMO_DLG, parent)
{
   this->m_pConfig = pConfig;
}

CAtmoClassicConfigDialog::~CAtmoClassicConfigDialog(void)
{
}


ATMO_BOOL CAtmoClassicConfigDialog::InitDialog(WPARAM wParam) {

        m_hCbxComports = getDlgItem(IDC_COMBO1);
        InitDialog_ComPorts( m_hCbxComports );
		
		int com = m_pConfig->getArduComport() - 1;
        if(com < 0) com = 0;
        ComboBox_SetCurSel(m_hCbxComports, com);

		char buf[10];
		HWND control;

		 m_hCbxBaudrate = getDlgItem(IDC_COMBO2);
        ComboBox_AddString(m_hCbxBaudrate, "115200");
        ComboBox_AddString(m_hCbxBaudrate, "230400");
		ComboBox_AddString(m_hCbxBaudrate, "345600");
        ComboBox_SetCurSel(m_hCbxBaudrate, m_pConfig->getArdu_BaudrateIndex());

		control = getDlgItem(IDC_EDIT1);
		Edit_LimitText(control, 3);
		sprintf(buf, "%d", m_pConfig->getAtmoClLeds());
		Edit_SetText(control, buf);

		return ATMO_FALSE;
}


ATMO_BOOL CAtmoClassicConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {

	switch(wmId) {
	case IDOK: {
		int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMBO1));

		HWND control = getDlgItem(IDC_EDIT1);
		char buf[10];
		Edit_GetText(control, buf, sizeof(buf));
		m_pConfig->setAtmoClLeds(atoi(buf));
	
		m_pConfig->setArduComport(comportSel + 1);
		m_pConfig->setArdu_BaudrateIndex( ComboBox_GetCurSel(getDlgItem(IDC_COMBO2)) );

		
		EndDialog(this->m_hDialog, wmId);
		break;
			   }

        case IDCANCEL: {
            // m_pBackupConfig --> wieder herstellen...
           EndDialog(this->m_hDialog, wmId);
           break;
        }

       default:
           return ATMO_FALSE;

    }

    return ATMO_TRUE;
}
