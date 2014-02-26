/*
 * AtmoWhiteSetup.cpp: Dialog for doing the white calibration of the LED Stripes in the software
 * works with all versions of the hardware but consumes some CPU ressources
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#include "StdAfx.h"

#include "resource.h"
#include "atmowhitesetup.h"
#include "atmocustomcolorpicker.h"
#include "atmotools.h"



CAtmoWhiteSetup::CAtmoWhiteSetup(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int red,int green,int blue,ATMO_BOOL useSoftware, ATMO_BOOL useSoftware2, ATMO_BOOL useColorK, ATMO_BOOL use3dlut, ATMO_BOOL useinvert)
                : CAtmoCustomColorPicker(hInst, parent, IDD_WHITEADJ, pAtmoDynData, red, green, blue)
{
    CAtmoConfig *config = pAtmoDynData->getAtmoConfig();

    this->m_UseSoftware = useSoftware;
	this->m_UseSoftware2 = useSoftware2;
	this->m_UseColorK = useColorK;
	this->m_Use3dlut = use3dlut;
	this->m_Useinvert = useinvert;

    m_GammaCorrect = config->getSoftware_gamma_mode();
    m_Global_Gamma = config->getSoftware_gamma_global();
    m_Gamma_Red    = config->getSoftware_gamma_red();
    m_Gamma_Green  = config->getSoftware_gamma_green();
    m_Gamma_Blue   = config->getSoftware_gamma_blue();
}

CAtmoWhiteSetup::~CAtmoWhiteSetup(void)
{
}

ATMO_BOOL CAtmoWhiteSetup::InitDialog(WPARAM wParam) {
     CAtmoCustomColorPicker::InitDialog(wParam);

     CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();

     HWND hwndCtrl;
	 char buffer[10];

     hwndCtrl = this->getDlgItem(IDC_ENABLE);
     if(this->m_UseSoftware)
        Button_SetCheck(hwndCtrl, BST_CHECKED);
     else
        Button_SetCheck(hwndCtrl, BST_UNCHECKED);

	 hwndCtrl = this->getDlgItem(IDC_CHECK2);
     if(this->m_UseSoftware2)
        Button_SetCheck(hwndCtrl, BST_CHECKED);
     else
        Button_SetCheck(hwndCtrl, BST_UNCHECKED);

	 hwndCtrl = this->getDlgItem(IDC_CHECK4);
     if(this->m_UseColorK)
        Button_SetCheck(hwndCtrl, BST_CHECKED);
     else
        Button_SetCheck(hwndCtrl, BST_UNCHECKED);

	 hwndCtrl = this->getDlgItem(IDC_CHECK7);
     if(this->m_Useinvert)
        Button_SetCheck(hwndCtrl, BST_CHECKED);
     else
        Button_SetCheck(hwndCtrl, BST_UNCHECKED);

	hwndCtrl = this->getDlgItem(IDC_CHECK5);
	if(config->m_3dlut)
		EnableWindow(hwndCtrl,1);
	 else
		EnableWindow(hwndCtrl,0);
     if(this->m_Use3dlut)
        Button_SetCheck(hwndCtrl, BST_CHECKED);
     else
        Button_SetCheck(hwndCtrl, BST_UNCHECKED);

	 hwndCtrl = this->getDlgItem(IDC_BUTTON5);
	 if(config->m_3dlut)
		EnableWindow(hwndCtrl,1);
	 else
		EnableWindow(hwndCtrl,0);
	 


     // config->getSoftware_gamma_global();
     // config->getSoftware_gamma_red();
     // config->getSoftware_gamma_green();
     // config->getSoftware_gamma_blue();
     // AtmoGammaCorrect mode = config->getSoftware_gamma_mode();

     // order of items must match enum AtmoGammaCorrect!!
     hwndCtrl = getDlgItem(IDC_SW_GAMMA_MODE);
     ComboBox_AddString(hwndCtrl, "deaktiviert");
     ComboBox_AddString(hwndCtrl, "je Farbanteil");
     ComboBox_AddString(hwndCtrl, "Global");
     ComboBox_SetCurSel(hwndCtrl, int(m_GammaCorrect) );
 
     hwndCtrl = getDlgItem(IDC_EDT_GLOBALGAMMA);
     Edit_LimitText(hwndCtrl,4);
     hwndCtrl = getDlgItem(IDC_EDT_GAMMA_RED);
     Edit_LimitText(hwndCtrl,4);
     hwndCtrl = getDlgItem(IDC_EDT_GAMMA_GREEN);
     Edit_LimitText(hwndCtrl,4);
     hwndCtrl = getDlgItem(IDC_EDT_GAMMA_BLUE);
     Edit_LimitText(hwndCtrl,4);

//calib

	 hwndCtrl = this->getDlgItem(IDC_RADIO11);
     Button_SetCheck(hwndCtrl, BST_CHECKED);

     hwndCtrl = this->getDlgItem(IDC_EDIT1);
        sprintf(buffer,"%d",config->red_grid[0] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT2);
        sprintf(buffer,"%d",config->green_grid[0] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT3);
        sprintf(buffer,"%d",config->blue_grid[0] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT4);
        sprintf(buffer,"%d",config->red_grid[1] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT5);
        sprintf(buffer,"%d",config->green_grid[1] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT6);
        sprintf(buffer,"%d",config->blue_grid[1] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT7);
        sprintf(buffer,"%d",config->red_grid[2] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT8);
        sprintf(buffer,"%d",config->green_grid[2] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT9);
        sprintf(buffer,"%d",config->blue_grid[2] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT10);
        sprintf(buffer,"%d",config->red_grid[3] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT11);
        sprintf(buffer,"%d",config->green_grid[3] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT12);
        sprintf(buffer,"%d",config->blue_grid[3] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT13);
        sprintf(buffer,"%d",config->red_grid[4] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT14);
        sprintf(buffer,"%d",config->green_grid[4] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT15);
        sprintf(buffer,"%d",config->blue_grid[4] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT16);
        sprintf(buffer,"%d",config->red_grid[5] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT17);
        sprintf(buffer,"%d",config->green_grid[5] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT18);
        sprintf(buffer,"%d",config->blue_grid[5] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT19);
        sprintf(buffer,"%d",config->red_grid[6] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT20);
        sprintf(buffer,"%d",config->green_grid[6] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT21);
        sprintf(buffer,"%d",config->blue_grid[6] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT22);
        sprintf(buffer,"%d",config->red_grid[7] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT23);
        sprintf(buffer,"%d",config->green_grid[7] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT24);
        sprintf(buffer,"%d",config->blue_grid[7] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT25);
        sprintf(buffer,"%d",config->red_grid[8] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT26);
        sprintf(buffer,"%d",config->green_grid[8] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT27);
        sprintf(buffer,"%d",config->blue_grid[8] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT28);
        sprintf(buffer,"%d",config->red_grid[9] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT29);
        sprintf(buffer,"%d",config->green_grid[9] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT30);
        sprintf(buffer,"%d",config->blue_grid[9] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT31);
        sprintf(buffer,"%d",config->red_grid[10] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT32);
        sprintf(buffer,"%d",config->green_grid[10] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT33);
        sprintf(buffer,"%d",config->blue_grid[10] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT34);
	    sprintf(buffer,"%d",config->red_ColorK[256][0] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT35);
        sprintf(buffer,"%d",config->red_ColorK[256][1] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT36);
        sprintf(buffer,"%d",config->red_ColorK[256][2] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT37);
	    sprintf(buffer,"%d",config->green_ColorK[256][0] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT38);
        sprintf(buffer,"%d",config->green_ColorK[256][1] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT39);
        sprintf(buffer,"%d",config->green_ColorK[256][2] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT40);
	    sprintf(buffer,"%d",config->blue_ColorK[256][0] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT41);
        sprintf(buffer,"%d",config->blue_ColorK[256][1] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT42);
        sprintf(buffer,"%d",config->blue_ColorK[256][2] );
        Edit_SetText(hwndCtrl,buffer);

	hwndCtrl = this->getDlgItem(IDC_EDIT43);
	    sprintf(buffer,"%d",config->red_ColorK[257][0] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT44);
        sprintf(buffer,"%d",config->red_ColorK[257][1] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT45);
        sprintf(buffer,"%d",config->red_ColorK[257][2] );
        Edit_SetText(hwndCtrl,buffer);

	 hwndCtrl = this->getDlgItem(IDC_EDIT46);
	    sprintf(buffer,"%d",config->green_ColorK[257][0] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT47);
        sprintf(buffer,"%d",config->green_ColorK[257][1] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT48);
        sprintf(buffer,"%d",config->green_ColorK[257][2] );
        Edit_SetText(hwndCtrl,buffer);

	hwndCtrl = this->getDlgItem(IDC_EDIT49);
	    sprintf(buffer,"%d",config->blue_ColorK[257][0] );
        Edit_SetText(hwndCtrl,buffer);
	 hwndCtrl = this->getDlgItem(IDC_EDIT50);
        sprintf(buffer,"%d",config->blue_ColorK[257][1] );
        Edit_SetText(hwndCtrl,buffer);
     hwndCtrl = this->getDlgItem(IDC_EDIT51);
        sprintf(buffer,"%d",config->blue_ColorK[257][2] );
        Edit_SetText(hwndCtrl,buffer);


     SetupSliderControl(IDC_SL_GLOBALGAMMA, 10, 350, m_Global_Gamma, 1);
     SetEditDouble(IDC_EDT_GLOBALGAMMA, (double)m_Global_Gamma / 100.0);

     // 1.1 - bis 3.5
     SetupSliderControl(IDC_SL_GAMMA_RED,10,350,m_Gamma_Red, 1);
     SetEditDouble(IDC_EDT_GAMMA_RED, (double)m_Gamma_Red / 100.0);

     SetupSliderControl(IDC_SL_GAMMA_GREEN,10,350,m_Gamma_Green,1);
     SetEditDouble(IDC_EDT_GAMMA_GREEN, (double)m_Gamma_Green / 100.0);

     SetupSliderControl(IDC_SL_GAMMA_BLUE2,10,350, m_Gamma_Blue , 1);
     SetEditDouble(IDC_EDT_GAMMA_BLUE, (double)m_Gamma_Blue / 100.0);


     SetWindowText(this->m_hDialog,"Weissabgleich"); //?
     return ATMO_TRUE;
}

void CAtmoWhiteSetup::HandleHorzScroll(int code,int position,HWND scrollBarHandle) {
    int dlgItemId = GetDlgCtrlID(scrollBarHandle);
	
	HWND hwndCtrl;
	CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();
	int pos   = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);

    switch(dlgItemId) {
          case IDC_SL_GLOBALGAMMA:
               m_Global_Gamma = pos;
               SetEditDouble(IDC_EDT_GLOBALGAMMA, (double)m_Global_Gamma / 100.0);

			   hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					/*int Red   = max(max((float)m_cRed[0]*this->m_iRed/255.0f,(float)m_cGreen[0]*this->m_iGreen/255.0f),(float)m_cBlue[0]*this->m_iBlue/255.0f);
					int Green =  max(max((float)m_cRed[1]*this->m_iRed/255.0f,(float)m_cGreen[1]*this->m_iGreen/255.0f),(float)m_cBlue[1]*this->m_iBlue/255.0f);
					int Blue  = max(max((float)m_cRed[2]*this->m_iRed/255.0f,(float)m_cGreen[2]*this->m_iGreen/255.0f),(float)m_cBlue[2]*this->m_iBlue/255.0f);*/
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

               break;

          case IDC_SL_GAMMA_RED:
               m_Gamma_Red = pos;
               SetEditDouble(IDC_EDT_GAMMA_RED, (double)m_Gamma_Red / 100.0);

			    hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

               break;

          case IDC_SL_GAMMA_GREEN:
               m_Gamma_Green = pos;
               SetEditDouble(IDC_EDT_GAMMA_GREEN, (double)m_Gamma_Green / 100.0);

			   hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

               break;

          case IDC_SL_GAMMA_BLUE2:
               m_Gamma_Blue = pos;
               SetEditDouble(IDC_EDT_GAMMA_BLUE, (double)m_Gamma_Blue / 100.0);

			   hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

               break;

           default:
               CAtmoCustomColorPicker::HandleHorzScroll(code, position, scrollBarHandle);
               return;
    }
}


