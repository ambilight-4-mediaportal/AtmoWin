/*
* AtmoCustomColorPicker.cpp: basic dialog for color selection - used only for inheritance to CAtmoColorPicker and CAtmoWhiteSetup
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/
#include "StdAfx.h"
#include "atmocustomcolorpicker.h"

#include "StdAfx.h"
#include "resource.h"
#include "windowsx.h"
#include "AtmoConfig.h"
#include "AtmoConnection.h"
#include "AtmoTools.h"
#include "commctrl.h"
#include "AtmoDmxSerialConnection.h"

CAtmoCustomColorPicker::CAtmoCustomColorPicker(HINSTANCE hInst, HWND parent, WORD dlgRessourceID, CAtmoDynData *pAtmoDynData, int red,int green,int blue):
	CBasicDialog(hInst, dlgRessourceID, parent)
{
	this->m_pAtmoDynData = pAtmoDynData;
	outputColor(red,green,blue);
	m_iRed   = red;
	m_iGreen = green;
	m_iBlue  = blue;
}

CAtmoCustomColorPicker::~CAtmoCustomColorPicker(void)
{
}

void CAtmoCustomColorPicker::out_wb_color()
{
	/*HWND hwndCtrl;
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
	}*/

	outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
	outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
}


void CAtmoCustomColorPicker::UpdateColorControls(ATMO_BOOL sliders,ATMO_BOOL edits) 
{
	// edits und sliders in sync!
	HWND hwndCtrl;
	char buffer[10];
	if(edits == ATMO_TRUE) 
	{
		hwndCtrl = this->getDlgItem(IDC_EDT_RED);
		sprintf(buffer,"%d",this->m_iRed);
		Edit_SetText(hwndCtrl,buffer);

		hwndCtrl = this->getDlgItem(IDC_EDT_GREEN);
		sprintf(buffer,"%d",this->m_iGreen);
		Edit_SetText(hwndCtrl,buffer);

		hwndCtrl = this->getDlgItem(IDC_EDT_BLUE);
		sprintf(buffer,"%d",this->m_iBlue);
		Edit_SetText(hwndCtrl,buffer);

		//calib
		CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();

		hwndCtrl = this->getDlgItem(IDC_RADIO1);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT1);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[0] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT2);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[0] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT3);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[0] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO2);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT4);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[1] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT5);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[1] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT6);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[1] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO3);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT7);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[2] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT8);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[2] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT9);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[2] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO4);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT10);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[3] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT11);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[3] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT12);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[3] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO5);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT13);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[4] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT14);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[4] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT15);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[4] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO6);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT16);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[5] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT17);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[5] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT18);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[5] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO7);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT19);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[6] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT20);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[6] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT21);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[6] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO8);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT22);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[7] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT23);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[7] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT24);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[7] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RADIO9);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT25);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[8] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT26);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[8] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT27);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[8] = this->m_iBlue;

		}
		hwndCtrl = this->getDlgItem(IDC_RADIO10);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT28);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[9] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT29);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[9] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT30);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[9] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RB_WHITE);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT31);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gRed[10] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT32);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gGreen[10] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT33);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_gBlue[10] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RB_RED);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT34);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cRed[0] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT35);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cRed[1] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT36);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cRed[2] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RB_GREEN);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT37);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cGreen[0] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT38);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cGreen[1] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT39);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cGreen[2] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RB_BLUE);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT40);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cBlue[0] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT41);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cBlue[1] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT42);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cBlue[2] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RB_YELLOW);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT43);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cYellow[0] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT44);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cYellow[1] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT45);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cYellow[2] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RB_MAGENTA);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT46);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cMagenta[0] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT47);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cMagenta[1] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT48);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cMagenta[2] = this->m_iBlue;
		}
		hwndCtrl = this->getDlgItem(IDC_RB_CYAN);
		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
		{
			hwndCtrl = this->getDlgItem(IDC_EDIT49);
			sprintf(buffer,"%d",this->m_iRed);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cCyan[0] = this->m_iRed;

			hwndCtrl = this->getDlgItem(IDC_EDIT50);
			sprintf(buffer,"%d",this->m_iGreen);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cCyan[1] = this->m_iGreen;

			hwndCtrl = this->getDlgItem(IDC_EDIT51);
			sprintf(buffer,"%d",this->m_iBlue);
			Edit_SetText(hwndCtrl,buffer);
			this->m_cCyan[2] = this->m_iBlue;
		}

	}

	if(sliders == ATMO_TRUE) 
	{
		hwndCtrl = this->getDlgItem(IDC_SL_RED);
		SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iRed);

		hwndCtrl = this->getDlgItem(IDC_SL_GREEN);
		SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iGreen);

		hwndCtrl = this->getDlgItem(IDC_SL_BLUE);
		SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iBlue);
	}
}

