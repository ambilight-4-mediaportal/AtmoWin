#include "StdAfx.h"
#include "resource.h"
#include "atmotools.h"
#include "AtmoLiveSettings.h"

CAtmoLiveSettings::CAtmoLiveSettings(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData) : CBasicDialog(hInst, IDD_EFFECT_LIVE, parent) 
{
   this->m_pDynData = pDynData;
   m_pBackupConfig = new CAtmoConfig();
   m_pBackupConfig->Assign( m_pDynData->getAtmoConfig() );

}

CAtmoLiveSettings::~CAtmoLiveSettings(void)
{
  delete m_pBackupConfig;
}

void CAtmoLiveSettings::LoadDisplayList() {
     CAtmoDisplays *pAtmoDisplays = this->m_pDynData->getAtmoDisplays();
     HWND hwndCtrl;
     hwndCtrl = getDlgItem(IDC_DISPLAYS);

     ComboBox_ResetContent(hwndCtrl);

     for(int i=0;i<pAtmoDisplays->getCount();i++) {
         TAtmoDisplayInfo nfo = pAtmoDisplays->getDisplayInfo(i);
         ComboBox_AddString(hwndCtrl, nfo.infoText);
     }
     ComboBox_SetCurSel(hwndCtrl, m_pDynData->getAtmoConfig()->getLiveView_DisplayNr());
}

ATMO_BOOL CAtmoLiveSettings::InitDialog(WPARAM wParam) 
{
        CAtmoConfig *config = m_pDynData->getAtmoConfig();
        char buffer[256];
        HWND hwndCtrl;

        hwndCtrl = this->getDlgItem(IDC_GDI_CAPURE_RATE);
        sprintf(buffer,"%d",config->getLiveView_GDI_FrameRate());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,2);

        LoadDisplayList();

        hwndCtrl = getDlgItem(IDC_FILTERMODE);
        ComboBox_AddString(hwndCtrl, "No Filter");
        ComboBox_AddString(hwndCtrl, "Combined");
        ComboBox_AddString(hwndCtrl, "Percent");
        ComboBox_SetCurSel(hwndCtrl, (int)config->getLiveViewFilterMode());

        hwndCtrl = getDlgItem(IDC_WIDESCREEN);
        Button_SetCheck(hwndCtrl, config->getLiveView_WidescreenMode());

        SetupSliderControl(IDC_EdgeWeighting,1,30,config->getLiveView_EdgeWeighting(),5);
        SendMessage(getDlgItem(IDC_EdgeWeighting),TBM_SETPAGESIZE,0,2);
        sprintf(buffer,"Edge weighting [%d]",config->getLiveView_EdgeWeighting());
        SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);

        SetupSliderControl(IDC_BrightCorrect,50,300,config->getLiveView_BrightCorrect(),15);
        SendMessage(getDlgItem(IDC_BrightCorrect),TBM_SETPAGESIZE,0,10);
        sprintf(buffer,"Brightness [%d%%]",config->getLiveView_BrightCorrect());
        SetStaticText(IDC_TXT_BRIGHTCORRECT, buffer);

        SetupSliderControl(IDC_DarknessLimit,0,10,config->getLiveView_DarknessLimit(),1);
        SendMessage(getDlgItem(IDC_DarknessLimit),TBM_SETPAGESIZE,0,1);
        sprintf(buffer,"Darkness Limit [%d]",config->getLiveView_DarknessLimit());
        SetStaticText(IDC_TXT_DARKNESSLIMIT, buffer);

        SetupSliderControl(IDC_HueWinSize,0,5,config->getLiveView_HueWinSize(),1);
        SendMessage(getDlgItem(IDC_HueWinSize),TBM_SETPAGESIZE,0,1);
        sprintf(buffer,"Hue windowing [%d]",config->getLiveView_HueWinSize());
        SetStaticText(IDC_TXT_HUEWINSIZE, buffer);

        SetupSliderControl(IDC_SatWinSize,0,5,config->getLiveView_SatWinSize(),1);
        SendMessage(getDlgItem(IDC_SatWinSize),TBM_SETPAGESIZE,0,1);
        sprintf(buffer,"Sat windowing [%d]",config->getLiveView_SatWinSize());
        SetStaticText(IDC_TXT_SATWINSIZE, buffer);
		
		 SetupSliderControl(IDC_Overlap,0,3,config->getLiveView_Overlap(),1);
         SendMessage(getDlgItem(IDC_Overlap),TBM_SETPAGESIZE,0,1);
         sprintf(buffer,"Zone overlap [%d]",config->getLiveView_Overlap());
         SetStaticText(IDC_TXT_OVERLAP, buffer);

        SetupSliderControl(IDC_Filter_MeanLength,300,5000,config->getLiveViewFilter_MeanLength(),0);
        SendMessage(getDlgItem(IDC_Filter_MeanLength),TBM_SETPAGESIZE,0,100);
        sprintf(buffer,"Filter length [%d ms]",config->getLiveViewFilter_MeanLength());
        SetStaticText(IDC_TXT_Filter_MeanLength, buffer);

        SetupSliderControl(IDC_Filter_MeanThreshold,1,100,config->getLiveViewFilter_MeanThreshold(),0);
        SendMessage(getDlgItem(IDC_Filter_MeanThreshold),TBM_SETPAGESIZE,0,5);
        sprintf(buffer,"Filter threshold [%d%%]",config->getLiveViewFilter_MeanThreshold());
        SetStaticText(IDC_TXT_Filter_MeanThreshold, buffer);

        SetupSliderControl(IDC_Filter_PercentNew,1,100,config->getLiveViewFilter_PercentNew(),0);
        SendMessage(getDlgItem(IDC_Filter_PercentNew),TBM_SETPAGESIZE,0,5);
        sprintf(buffer,"Filter smoothness [%d%%]",config->getLiveViewFilter_PercentNew());
        SetStaticText(IDC_TXT_Filter_PercentNew, buffer);

        return ATMO_FALSE;
}

