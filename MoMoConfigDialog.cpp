#include "StdAfx.h"
#include "MoMoConfigDialog.h"
#include "resource.h"

CMoMoConfigDialog::CMoMoConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
     CBasicDialog(hInst, IDD_CFG_MOMO_DLG, parent)
{
   this->m_pConfig = pConfig;
}

CMoMoConfigDialog::~CMoMoConfigDialog(void)
{
}


ATMO_BOOL CMoMoConfigDialog::InitDialog(WPARAM wParam) {

        m_hCbxComports = getDlgItem(IDC_COMPORT);
        InitDialog_ComPorts( m_hCbxComports );
        int com = m_pConfig->getComport() - 1;
        if(com < 0) com = 0;
        ComboBox_SetCurSel(m_hCbxComports, com);

        m_hCbxDeviceType = getDlgItem(IDC_CHANNELS); 
        ComboBox_AddString(m_hCbxDeviceType, "3 - Channels");
        ComboBox_AddString(m_hCbxDeviceType, "4 - Channels");
        int channels = m_pConfig->getMoMo_Channels();
        if(channels == 4)
           ComboBox_SetCurSel(m_hCbxDeviceType, 1);
        else
           ComboBox_SetCurSel(m_hCbxDeviceType, 0);

        return ATMO_FALSE;
}


ATMO_BOOL CMoMoConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
    switch(wmId) {
        case IDOK: {
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


