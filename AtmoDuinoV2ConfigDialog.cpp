#include "StdAfx.h"
#include "AtmoDuinoV2ConfigDialog.h"
#include "resource.h"
#include "commctrl.h"
#include "Language.h"

CAtmoDuinoV2ConfigDialog::CAtmoDuinoV2ConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) :
CBasicDialog(hInst, IDD_CFG_ATMODUINOV2_DLG, parent)
{
    this->m_pConfig = pConfig;
}

CAtmoDuinoV2ConfigDialog::~CAtmoDuinoV2ConfigDialog(void)
{
}


ATMO_BOOL CAtmoDuinoV2ConfigDialog::InitDialog(WPARAM wParam) 
{
    m_hCbxComports = getDlgItem(IDC_COMPORT);
    InitDialog_ComPorts(m_hCbxComports);
    int com = m_pConfig->getComport() - 1;
    if (com < 0) com = 0;

    ComboBox_SetCurSel(m_hCbxComports, com);

		m_hCbxBaudrate = getDlgItem(IDC_COMBO2);
		ComboBox_AddString(m_hCbxBaudrate, "115200");
		ComboBox_AddString(m_hCbxBaudrate, "230400");
		ComboBox_AddString(m_hCbxBaudrate, "250000");
		ComboBox_AddString(m_hCbxBaudrate, "500000");
		ComboBox_AddString(m_hCbxBaudrate, "1000000");
		ComboBox_AddString(m_hCbxBaudrate, "2000000");
		ComboBox_AddString(m_hCbxBaudrate, "4000000");
		ComboBox_AddString(m_hCbxBaudrate, "6000000");
		ComboBox_AddString(m_hCbxBaudrate, "8000000");
		ComboBox_AddString(m_hCbxBaudrate, "10000000");
		ComboBox_SetCurSel(m_hCbxBaudrate, m_pConfig->getArduV2_BaudrateIndex());

    char buf[10];
    HWND control;

    control = getDlgItem(IDC_ATMOCL_LED_AMOUNT);
    Edit_LimitText(control, 3);
    sprintf(buf, "%d", m_pConfig->getAtmoV2ClLeds());
    Edit_SetText(control, buf);

    return ATMO_FALSE;
}


ATMO_BOOL CAtmoDuinoV2ConfigDialog::ExecuteCommand(HWND hControl, int wmId, int wmEvent) {

    switch (wmId) 
		{
    case IDOK: 
			{
        int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));

        m_pConfig->setComport(comportSel + 1);
				m_pConfig->setArduV2_BaudrateIndex(ComboBox_GetCurSel(getDlgItem(IDC_COMBO2)));

        HWND control = getDlgItem(IDC_ATMOCL_LED_AMOUNT);
        char buf[10];
        Edit_GetText(control, buf, sizeof(buf));
        m_pConfig->setAtmoV2ClLeds(atoi(buf));

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