void CAtmoLiveSettings::HandleHorzScroll(int code,int position,HWND scrollBarHandle) {
     // slider handling...
    char buffer[100];
    int dlgItemId = GetDlgCtrlID(scrollBarHandle);
    switch(dlgItemId) {
           case IDC_EdgeWeighting: {
                int EdgeWeighting = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Edge weighting [%d]",EdgeWeighting);
                SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_EdgeWeighting(EdgeWeighting);
                break;
           }

           case IDC_BrightCorrect: {
                int BrightCorrect = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Brightness [%d%%]",BrightCorrect);
                SetStaticText(IDC_TXT_BRIGHTCORRECT, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_BrightCorrect(BrightCorrect);
                break;
           }
           case IDC_DarknessLimit: {
                int DarknessLimit = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Darkness Limit [%d]",DarknessLimit);
                SetStaticText(IDC_TXT_DARKNESSLIMIT, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_DarknessLimit(DarknessLimit);
                break;
           }

           case IDC_HueWinSize: {
                int HueWinSize = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Hue windowing [%d]",HueWinSize);
                SetStaticText(IDC_TXT_HUEWINSIZE, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_HueWinSize(HueWinSize);
                break;
           }

           case IDC_SatWinSize: {
                int SatWinSize = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Sat windowing [%d]",SatWinSize);
                SetStaticText(IDC_TXT_SATWINSIZE, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_SatWinSize(SatWinSize);
                break;
           }

		   case IDC_Overlap: {
                int Overlap = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Zone overlap [%d]",Overlap);
                SetStaticText(IDC_TXT_OVERLAP, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_Overlap(Overlap);
                break;
           }
		   
           case IDC_Filter_MeanLength: {
                int Filter_MeanLength = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Filter length [%d ms]",Filter_MeanLength);
                SetStaticText(IDC_TXT_Filter_MeanLength, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveViewFilter_MeanLength(Filter_MeanLength);
                break;
           }

           case IDC_Filter_MeanThreshold: {
                int Filter_MeanThreshold = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Filter threshold [%d%%]",Filter_MeanThreshold);
                SetStaticText(IDC_TXT_Filter_MeanThreshold, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveViewFilter_MeanThreshold(Filter_MeanThreshold);
                break;
           }

           case IDC_Filter_PercentNew: {
                int Filter_PercentNew = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Filter smoothness [%d%%]",Filter_PercentNew);
                SetStaticText(IDC_TXT_Filter_PercentNew, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveViewFilter_PercentNew(Filter_PercentNew);
                break;
           }

           default:
               return;

    }
}
void CAtmoLiveSettings::HandleVertScroll(int code,int position,HWND scrollBarHandle) {
}

ATMO_BOOL CAtmoLiveSettings::UpdateLiveViewValues(ATMO_BOOL showPreview) {
     HWND hwndCtrl;
     CAtmoDisplays *pAtmoDisplays = this->m_pDynData->getAtmoDisplays();
     CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();

     pAtmoDisplays->ReloadList(); // auf nummer sicher!

     hwndCtrl = getDlgItem(IDC_DISPLAYS);
     int i = ComboBox_GetCurSel(hwndCtrl);
     if(i>=pAtmoDisplays->getCount()) {
         pAtmoConfig->setLiveView_DisplayNr(0);
         LoadDisplayList();
         MessageBox(this->m_hDialog,"Ausgewähltes Display nicht mehr in der Liste ;-)","Fehler",MB_ICONERROR | MB_OK);
         return ATMO_FALSE;
     }
     pAtmoConfig->setLiveView_DisplayNr(i);

     if(showPreview == ATMO_TRUE) {
        CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
        if((pAtmoConnection!=NULL) && pAtmoConnection->isOpen()) {
            CAtmoTools::SwitchEffect(m_pDynData, emLivePicture);
         }
     }
     return ATMO_TRUE;
}


ATMO_BOOL CAtmoLiveSettings::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {

    switch(wmId) {
        case IDOK: {
           CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();

           if(UpdateLiveViewValues(ATMO_FALSE) == ATMO_FALSE) return 0;

           EndDialog(this->m_hDialog, wmId);
           break;
        }

        case IDCANCEL: {
            // m_pBackupConfig --> wieder herstellen...
            CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
            pAtmoConfig->Assign(this->m_pBackupConfig);

            EndDialog(this->m_hDialog, wmId);
            break;
        }

        case IDC_WIDESCREEN: {
            CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
            if(Button_GetCheck(hControl) == BST_CHECKED)
               pAtmoConfig->setLiveView_WidescreenMode(1);
            else
               pAtmoConfig->setLiveView_WidescreenMode(0);
            break;
        }

        case IDC_FILTERMODE: {
             /// CBN_SELCHANGE
            if(wmEvent == CBN_SELCHANGE) {
               CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
               int i = ComboBox_GetCurSel(hControl);
               pAtmoConfig->setLiveViewFilterMode((AtmoFilterMode)i);
            }
            break;
        }

        case IDC_DISPLAYS: {
            if(wmEvent == CBN_SELCHANGE) {
               CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
               int i = ComboBox_GetCurSel(hControl);
               if(i != pAtmoConfig->getLiveView_DisplayNr()) {
                  UpdateLiveViewValues( pAtmoConfig->getEffectMode() == emLivePicture );
               }
            }
            break;
        }

        case IDC_GDI_CAPURE_RATE: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=1) && (value<=50)) {
                        CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                        pAtmoConfig->setLiveView_GDI_FrameRate(value);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

       default:
           return ATMO_FALSE;

    }

    return ATMO_TRUE;
}