ATMO_BOOL CAtmoCustomColorPicker::InitDialog(WPARAM wParam) 
{
	HWND hwndCtrl;
	hwndCtrl = this->getDlgItem(IDC_EDT_RED);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = this->getDlgItem(IDC_EDT_GREEN);
	Edit_LimitText(hwndCtrl,3);
	hwndCtrl = this->getDlgItem(IDC_EDT_BLUE);
	Edit_LimitText(hwndCtrl,3);

	//calib
	CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();

	hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
	Button_SetCheck(hwndCtrl, BST_CHECKED);

	for (int iii=0; iii < 11; iii++)
		this->m_gRed[iii] = config->red_grid[iii] ;

	for (int iii=0; iii <= 12; iii++)
		config->red_whiteAdj[iii] = int( (float)config->red_grid[0]/13* iii) ;
	for (int iii=13; iii <= 25; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[0]+(float)(config->red_grid[1]-config->red_grid[0])/13*(iii-13));
	for (int iii=26; iii <= 50; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[1]+(float)(config->red_grid[2]-config->red_grid[1])/25*(iii-26));
	for (int iii=51; iii <= 76; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[2]+(float)(config->red_grid[3]-config->red_grid[2])/25*(iii-51));
	for (int iii=77; iii <= 101; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[3]+(float)(config->red_grid[4]-config->red_grid[3])/24*(iii-77));
	for (int iii=102; iii <= 127; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[4]+(float)(config->red_grid[5]-config->red_grid[4])/25*(iii-102));
	for (int iii=128; iii <= 153; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[5]+(float)(config->red_grid[6]-config->red_grid[5])/25*(iii-128));
	for (int iii=154; iii <= 178; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[6]+(float)(config->red_grid[7]-config->red_grid[6])/24*(iii-154));
	for (int iii=179; iii <= 204; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[7]+(float)(config->red_grid[8]-config->red_grid[7])/25*(iii-179));
	for (int iii=205; iii <= 229; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[8]+(float)(config->red_grid[9]-config->red_grid[8])/24*(iii-205));
	for (int iii=230; iii <= 255; iii++)
		config->red_whiteAdj[iii] = int(config->red_grid[9]+(float)(config->red_grid[10]-config->red_grid[9])/25*(iii-230));

	for (int iii=0; iii < 11; iii++)
		this->m_gGreen[iii] = config->green_grid[iii] ;

	for (int iii=0; iii <= 12; iii++)
		config->green_whiteAdj[iii] = int( (float)config->green_grid[0]/13* iii) ;
	for (int iii=13; iii <= 25; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[0]+(float)(config->green_grid[1]-config->green_grid[0])/13*(iii-13));
	for (int iii=26; iii <= 50; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[1]+(float)(config->green_grid[2]-config->green_grid[1])/25*(iii-26));
	for (int iii=51; iii <= 76; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[2]+(float)(config->green_grid[3]-config->green_grid[2])/25*(iii-51));
	for (int iii=77; iii <= 101; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[3]+(float)(config->green_grid[4]-config->green_grid[3])/24*(iii-77));
	for (int iii=102; iii <= 127; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[4]+(float)(config->green_grid[5]-config->green_grid[4])/25*(iii-102));
	for (int iii=128; iii <= 153; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[5]+(float)(config->green_grid[6]-config->green_grid[5])/25*(iii-128));
	for (int iii=154; iii <= 178; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[6]+(float)(config->green_grid[7]-config->green_grid[6])/24*(iii-154));
	for (int iii=179; iii <= 204; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[7]+(float)(config->green_grid[8]-config->green_grid[7])/25*(iii-179));
	for (int iii=205; iii <= 229; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[8]+(float)(config->green_grid[9]-config->green_grid[8])/24*(iii-205));
	for (int iii=230; iii <= 255; iii++)
		config->green_whiteAdj[iii] = int(config->green_grid[9]+(float)(config->green_grid[10]-config->green_grid[9])/25*(iii-230));

	for (int iii=0; iii < 11; iii++)
		this->m_gBlue[iii] = config->blue_grid[iii] ;

	for (int iii=0; iii <= 12; iii++)
		config->blue_whiteAdj[iii] = int( (float)config->blue_grid[0]/13* iii) ;
	for (int iii=13; iii <= 25; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[0]+(float)(config->blue_grid[1]-config->blue_grid[0])/13*(iii-13));
	for (int iii=26; iii <= 50; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[1]+(float)(config->blue_grid[2]-config->blue_grid[1])/25*(iii-26));
	for (int iii=51; iii <= 76; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[2]+(float)(config->blue_grid[3]-config->blue_grid[2])/25*(iii-51));
	for (int iii=77; iii <= 101; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[3]+(float)(config->blue_grid[4]-config->blue_grid[3])/24*(iii-77));
	for (int iii=102; iii <= 127; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[4]+(float)(config->blue_grid[5]-config->blue_grid[4])/25*(iii-102));
	for (int iii=128; iii <= 153; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[5]+(float)(config->blue_grid[6]-config->blue_grid[5])/25*(iii-128));
	for (int iii=154; iii <= 178; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[6]+(float)(config->blue_grid[7]-config->blue_grid[6])/24*(iii-154));
	for (int iii=179; iii <= 204; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[7]+(float)(config->blue_grid[8]-config->blue_grid[7])/25*(iii-179));
	for (int iii=205; iii <= 229; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[8]+(float)(config->blue_grid[9]-config->blue_grid[8])/24*(iii-205));
	for (int iii=230; iii <= 255; iii++)
		config->blue_whiteAdj[iii] = int(config->blue_grid[9]+(float)(config->blue_grid[10]-config->blue_grid[9])/25*(iii-230));

	this->m_iRed = this->m_gRed[10];
	this->m_iGreen = this->m_gGreen[10];
	this->m_iBlue = this->m_gBlue[10];
	int red = m_iRed;
	int green = m_iGreen;
	int blue = m_iBlue;

	this->m_cRed[0] = config->red_ColorK[256][0];
	this->m_cRed[1] = config->red_ColorK[256][1];
	this->m_cRed[2] = config->red_ColorK[256][2];
	this->m_cGreen[0] = config->green_ColorK[256][0];
	this->m_cGreen[1] = config->green_ColorK[256][1];
	this->m_cGreen[2] = config->green_ColorK[256][2];
	this->m_cBlue[0] = config->blue_ColorK[256][0];
	this->m_cBlue[1] = config->blue_ColorK[256][1];
	this->m_cBlue[2] = config->blue_ColorK[256][2];

	this->m_cYellow[0] = config->red_ColorK[257][0];
	this->m_cYellow[1] = config->red_ColorK[257][1];
	this->m_cYellow[2] = config->red_ColorK[257][2];
	this->m_cMagenta[0] = config->green_ColorK[257][0];
	this->m_cMagenta[1] = config->green_ColorK[257][1];
	this->m_cMagenta[2] = config->green_ColorK[257][2];
	this->m_cCyan[0] = config->blue_ColorK[257][0];
	this->m_cCyan[1] = config->blue_ColorK[257][1];
	this->m_cCyan[2] = config->blue_ColorK[257][2];

	/*hwndCtrl = this->getDlgItem(IDC_CHECK4);
	if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)*/
	if (config->isUseColorKWhiteAdj())
	{
		//normieren 255
		/* int sum_r = config->red_ColorK[256][0]+config->green_ColorK[256][0]+config->blue_ColorK[256][0];
		int sum_g = config->red_ColorK[256][1]+config->green_ColorK[256][1]+config->blue_ColorK[256][1];
		int sum_b = config->red_ColorK[256][2]+config->green_ColorK[256][2]+config->blue_ColorK[256][2];
		int c_max = sum_g;
		if (sum_r > c_max){ c_max = sum_r;}
		if (c_max < sum_b){ c_max = sum_b;}
		if (c_max > 255) 
		{
		float fac = (float)255/(float)c_max;
		config->red_ColorK[255][0] = config->red_ColorK[256][0]*fac;
		config->red_ColorK[255][1] = config->red_ColorK[256][1]*fac;
		config->red_ColorK[255][2] = config->red_ColorK[256][2]*fac;
		config->green_ColorK[255][0] = config->green_ColorK[256][0]*fac;
		config->green_ColorK[255][1] = config->green_ColorK[256][1]*fac;
		config->green_ColorK[255][2] = config->green_ColorK[256][2]*fac;
		config->blue_ColorK[255][0] = config->blue_ColorK[256][0]*fac;
		config->blue_ColorK[255][1] = config->blue_ColorK[256][1]*fac;
		config->blue_ColorK[255][2] = config->blue_ColorK[256][2]*fac;
		}
		else*/
		{
			config->red_ColorK[255][0] = config->red_ColorK[256][0];
			config->red_ColorK[255][1] = config->red_ColorK[256][1];
			config->red_ColorK[255][2] = config->red_ColorK[256][2];
			config->green_ColorK[255][0] = config->green_ColorK[256][0];
			config->green_ColorK[255][1] = config->green_ColorK[256][1];
			config->green_ColorK[255][2] = config->green_ColorK[256][2];
			config->blue_ColorK[255][0] = config->blue_ColorK[256][0];
			config->blue_ColorK[255][1] = config->blue_ColorK[256][1];
			config->blue_ColorK[255][2] = config->blue_ColorK[256][2];
		}
		/*red   = max(max((float)m_cRed[0]*this->m_iRed/255.0f,(float)m_cGreen[0]*this->m_iGreen/255.0f),(float)m_cBlue[0]*this->m_iBlue/255.0f);
		green =  max(max((float)m_cRed[1]*this->m_iRed/255.0f,(float)m_cGreen[1]*this->m_iGreen/255.0f),(float)m_cBlue[1]*this->m_iBlue/255.0f);
		blue  = max(max((float)m_cRed[2]*this->m_iRed/255.0f,(float)m_cGreen[2]*this->m_iGreen/255.0f),(float)m_cBlue[2]*this->m_iBlue/255.0f);*/
		red = min((float)m_cRed[0]*m_gRed[10]/255.0f+(float)m_cGreen[0]*m_gGreen[10]/255.0f+(float)m_cBlue[0]*m_gBlue[10]/255.0f,255);
		green = min((float)m_cRed[1]*m_gRed[10]/255.0f+(float)m_cGreen[1]*m_gGreen[10]/255.0f+(float)m_cBlue[1]*m_gBlue[10]/255.0f,255);
		blue = min((float)m_cRed[2]*m_gRed[10]/255.0f+(float)m_cGreen[2]*m_gGreen[10]/255.0f+(float)m_cBlue[2]*m_gBlue[10]/255.0f,255);

	}

	//
	hwndCtrl = this->getDlgItem(IDC_SL_RED);
	SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
	SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
	SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iRed);
	SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

	hwndCtrl = this->getDlgItem(IDC_SL_GREEN);
	SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
	SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
	SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iGreen);
	SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

	hwndCtrl = this->getDlgItem(IDC_SL_BLUE);
	SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
	SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
	SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iBlue);
	SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

	UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
	outputColor(red, green, blue);outputColor(red, green, blue);

	hwndCtrl = this->getDlgItem(IDC_RB_WHITE);
	Button_SetCheck(hwndCtrl, BST_CHECKED);

	return ATMO_TRUE;
}

