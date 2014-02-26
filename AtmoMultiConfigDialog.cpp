#include "StdAfx.h"
#include "AtmoMultiConfigDialog.h"
#include "resource.h"

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

   m_hCbxComports[0] = getDlgItem(IDC_COMPORT_0);
   ComboBox_AddString(m_hCbxComports[0], "disabled");
   InitDialog_ComPorts(m_hCbxComports[0]);
   com = m_pConfig->getComport(0);
   if(com < 0) com = 0;
   ComboBox_SetCurSel(m_hCbxComports[0], com);

   m_hCbxComports[1] = getDlgItem(IDC_COMPORT_1);
   ComboBox_AddString(m_hCbxComports[1], "disabled");
   InitDialog_ComPorts(m_hCbxComports[1]);
   com = m_pConfig->getComport(1);
   if(com < 0) com = 0;
   ComboBox_SetCurSel(m_hCbxComports[1], com);

   m_hCbxComports[2] = getDlgItem(IDC_COMPORT_2);
   ComboBox_AddString(m_hCbxComports[2], "disabled");
   InitDialog_ComPorts(m_hCbxComports[2]);
   com = m_pConfig->getComport(2);
   if(com < 0) com = 0;
   ComboBox_SetCurSel(m_hCbxComports[2], com);

   m_hCbxComports[3] = getDlgItem(IDC_COMPORT_3);
   ComboBox_AddString(m_hCbxComports[3], "disabled");
   InitDialog_ComPorts(m_hCbxComports[3]);
   com = m_pConfig->getComport(3);
   if(com < 0) com = 0;
   ComboBox_SetCurSel(m_hCbxComports[3], com);

   return ATMO_FALSE;
}


ATMO_BOOL CAtmoMultiConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {

    switch(wmId) {
        case IDOK: {
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