ATMO_BOOL CAtmoWhiteSetup::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {
    HWND hwndCtrl;
	CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();
	switch(wmId) {
        //simple whiteadjust
		case IDC_ENABLE: {
            if( Button_GetCheck(hControl) == BST_CHECKED) 
			   {
                m_UseSoftware = ATMO_TRUE;

				m_Use3dlut = ATMO_FALSE;
				m_UseColorK = ATMO_FALSE;
				hwndCtrl = this->getDlgItem(IDC_CHECK4);
				Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_CHECK5);
				Button_SetCheck(hwndCtrl, BST_UNCHECKED);
			   } 
			else 
			{
                m_UseSoftware = ATMO_FALSE;
            }
            break;
        }
		//loaded 3d lut
		case IDC_CHECK5: {
            if( Button_GetCheck(hControl) == BST_CHECKED) 
			{
                m_Use3dlut = ATMO_TRUE;
				m_UseColorK = ATMO_FALSE;
				m_UseSoftware = ATMO_TRUE;
				hwndCtrl = this->getDlgItem(IDC_CHECK4);
				Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_ENABLE);
				Button_SetCheck(hwndCtrl, BST_UNCHECKED);
            } 
			else 
			{
                m_Use3dlut = ATMO_FALSE;
            }
            break;
        }
		//invert
        case IDC_CHECK7: {
            if( Button_GetCheck(hControl) == BST_CHECKED) 
			{
                m_Useinvert = ATMO_TRUE;
	            } 
			else 
			{
                m_Useinvert = ATMO_FALSE;
            }
            break;
        }
		// greyscale
		case IDC_CHECK2: {
            if( Button_GetCheck(hControl) == BST_CHECKED) 
			{
                m_UseSoftware2 = ATMO_TRUE;

            } 
			else {
                m_UseSoftware2 = ATMO_FALSE;

				hwndCtrl = this->getDlgItem(IDC_RADIO1);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO2);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO3);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO4);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO5);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO6);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO7);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO8);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO9);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO10);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO11);
                Button_SetCheck(hwndCtrl, BST_CHECKED);

				this->m_iRed = this->m_gRed[10] ;
				this->m_iGreen = this->m_gGreen[10];
				this->m_iBlue = this->m_gBlue[10];
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
                hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}
		    }
           break; 
        }
		
		case IDC_CHECK3:  //view color
			{
		
	        if( Button_GetCheck(hControl) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
	        else outputColor(0,0,0);
	 break;
			}
		//color adjust 1x1 Cube
		case IDC_CHECK4: {
            if( Button_GetCheck(hControl) == BST_CHECKED) {
                m_UseColorK = ATMO_TRUE;
				/*hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{*/
					outputColor(m_iRed, m_iGreen, m_iBlue);
				  //}
			   	m_Use3dlut = ATMO_FALSE;
				hwndCtrl = this->getDlgItem(IDC_CHECK5);
				Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				m_UseSoftware = ATMO_FALSE;
				hwndCtrl = this->getDlgItem(IDC_ENABLE);
				Button_SetCheck(hwndCtrl, BST_UNCHECKED);
		
			} 
			else {
                m_UseColorK = ATMO_FALSE;

				hwndCtrl = this->getDlgItem(IDC_RADIO12);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO13);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO14);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO15);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO16);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);
				hwndCtrl = this->getDlgItem(IDC_RADIO17);
                Button_SetCheck(hwndCtrl, BST_UNCHECKED);

				/*this->m_iRed = this->m_gRed[10] ;
				this->m_iGreen = this->m_gGreen[10];
				this->m_iBlue = this->m_gBlue[10];
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);*/
                outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
		    }
           break; 
        }

        case IDC_SW_GAMMA_MODE: {
             //if(wmEvent == CBN_CLOSEUP)
			 {
                int i = ComboBox_GetCurSel(hControl);
                if(i>=0) {
                   m_GammaCorrect = (AtmoGammaCorrect)i;

				   hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

                }
             }
             break;
        }

        case IDC_EDT_GLOBALGAMMA: {
             if(wmEvent == EN_CHANGE) {
                double value;
                if(this->GetEditDouble(hControl,value) && (value>=0.1) && (value<=3.5)) {
                    m_Global_Gamma = int(value * 100);
                    SendMessage(getDlgItem(IDC_SL_GLOBALGAMMA), TBM_SETPOS, 1, m_Global_Gamma);

					hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }

        case IDC_EDT_GAMMA_RED: {
             if(wmEvent == EN_CHANGE) {
                double value; // 1.1 3.5
                if(this->GetEditDouble(hControl,value) && (value>=0.1f) && (value<=3.5f)) {
                    int iValue = (int)(value*100.0);
                    m_Gamma_Red = iValue;
                    SendMessage(getDlgItem(IDC_SL_GAMMA_RED), TBM_SETPOS, 1, iValue);

					hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }

        case IDC_EDT_GAMMA_GREEN: {
             if(wmEvent == EN_CHANGE) {
                double value; // 1.1 3.5
                if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f)) {
                    int iValue = (int)(value*100);
                    m_Gamma_Green = iValue;
                    SendMessage(getDlgItem(IDC_SL_GAMMA_GREEN), TBM_SETPOS, 1, iValue);

					hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }

        case IDC_EDT_GAMMA_BLUE: {
             if(wmEvent == EN_CHANGE) {
                double value; // 1.1 3.5
                if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f)) {
                    int iValue = (int)(value*100);
                    m_Gamma_Blue = iValue;
                    SendMessage(getDlgItem(IDC_SL_GAMMA_BLUE2), TBM_SETPOS, 1, iValue);

					hwndCtrl = this->getDlgItem(IDC_CHECK4);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
					int Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
					int Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
					int Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
					outputColor(Red, Green, Blue);
				  }
					else
					{
		
                hwndCtrl = this->getDlgItem(IDC_CHECK3);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
					}

                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }


        default:
            return CAtmoCustomColorPicker::ExecuteCommand(hControl, wmId, wmEvent);
    }

   return ATMO_TRUE;
}

void CAtmoWhiteSetup::outputColor(int red,int green,int blue) {
     // red, green, blue -- are no color value -- thats are norming value for red,green and blue;!
     CAtmoConnection *pAtmoConnection = this->m_pAtmoDynData->getAtmoConnection();
     if((pAtmoConnection==NULL) || (pAtmoConnection->isOpen()==ATMO_FALSE)) return;

         // hardware weissableich ausser kraft setzen!
         // mittels Dummypaket? (alte Hardware wird dieses Paket stillschweigend ignorieren hoffe ich)
     pColorPacket packet;
     AllocColorPacket(packet, m_pAtmoDynData->getAtmoConfig()->getZoneCount());

	 //calib
	 packet = CAtmoTools::ApplyGamma2(m_GammaCorrect,m_Gamma_Red, m_Gamma_Green, m_Gamma_Blue, m_Global_Gamma, packet);
	 //

	 for(int i=0; i < packet->numColors; i++) {
         packet->zone[i].r = (255 * red) / 255;
         packet->zone[i].g = (255 * green) / 255;
         packet->zone[i].b = (255 * blue) / 255;
     }

     pAtmoConnection->SendData(packet);

     delete (char *)packet;
}

ATMO_BOOL CAtmoWhiteSetup::Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int &red, int &green, int &blue, ATMO_BOOL &useSoftware, ATMO_BOOL &useSoftware2, ATMO_BOOL &useColorK, ATMO_BOOL &use3dlut, ATMO_BOOL &useinvert) {
       // so kann ColorPicker sein aktuellen Farbwert ausgeben - ohne etwas zu stören...
       EffectMode oldEffectMode = CAtmoTools::SwitchEffect(pAtmoDynData, emDisabled);

       CAtmoWhiteSetup *pAtmoWhiteSetup = new CAtmoWhiteSetup(hInst, parent, pAtmoDynData, red, green, blue, useSoftware, useSoftware2, useColorK, use3dlut, useinvert);

       ATMO_BOOL result = (pAtmoWhiteSetup->ShowModal() == IDOK);
       if(result == ATMO_TRUE) {
           red   = pAtmoWhiteSetup->m_iRed;
           green = pAtmoWhiteSetup->m_iGreen;
           blue  = pAtmoWhiteSetup->m_iBlue;
           useSoftware = pAtmoWhiteSetup->m_UseSoftware;
		   useSoftware2 = pAtmoWhiteSetup->m_UseSoftware2;
		   useColorK = pAtmoWhiteSetup->m_UseColorK;
		   use3dlut = pAtmoWhiteSetup->m_Use3dlut;
		   useinvert = pAtmoWhiteSetup->m_Useinvert;

           CAtmoConfig *pConfig = pAtmoDynData->getAtmoConfig();
           pConfig->setSoftware_gamma_global( pAtmoWhiteSetup->m_Global_Gamma );
           pConfig->setSoftware_gamma_red( pAtmoWhiteSetup->m_Gamma_Red );
           pConfig->setSoftware_gamma_green( pAtmoWhiteSetup->m_Gamma_Green );
           pConfig->setSoftware_gamma_blue( pAtmoWhiteSetup->m_Gamma_Blue );
           pConfig->setSoftware_gamma_mode( pAtmoWhiteSetup->m_GammaCorrect );
       }
       delete pAtmoWhiteSetup;

       CAtmoTools::SwitchEffect(pAtmoDynData, oldEffectMode);

       return result;
}