ATMO_BOOL CAtmoCustomColorPicker::ExecuteCommand(HWND hControl,int wmId, int wmEvent)
{
	//calib
	HWND hwndCtrl;
	CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();
	char buffer[10];

	switch(wmId) 
	{
	case IDOK: 
		{
			// calib
			// WB Grid sichern
			CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();
			for (int iii=0; iii < 11; iii++)
				config->red_grid[iii] = this->m_gRed[iii] ;
			for (int iii=0; iii < 11; iii++)
				config->green_grid[iii] = this->m_gGreen[iii] ;
			for (int iii=0; iii < 11; iii++)
				config->blue_grid[iii] = this->m_gBlue[iii] ;
			// ColorB sichern
			for (int iii=0; iii < 3; iii++)
				config->red_ColorK[256][iii] = this->m_cRed[iii] ;
			for (int iii=0; iii < 3; iii++)
				config->green_ColorK[256][iii] = this->m_cGreen[iii] ;
			for (int iii=0; iii < 3; iii++)
				config->blue_ColorK[256][iii] = this->m_cBlue[iii] ;
			for (int iii=0; iii < 3; iii++)
				config->red_ColorK[257][iii] = this->m_cYellow[iii] ;
			for (int iii=0; iii < 3; iii++)
				config->green_ColorK[257][iii] = this->m_cMagenta[iii] ;
			for (int iii=0; iii < 3; iii++)
				config->blue_ColorK[257][iii] = this->m_cCyan[iii] ;

			config->ColorCube[0][0][0][0]=0;config->ColorCube[0][0][0][1]=0;config->ColorCube[0][0][0][2]=0; //black

			config->ColorCube[1][0][0][0]=config->red_ColorK[256][0];
			config->ColorCube[1][0][0][1]=config->red_ColorK[256][1];
			config->ColorCube[1][0][0][2]=config->red_ColorK[256][2]; //red

			config->ColorCube[0][1][0][0]=config->green_ColorK[256][0];
			config->ColorCube[0][1][0][1]=config->green_ColorK[256][1];
			config->ColorCube[0][1][0][2]=config->green_ColorK[256][2]; //green

			config->ColorCube[0][0][1][0]=config->blue_ColorK[256][0];
			config->ColorCube[0][0][1][1]=config->blue_ColorK[256][1];
			config->ColorCube[0][0][1][2]=config->blue_ColorK[256][2]; //blue

			config->ColorCube[1][1][0][0]=config->red_ColorK[257][0];
			config->ColorCube[1][1][0][1]=config->red_ColorK[257][1];
			config->ColorCube[1][1][0][2]=config->red_ColorK[257][2]; //yellow

			config->ColorCube[1][0][1][0]=config->green_ColorK[257][0];
			config->ColorCube[1][0][1][1]=config->green_ColorK[257][1];
			config->ColorCube[1][0][1][2]=config->green_ColorK[257][2]; //magenta

			config->ColorCube[0][1][1][0]=config->blue_ColorK[257][0];
			config->ColorCube[0][1][1][1]=config->blue_ColorK[257][1];
			config->ColorCube[0][1][1][2]=config->blue_ColorK[257][2]; //cyan

			config->ColorCube[1][1][1][0]=config->red_grid[10] ;config->ColorCube[1][1][1][1]=config->green_grid[10];
			config->ColorCube[1][1][1][2]=config->blue_grid[10]; //white

			// einfacher Weissabgleich + Farbkorrektur
			hwndCtrl = this->getDlgItem(IDC_CHK_COLORADJ);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				hwndCtrl = this->getDlgItem(IDC_CHK_WHITEADJ);
				if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
				{
					hwndCtrl = this->getDlgItem(IDC_CHECK2);
					if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) != BST_CHECKED)
					{
						;
					}
				}
			}

			// 11 level Weissabgleich + Farbkorrektur
			hwndCtrl = this->getDlgItem(IDC_CHK_COLORADJ);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				hwndCtrl = this->getDlgItem(IDC_CHECK2);
				if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
				{
					;//
				}
			}

			this->m_iRed = this->m_gRed[10] ;
			this->m_iGreen = this->m_gGreen[10];
			this->m_iBlue = this->m_gBlue[10];

			EndDialog(this->m_hDialog, wmId);
			break;

		}


	case IDCANCEL: 
		{
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDC_RADIO1:
		{

			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[0] ;
			this->m_iGreen = this->m_gGreen[0];
			this->m_iBlue = this->m_gBlue[0];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}

	case IDC_RADIO2: 
		{

			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[1] ;
			this->m_iGreen = this->m_gGreen[1];
			this->m_iBlue = this->m_gBlue[1];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO3: 
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[2] ;
			this->m_iGreen = this->m_gGreen[2];
			this->m_iBlue = this->m_gBlue[2];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO4:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[3] ;
			this->m_iGreen = this->m_gGreen[3];
			this->m_iBlue = this->m_gBlue[3];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO5:
		{

			this->m_iRed = this->m_gRed[4] ;
			this->m_iGreen = this->m_gGreen[4];
			this->m_iBlue = this->m_gBlue[4];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO6:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[5] ;
			this->m_iGreen = this->m_gGreen[5];
			this->m_iBlue = this->m_gBlue[5];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO7:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[6] ;
			this->m_iGreen = this->m_gGreen[6];
			this->m_iBlue = this->m_gBlue[6];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO8: 
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[7] ;
			this->m_iGreen = this->m_gGreen[7];
			this->m_iBlue = this->m_gBlue[7];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO9:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[8] ;
			this->m_iGreen = this->m_gGreen[8];
			this->m_iBlue = this->m_gBlue[8];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RADIO10: 
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_gRed[9] ;
			this->m_iGreen = this->m_gGreen[9];
			this->m_iBlue = this->m_gBlue[9];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			out_wb_color();
			break;
		}
	case IDC_RB_WHITE:
		{
			this->m_iRed = this->m_gRed[10] ;
			this->m_iGreen = this->m_gGreen[10];
			this->m_iBlue = this->m_gBlue[10];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);

			//out_wb_color();
			out_wb_color();
			break;
		}

	case IDC_RB_RED: 
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_cRed[0] ;
			this->m_iGreen = this->m_cRed[1];
			this->m_iBlue = this->m_cRed[2];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
			hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) out_wb_color();;
			break;
		}
	case IDC_RB_GREEN:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_cGreen[0] ;
			this->m_iGreen = this->m_cGreen[1];
			this->m_iBlue = this->m_cGreen[2];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
			hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) out_wb_color();;
			break;
		}
	case IDC_RB_BLUE:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_cBlue[0] ;
			this->m_iGreen = this->m_cBlue[1];
			this->m_iBlue = this->m_cBlue[2];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
			hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) out_wb_color();;
			break;
		}
	case IDC_RB_YELLOW:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_cYellow[0] ;
			this->m_iGreen = this->m_cYellow[1];
			this->m_iBlue = this->m_cYellow[2];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
			hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) out_wb_color();;
			break;
		}
	case IDC_RB_MAGENTA:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_cMagenta[0] ;
			this->m_iGreen = this->m_cMagenta[1];
			this->m_iBlue = this->m_cMagenta[2];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
			hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) out_wb_color();;
			break;
		}
	case IDC_RB_CYAN:
		{
			//UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
			this->m_iRed = this->m_cCyan[0] ;
			this->m_iGreen = this->m_cCyan[1];
			this->m_iBlue = this->m_cCyan[2];
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
			hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) out_wb_color();;
			break;
		}

	case IDC_CHK_SEDUCALMODE:
		{

			/*CAtmoDmxSerialConnection  *pAtmoConnection = this->m_pAtmoDynData-> getAtmoConnection();
			if((pAtmoConnection==NULL) || (pAtmoConnection->isOpen()==ATMO_FALSE)) break;

			DWORD iBytesWritten;

			hwndCtrl = this->getDlgItem(IDC_CHECK6);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) pAtmoConnection->SendSEDU(1);

			break;*/
		}

	case IDC_BUTTON1:
		{
			this->m_gRed[0]=12 ;
			this->m_gRed[1]=25 ;
			this->m_gRed[2]=51 ;
			this->m_gRed[3]=76 ;
			this->m_gRed[4]=102 ;
			this->m_gRed[5]=127 ;
			this->m_gRed[6]=153 ;
			this->m_gRed[7]=178 ;
			this->m_gRed[8]=204 ;
			this->m_gRed[9]=229 ;
			this->m_gRed[10]=255 ;
			this->m_gGreen[0]=12 ;
			this->m_gGreen[1]=25 ;
			this->m_gGreen[2]=51 ;
			this->m_gGreen[3]=76 ;
			this->m_gGreen[4]=102 ;
			this->m_gGreen[5]=127 ;
			this->m_gGreen[6]=153 ;
			this->m_gGreen[7]=178 ;
			this->m_gGreen[8]=204 ;
			this->m_gGreen[9]=229 ;
			this->m_gGreen[10]=255 ;
			this->m_gBlue[0]=12 ;
			this->m_gBlue[1]=25 ;
			this->m_gBlue[2]=51 ;
			this->m_gBlue[3]=76 ;
			this->m_gBlue[4]=102 ;
			this->m_gBlue[5]=127 ;
			this->m_gBlue[6]=153 ;
			this->m_gBlue[7]=178 ;
			this->m_gBlue[8]=204 ;
			this->m_gBlue[9]=229 ;
			this->m_gBlue[10]=255 ;
			UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
			break;
		}

	case IDC_BUTTON4:
		{
			hwndCtrl = this->getDlgItem(IDC_RADIO1);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=12 ;
				m_iGreen=12 ;
				m_iBlue=12 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO2);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=25 ;
				m_iGreen=25 ;
				m_iBlue=25 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO3);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=51 ;
				m_iGreen=51 ;
				m_iBlue=51 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO4);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=76 ;
				m_iGreen=76 ;
				m_iBlue=76 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO5);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=102 ;
				m_iGreen=102 ;
				m_iBlue=102 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO6);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=127 ;
				m_iGreen=127 ;
				m_iBlue=127 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO7);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=153 ;
				m_iGreen=153 ;
				m_iBlue=153 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO8);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=178 ;
				m_iGreen=178 ;
				m_iBlue=178 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO9);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=204 ;
				m_iGreen=204 ;
				m_iBlue=204 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RADIO10);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=229 ;
				m_iGreen=229 ;
				m_iBlue=229 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RB_WHITE);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=255 ;
				m_iGreen=255 ;
				m_iBlue=255 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RB_RED);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=255 ;
				m_iGreen=0 ;
				m_iBlue=0 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RB_GREEN);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=0 ;
				m_iGreen=255 ;
				m_iBlue=0 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RB_BLUE);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=0 ;
				m_iGreen=0 ;
				m_iBlue=255 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RB_YELLOW);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=255 ;
				m_iGreen=255 ;
				m_iBlue=0 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RB_MAGENTA);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=255 ;
				m_iGreen=0 ;
				m_iBlue=255 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			
			hwndCtrl = this->getDlgItem(IDC_RB_CYAN);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				m_iRed=0 ;
				m_iGreen=255 ;
				m_iBlue=255 ;
				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			break;
		}

		//get from lut
	case IDC_BUTTON5: 
		{							
			hwndCtrl = this->getDlgItem(IDC_RB_WHITE);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				this->m_iRed = config->little_ColorCube[16][16][16][0];
				this->m_iGreen = config->little_ColorCube[16][16][16][1];
				this->m_iBlue = config->little_ColorCube[16][16][16][2];

				hwndCtrl = this->getDlgItem(IDC_EDIT31);
				sprintf(buffer,"%d",this->m_iRed);
				Edit_SetText(hwndCtrl,buffer);
				this->m_gRed[10] = this->m_iRed;

				hwndCtrl = this->getDlgItem(IDC_EDIT32);
				sprintf(buffer,"%d",this->m_iGreen);
				Edit_SetText(hwndCtrl,buffer);
				this->m_gGreen[10] = this->m_iGreen;

				hwndCtrl = this->getDlgItem(IDC_EDIT33);
				sprintf(buffer,"%d",this->m_iBlue);
				Edit_SetText(hwndCtrl,buffer);
				this->m_gBlue[10] = this->m_iBlue;

				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}

			hwndCtrl = this->getDlgItem(IDC_RB_RED);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				this->m_iRed = config->little_ColorCube[16][0][0][0];
				this->m_iGreen = config->little_ColorCube[16][0][0][1];
				this->m_iBlue = config->little_ColorCube[16][0][0][2];

				hwndCtrl = this->getDlgItem(IDC_EDIT34);
				sprintf(buffer,"%d",this->m_iRed);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cRed[0] = this->m_iRed;

				hwndCtrl = this->getDlgItem(IDC_EDIT35);
				sprintf(buffer,"%d",this->m_iGreen);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cRed[1] = this->m_iGreen;

				hwndCtrl = this->getDlgItem(IDC_EDIT36);
				sprintf(buffer,"%d",this->m_iBlue);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cRed[2] = this->m_iBlue;

				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}

			hwndCtrl = this->getDlgItem(IDC_RB_GREEN);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				this->m_iRed = config->little_ColorCube[0][16][0][0];
				this->m_iGreen = config->little_ColorCube[0][16][0][1];
				this->m_iBlue = config->little_ColorCube[0][16][0][2];

				hwndCtrl = this->getDlgItem(IDC_EDIT37);
				sprintf(buffer,"%d",this->m_iRed);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cGreen[0] = this->m_iRed;

				hwndCtrl = this->getDlgItem(IDC_EDIT38);
				sprintf(buffer,"%d",this->m_iGreen);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cGreen[1] = this->m_iGreen;

				hwndCtrl = this->getDlgItem(IDC_EDIT39);
				sprintf(buffer,"%d",this->m_iBlue);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cGreen[2] = this->m_iBlue;

				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}

			hwndCtrl = this->getDlgItem(IDC_RB_BLUE);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				this->m_iRed = config->little_ColorCube[0][0][16][0];
				this->m_iGreen = config->little_ColorCube[0][0][16][1];
				this->m_iBlue = config->little_ColorCube[0][0][16][2];

				hwndCtrl = this->getDlgItem(IDC_EDIT40);
				sprintf(buffer,"%d",this->m_iRed);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cBlue[0] = this->m_iRed;

				hwndCtrl = this->getDlgItem(IDC_EDIT41);
				sprintf(buffer,"%d",this->m_iGreen);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cBlue[1] = this->m_iGreen;

				hwndCtrl = this->getDlgItem(IDC_EDIT42);
				sprintf(buffer,"%d",this->m_iBlue);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cBlue[2] = this->m_iBlue;

				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}

			hwndCtrl = this->getDlgItem(IDC_RB_YELLOW);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				this->m_iRed = config->little_ColorCube[16][16][0][0];
				this->m_iGreen = config->little_ColorCube[16][16][0][1];
				this->m_iBlue = config->little_ColorCube[16][16][0][2];

				hwndCtrl = this->getDlgItem(IDC_EDIT43);
				sprintf(buffer,"%d",this->m_iRed);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cYellow[0] = this->m_iRed;

				hwndCtrl = this->getDlgItem(IDC_EDIT44);
				sprintf(buffer,"%d",this->m_iGreen);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cYellow[1] = this->m_iGreen;

				hwndCtrl = this->getDlgItem(IDC_EDIT45);
				sprintf(buffer,"%d",this->m_iBlue);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cYellow[2] = this->m_iBlue;

				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}

			hwndCtrl = this->getDlgItem(IDC_RB_MAGENTA);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				this->m_iRed = config->little_ColorCube[16][0][16][0];
				this->m_iGreen = config->little_ColorCube[16][0][16][1];
				this->m_iBlue = config->little_ColorCube[16][0][16][2];

				hwndCtrl = this->getDlgItem(IDC_EDIT46);
				sprintf(buffer,"%d",this->m_iRed);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cMagenta[0] = this->m_iRed;

				hwndCtrl = this->getDlgItem(IDC_EDIT47);
				sprintf(buffer,"%d",this->m_iGreen);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cMagenta[1] = this->m_iGreen;

				hwndCtrl = this->getDlgItem(IDC_EDIT48);
				sprintf(buffer,"%d",this->m_iBlue);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cMagenta[2] = this->m_iBlue;

				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}

			hwndCtrl = this->getDlgItem(IDC_RB_CYAN);
			if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
			{
				this->m_iRed = config->little_ColorCube[0][16][16][0];
				this->m_iGreen = config->little_ColorCube[0][16][16][1];
				this->m_iBlue = config->little_ColorCube[0][16][16][2];

				hwndCtrl = this->getDlgItem(IDC_EDIT49);
				sprintf(buffer,"%d",this->m_iRed);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cCyan[0] = this->m_iRed;

				hwndCtrl = this->getDlgItem(IDC_EDIT50);
				sprintf(buffer,"%d",this->m_iGreen);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cCyan[1] = this->m_iGreen;

				hwndCtrl = this->getDlgItem(IDC_EDIT51);
				sprintf(buffer,"%d",this->m_iBlue);
				Edit_SetText(hwndCtrl,buffer);
				this->m_cCyan[2] = this->m_iBlue;

				UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
				out_wb_color();
			}
			break;
		}

	case IDC_EDT_RED: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=255))
					{
						this->m_iRed = value;
						UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
						/*hwndCtrl = this->getDlgItem(IDC_CHECK4);
						if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
						{
						int Red   = max(max((float)m_cRed[0]*this->m_iRed/255.0f,(float)m_cGreen[0]*this->m_iGreen/255.0f),(float)m_cBlue[0]*this->m_iBlue/255.0f);
						int Green =  max(max((float)m_cRed[1]*this->m_iRed/255.0f,(float)m_cGreen[1]*this->m_iGreen/255.0f),(float)m_cBlue[1]*this->m_iBlue/255.0f);
						int Blue  = max(max((float)m_cRed[2]*this->m_iRed/255.0f,(float)m_cGreen[2]*this->m_iGreen/255.0f),(float)m_cBlue[2]*this->m_iBlue/255.0f);
						outputColor(Red, Green, Blue);
						}
						else
						{

						hwndCtrl = this->getDlgItem(IDC_CHECK3);
						if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
						}*/
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_EDT_GREEN: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=255))
					{
						this->m_iGreen = value;
						UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
						/*hwndCtrl = this->getDlgItem(IDC_CHECK4);
						if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
						{
						int Red   = max(max((float)m_cRed[0]*this->m_iRed/255.0f,(float)m_cGreen[0]*this->m_iGreen/255.0f),(float)m_cBlue[0]*this->m_iBlue/255.0f);
						int Green =  max(max((float)m_cRed[1]*this->m_iRed/255.0f,(float)m_cGreen[1]*this->m_iGreen/255.0f),(float)m_cBlue[1]*this->m_iBlue/255.0f);
						int Blue  = max(max((float)m_cRed[2]*this->m_iRed/255.0f,(float)m_cGreen[2]*this->m_iGreen/255.0f),(float)m_cBlue[2]*this->m_iBlue/255.0f);
						outputColor(Red, Green, Blue);
						}
						else
						{

						hwndCtrl = this->getDlgItem(IDC_CHECK3);
						if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
						}*/
					} else {
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_EDT_BLUE:
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=255)) 
					{
						this->m_iBlue = value;
						UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
						/*hwndCtrl = this->getDlgItem(IDC_CHECK4);
						if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
						{
						int Red   = max(max((float)m_cRed[0]*this->m_iRed/255.0f,(float)m_cGreen[0]*this->m_iGreen/255.0f),(float)m_cBlue[0]*this->m_iBlue/255.0f);
						int Green =  max(max((float)m_cRed[1]*this->m_iRed/255.0f,(float)m_cGreen[1]*this->m_iGreen/255.0f),(float)m_cBlue[1]*this->m_iBlue/255.0f);
						int Blue  = max(max((float)m_cRed[2]*this->m_iRed/255.0f,(float)m_cGreen[2]*this->m_iGreen/255.0f),(float)m_cBlue[2]*this->m_iBlue/255.0f);
						outputColor(Red, Green, Blue);
						}
						else
						{

						hwndCtrl = this->getDlgItem(IDC_CHECK3);
						if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
						}*/
					} 
					else 
					{
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

void CAtmoCustomColorPicker::HandleHorzScroll(int code,int position,HWND scrollBarHandle)
{
	// WM_HSCROLL handler!
	//  WM_HSCROLL
	HWND hwndCtrl;

	int dlgItemId = GetDlgCtrlID(scrollBarHandle);
	switch(dlgItemId) 
	{
	case IDC_SL_RED:

		this->m_iRed   = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
		break;

	case IDC_SL_GREEN:
		this->m_iGreen = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
		break;

	case IDC_SL_BLUE:
		this->m_iBlue  = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
		break;
	default:
		return;

	}
	UpdateColorControls(ATMO_FALSE, ATMO_TRUE);

	// CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();

	/*int Red   = this->m_iRed;
	int Green = this->m_iGreen;
	int Blue  = this->m_iBlue;
	*/
	//hwndCtrl = this->getDlgItem(IDC_RB_RED);
	//   if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) != BST_CHECKED)
	//{
	//hwndCtrl = this->getDlgItem(IDC_RADIO13);
	//   if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) != BST_CHECKED)
	//{
	//hwndCtrl = this->getDlgItem(IDC_RADIO14);
	//   if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) != BST_CHECKED)
	//{   
	//hwndCtrl = this->getDlgItem(IDC_CHECK4);
	//		if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED)
	//		{
	//							
	//		 /*Red   = max(max((float)m_cRed[0]*this->m_iRed/255.0f,(float)m_cGreen[0]*this->m_iGreen/255.0f),(float)m_cBlue[0]*this->m_iBlue/255.0f);
	//		Green =  max(max((float)m_cRed[1]*this->m_iRed/255.0f,(float)m_cGreen[1]*this->m_iGreen/255.0f),(float)m_cBlue[1]*this->m_iBlue/255.0f);
	//		 Blue  = max(max((float)m_cRed[2]*this->m_iRed/255.0f,(float)m_cGreen[2]*this->m_iGreen/255.0f),(float)m_cBlue[2]*this->m_iBlue/255.0f);*/
	//		Red   = min((float)m_cRed[0]*m_iRed/255.0f+(float)m_cGreen[0]*m_iGreen/255.0f+(float)m_cBlue[0]*m_iBlue/255.0f,255);
	//		Green = min((float)m_cRed[1]*m_iRed/255.0f+(float)m_cGreen[1]*m_iGreen/255.0f+(float)m_cBlue[1]*m_iBlue/255.0f,255);
	//		Blue  = min((float)m_cRed[2]*m_iRed/255.0f+(float)m_cGreen[2]*m_iGreen/255.0f+(float)m_cBlue[2]*m_iBlue/255.0f,255);
	//	
	//	
	//	  }
	//}
	// 
	//}}

	hwndCtrl = this->getDlgItem(IDC_CHK_VIEWCOLOR);
	if(SendMessage(hwndCtrl, BM_GETSTATE, 0, 0) == BST_CHECKED) out_wb_color();

}


void CAtmoCustomColorPicker::outputColor(int red,int green,int blue)
{
}


