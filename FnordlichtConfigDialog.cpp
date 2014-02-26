#include "StdAfx.h"
#include "FnordlichtConfigDialog.h"
#include "resource.h"

CFnordlichtConfigDialog::CFnordlichtConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
     CBasicDialog(hInst, IDD_CFG_FNORDL_DLG, parent)
{
   this->m_pConfig = pConfig;
}

CFnordlichtConfigDialog::~CFnordlichtConfigDialog(void)
{
}


ATMO_BOOL CFnordlichtConfigDialog::InitDialog(WPARAM wParam) {

        m_hCbxComports = getDlgItem(IDC_COMPORT);
        InitDialog_ComPorts( m_hCbxComports );
        int com = m_pConfig->getComport() - 1;
        if(com < 0) com = 0;
        ComboBox_SetCurSel(m_hCbxComports, com);

        m_hEditNumChannels = getDlgItem(IDC_EDT_FNORDL_CH); 
        this->SetEditInt(m_hEditNumChannels, m_pConfig->getFnordlicht_Amount());

        return ATMO_FALSE;
}


ATMO_BOOL CFnordlichtConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
    switch(wmId) {
        case IDOK: {
           int numChannels;
           if(!this->GetEditInt(m_hEditNumChannels,numChannels)) {
               return ATMO_TRUE; 
           }
           if((numChannels < 1) || (numChannels>255)) {
               MessageBox(this->m_hDialog,"Number of channels out of range 1..255","Error",MB_ICONERROR | MB_OK);
               return ATMO_TRUE;
           }
           m_pConfig->setFnordlicht_Amount( numChannels );

           int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));
           m_pConfig->setComport(comportSel + 1);

           EndDialog(this->m_hDialog, wmId);
           break;
        }

        case IDCANCEL: {
           EndDialog(this->m_hDialog, wmId);
           break;
        }

       default:
           return ATMO_FALSE;

    }

    return ATMO_TRUE;
}


