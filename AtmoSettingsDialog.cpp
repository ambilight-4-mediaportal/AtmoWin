/*
* AtmoSettingsDialog.cpp: Setup Dialog for Atmo Win
*
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#include "StdAfx.h"
#include "windowsx.h"
#include "Winreg.h"
#include "ObjectModel.h"

#include "Atmosettingsdialog.h"
#include "AtmoConfig.h"
#include "AtmoThread.h"
#include "AtmoDisplays.h"
#include "AtmoConnection.h"
#include "AtmoTools.h"
#include "AtmoColorPicker.h"
#include "AtmoWhiteSetup.h"
#include "AtmoWhiteAdjHW.h"
#include "AtmoEditChannelAssignment.h"
#include "AtmoGradients.h"
#include "AtmoXMLConfig.h"

#include <string>
#include <strsafe.h>

#include "Resource.h"
#include "Language.h"
#pragma comment(lib, "user32.lib")

void CAtmoSettingsDialog::DeleteAllChannel()
{
	CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
	char *newconfigSection;

	GStringList *lstFind = new GStringList;
	GStringList *lstFound = new GStringList;
	GString *gsfound = new GString;
	char XMLSectionName[100];
	CUtils *Utils = new CUtils;

	string tmpStr = pAtmoConfig->lastprofile + "_ChannelAssignment_";
	newconfigSection = new char[tmpStr.length()+1];
	strcpy(newconfigSection, tmpStr.c_str());

	GString name = GString(newconfigSection);

	GetProfile().GetSectionNames(lstFind);

	__int64 count = lstFind->GetCount();	
	for(int i=0;i<count;i++) 
	{
		gsfound = lstFind->GetStrAt(i);

		if (gsfound->Left(strlen(name._str)) == name._str)
			lstFound->AddLast(gsfound->_str);
	}

	count = lstFound->GetCount() + 1;
	for(int i=0;i<count;i++) 
	{
		gsfound = lstFound->GetStrAt(i);

		if ( gsfound->Left(strlen(name._str)) == name._str)
		{
			strcpy(XMLSectionName, gsfound->_str);
			GetProfile().RemoveSection(XMLSectionName);
		}
	}
	lstFind->RemoveAll();
	lstFound->RemoveAll();
	pAtmoConfig->m_ChannelDelete = true;
	GetProfile().WriteCurrentConfig((const char *)Utils->strConfigFromFile, true);
}

CAtmoSettingsDialog::CAtmoSettingsDialog(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData) : CBasicDialog(hInst,IDD_SETUP,parent)
{
	this->m_pDynData = pDynData;
	m_pBackupConfig = new CAtmoConfig();
	m_pBackupConfig->Assign( m_pDynData->getAtmoConfig() );
	m_SaveHardWhiteSetup = ATMO_FALSE;
}

CAtmoSettingsDialog::~CAtmoSettingsDialog(void) 
{
	delete m_pBackupConfig;
}

void CAtmoSettingsDialog::LoadDisplayList() 
{
	CAtmoDisplays *pAtmoDisplays = this->m_pDynData->getAtmoDisplays();
	HWND hwndCtrl;
	hwndCtrl = getDlgItem(IDC_DISPLAYS);

	ComboBox_ResetContent(hwndCtrl);

	for(int i=0;i<pAtmoDisplays->getCount();i++) 
	{
		TAtmoDisplayInfo nfo = pAtmoDisplays->getDisplayInfo(i);
		ComboBox_AddString(hwndCtrl, nfo.infoText);
	}
	ComboBox_SetCurSel(hwndCtrl, m_pDynData->getAtmoConfig()->getLiveView_DisplayNr());
}

ATMO_BOOL CAtmoSettingsDialog::UpdateLiveViewValues(ATMO_BOOL showPreview)
{
	HWND hwndCtrl;
	CAtmoDisplays *pAtmoDisplays = this->m_pDynData->getAtmoDisplays();
	CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
	CLanguage *Lng = new CLanguage;

	pAtmoDisplays->ReloadList(); // auf nummer sicher!

	hwndCtrl = getDlgItem(IDC_DISPLAYS);
	int i = ComboBox_GetCurSel(hwndCtrl);

	if(i>=pAtmoDisplays->getCount()) 
	{
		pAtmoConfig->setLiveView_DisplayNr(0);
		LoadDisplayList();
		MessageBox(this->m_hDialog,Lng->sMessagesText[4],Lng->sMessagesText[3],MB_ICONERROR | MB_OK);
		return ATMO_FALSE;
	}
	pAtmoConfig->setLiveView_DisplayNr(i);

	if(showPreview == ATMO_TRUE)
	{
		CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
		if((pAtmoConnection!=NULL) && pAtmoConnection->isOpen())
		{
			CAtmoTools::SwitchEffect(m_pDynData, emLivePicture);
		}
	}
	return ATMO_TRUE;
}


ATMO_BOOL CAtmoSettingsDialog::UpdateColorChangeValues(ATMO_BOOL showPreview) 
{
	HWND hEditDelay = this->getDlgItem(IDC_ED_COLORCHANGE_DELAY);
	HWND hEditSteps = this->getDlgItem(IDC_ED_COLORCHANGE_STEPS);
	CLanguage *Lng = new CLanguage;

	char buffer[256];
	Edit_GetText(hEditDelay,buffer,255);
	int iDelay = atoi(buffer);
	if(iDelay < 10) 
	{
		MessageBox(this->m_hDialog,Lng->sMessagesText[5], Lng->sMessagesText[3],MB_OK | MB_ICONERROR);
		return ATMO_FALSE;
	}

	Edit_GetText(hEditSteps,buffer,255);
	atoi(buffer);
	int iSteps = atoi(buffer);
	if(iSteps < 1) 
	{
		MessageBox(this->m_hDialog,Lng->sMessagesText[6],Lng->sMessagesText[3],MB_OK | MB_ICONERROR);
		return ATMO_FALSE;
	}

	CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
	pAtmoConfig->setColorChanger_iDelay(iDelay);
	pAtmoConfig->setColorChanger_iSteps(iSteps);

	if(showPreview == ATMO_TRUE) 
	{
		CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
		if ((m_pDynData->getEffectThread()==NULL) || (pAtmoConfig->getEffectMode()!=emColorChange))  
		{
			if((pAtmoConnection!=NULL) && pAtmoConnection->isOpen())
			{
				CAtmoTools::SwitchEffect(m_pDynData, emColorChange);
			}
		}
	}

	return ATMO_TRUE;
}

ATMO_BOOL CAtmoSettingsDialog::UpdateLrColorChangeValues(ATMO_BOOL showPreview) 
{
	HWND hEditDelay = this->getDlgItem(IDC_ED_LRCOLORCHANGE_DELAY);
	HWND hEditSteps = this->getDlgItem(IDC_ED_LRCOLORCHANGE_STEPS);
	CLanguage *Lng = new CLanguage;

	char buffer[256];
	Edit_GetText(hEditDelay,buffer,255);
	int iDelay = atoi(buffer);
	if(iDelay < 10) 
	{
		MessageBox(this->m_hDialog,Lng->sMessagesText[7],Lng->sMessagesText[3],MB_OK | MB_ICONERROR);
		return ATMO_FALSE;
	}

	Edit_GetText(hEditSteps,buffer,255);
	atoi(buffer);
	int iSteps = atoi(buffer);
	if(iSteps < 1) 
	{
		MessageBox(this->m_hDialog,Lng->sMessagesText[8],Lng->sMessagesText[3],MB_OK | MB_ICONERROR);
		return ATMO_FALSE;
	}

	CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
	pAtmoConfig->setLrColorChanger_iDelay(iDelay);
	pAtmoConfig->setLrColorChanger_iSteps(iSteps);

	if(showPreview == ATMO_TRUE)
	{
		CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
		if ((m_pDynData->getEffectThread()==NULL) || (pAtmoConfig->getEffectMode()!=emLrColorChange))  
		{
			if((pAtmoConnection!=NULL) && pAtmoConnection->isOpen())
			{
				CAtmoTools::SwitchEffect(m_pDynData, emLrColorChange);
			}
		}
	}

	return ATMO_TRUE;
}


ATMO_BOOL CAtmoSettingsDialog::InitDialog(WPARAM wParam) 
{
	CAtmoConfig *config = m_pDynData->getAtmoConfig();
	char buffer[256],tmp[280];
	HWND hwndCtrl;

	GetWindowText(this->m_hDialog, buffer, sizeof(buffer));
	sprintf(tmp , "%s (%d x %d Edition... of MediaPortal Members)", buffer, CAP_WIDTH, CAP_HEIGHT );
	SetWindowText(this->m_hDialog, tmp);

	m_hCbxDevicetypes = getDlgItem(IDC_DEVICETYPE);
	for(int i=0; i<ATMO_DEVICE_COUNT; i++)
		ComboBox_AddString(m_hCbxDevicetypes, AtmoDeviceTypes[i]);
	if(m_pDynData->getAtmoConnection())
		ComboBox_SetCurSel(m_hCbxDevicetypes, (int)config->getConnectionType());
	else
		ComboBox_SetCurSel(m_hCbxDevicetypes, (int)-1);

	CLanguage *Lng = new CLanguage;

	Lng->szCurrentDir[Lng->GetSpecialFolder(CSIDL_COMMON_APPDATA)];	
	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

	Lng->XMLParse(Lng->szTemp, Lng->sSettingText, "SettingsDialog");

	m_hCbxEffects = getDlgItem(IDC_EFFECTS);
	ComboBox_AddString(m_hCbxEffects, Lng->sSettingText[0] );
	ComboBox_AddString(m_hCbxEffects, Lng->sSettingText[1] );
	ComboBox_AddString(m_hCbxEffects, Lng->sSettingText[2] );
	ComboBox_AddString(m_hCbxEffects, Lng->sSettingText[3] );
	ComboBox_AddString(m_hCbxEffects, Lng->sSettingText[4] );

	// Achtung - dazu muss die Reihenfolge der Enum Deklaration in AtmoConfig.h mit obiger Liste Synchron sein*g*
	ComboBox_SetCurSel(m_hCbxEffects, (int)config->getEffectMode());

	CUtils *Utils = new CUtils;

	hwndCtrl = getDlgItem(IDC_CB_PROFILES);

	__int64 count = Utils->profiles.GetCount();	
	GString rslt;
	for (int i=0; i<count;i++)
	{
		rslt = Utils->profiles.Serialize("|", i, 0);
		ComboBox_AddString(hwndCtrl, rslt);
	}
	Edit_SetText(hwndCtrl, config->lastprofile.data());

	hwndCtrl = getDlgItem(IDC_CB_DEVPROFILES);
	for (int i=0; i<count;i++)
	{
		rslt = Utils->profiles.Serialize("|", i, 0);
		ComboBox_AddString(hwndCtrl, rslt);
	}
	Edit_SetText(hwndCtrl, config->defaultprofile.data());

	hwndCtrl = this->getDlgItem(IDC_ED_COLORCHANGE_DELAY);
	sprintf(buffer,"%d",config->getColorChanger_iDelay());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,5);

	hwndCtrl = this->getDlgItem(IDC_ED_COLORCHANGE_STEPS);
	sprintf(buffer,"%d",config->getColorChanger_iSteps());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,4);

	hwndCtrl = this->getDlgItem(IDC_ED_LRCOLORCHANGE_DELAY);
	sprintf(buffer,"%d",config->getLrColorChanger_iDelay());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,5);

	hwndCtrl = this->getDlgItem(IDC_ED_LRCOLORCHANGE_STEPS);
	sprintf(buffer,"%d",config->getLrColorChanger_iSteps());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,4);

	hwndCtrl = this->getDlgItem(IDC_GDI_CAPURE_RATE);
	sprintf(buffer,"%d",config->getLiveView_GDI_FrameRate());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,2);

	hwndCtrl = this->getDlgItem(IDC_GDI_INTERLACE);
	sprintf(buffer,"%d",config->getLiveView_RowsPerFrame() );
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,2);

	hwndCtrl = this->getDlgItem(IDC_ED_ZONETOPC);
	sprintf(buffer,"%d",config->getZonesTopCount());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,3);

	hwndCtrl = this->getDlgItem(IDC_ED_ZONEBOTTOMC);
	sprintf(buffer,"%d",config->getZonesBottomCount());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,3);

	hwndCtrl = this->getDlgItem(IDC_ED_ZONELRC);
	sprintf(buffer,"%d",config->getZonesLRCount());
	Edit_SetText(hwndCtrl, buffer);
	Edit_LimitText(hwndCtrl,3);

	hwndCtrl = this->getDlgItem(IDC_CBX_ZONESUM);
	Button_SetCheck(hwndCtrl, config->getZoneSummary());

	hwndCtrl = this->getDlgItem(IDC_CHK_USESHUTDOWNCOLOR);
	Button_SetCheck(hwndCtrl, config->isSetShutdownColor());

	LoadDisplayList();

	SendMessage(getDlgItem(IDC_BU_CONFIG_DEVICE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[6]));
	SendMessage(getDlgItem(IDC_CBX_ZONESUM), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[7]));
	SendMessage(getDlgItem(IDC_BU_CHANNELASSIGNMENTS), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[8]));
	SendMessage(getDlgItem(IDC_BU_WHITEADJ_SW), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[9]));
	SendMessage(getDlgItem(IDC_BU_WHITEADJ_HW), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[10]));	
	SendMessage(getDlgItem(IDC_BU_SELSTATICCOLOR), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[11]));
	SendMessage(getDlgItem(IDC_BU_SELSHUTDOWNCOLOR), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[12]));
	SendMessage(getDlgItem(IDC_CHK_USESHUTDOWNCOLOR), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[13]));
	SendMessage(getDlgItem(IDC_WIDESCREEN), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[14]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[15]));
	SendMessage(getDlgItem(IDC_TXT_CURMODE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[16]));
	SendMessage(getDlgItem(IDC_GRP_FIXCOLOR), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[17]));
	SendMessage(getDlgItem(IDC_GRP_COLCHANGER), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[18]));
	SendMessage(getDlgItem(IDC_GRP_COLCHANGERLR), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[19]));	
	SendMessage(getDlgItem(IDC_GRP_SHUDOWNCOL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[20]));
	SendMessage(getDlgItem(IDC_TXT_STEPS), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[21]));
	SendMessage(getDlgItem(IDC_TXT_STEPS2), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[22]));
	SendMessage(getDlgItem(IDC_TXT_DEVICE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[23]));
	SendMessage(getDlgItem(IDC_TXT_TOPZONES), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[24]));	
	SendMessage(getDlgItem(IDC_TXT_BOTZONES), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[25]));
	SendMessage(getDlgItem(IDC_TXT_RLZONES), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[26]));
	SendMessage(getDlgItem(IDC_GRP_LIPARAM), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[27]));
	SendMessage(getDlgItem(IDC_LIVEMODE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[55]));

	hwndCtrl = getDlgItem(IDC_FILTERMODE);
	ComboBox_AddString(hwndCtrl, Lng->sSettingText[28]);
	ComboBox_AddString(hwndCtrl, Lng->sSettingText[29]);
	ComboBox_AddString(hwndCtrl, Lng->sSettingText[30]);
	ComboBox_SetCurSel(hwndCtrl, (int)config->getLiveViewFilterMode());

	hwndCtrl = getDlgItem(IDC_WIDESCREEN);
	Button_SetCheck(hwndCtrl, config->getLiveView_WidescreenMode());

	hwndCtrl = getDlgItem(IDC_LIVEMODE);
	Button_SetCheck(hwndCtrl, config->getLiveView_Mode());

	SetupSliderControl(IDC_EdgeWeighting,1,100,config->getLiveView_EdgeWeighting(),5);
	SendMessage(getDlgItem(IDC_EdgeWeighting),TBM_SETPAGESIZE,0,2);
	sprintf(buffer,Lng->sSettingText[31] + "[%d]",config->getLiveView_EdgeWeighting());
	SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);

	SetupSliderControl(IDC_BrightCorrect,10,300,config->getLiveView_BrightCorrect(),15);
	SendMessage(getDlgItem(IDC_BrightCorrect),TBM_SETPAGESIZE,0,10);
	sprintf(buffer,Lng->sSettingText[32] + "[%d%%]",config->getLiveView_BrightCorrect());
	SetStaticText(IDC_TXT_BRIGHTCORRECT, buffer);

	SetupSliderControl(IDC_DarknessLimit,0,50,config->getLiveView_DarknessLimit(),1);
	SendMessage(getDlgItem(IDC_DarknessLimit),TBM_SETPAGESIZE,0,1);
	sprintf(buffer,Lng->sSettingText[33] + "[%d]",config->getLiveView_DarknessLimit());
	SetStaticText(IDC_TXT_DARKNESSLIMIT, buffer);

	SetupSliderControl(IDC_HueWinSize,0,5,config->getLiveView_HueWinSize(),1);
	SendMessage(getDlgItem(IDC_HueWinSize),TBM_SETPAGESIZE,0,1);
	sprintf(buffer,Lng->sSettingText[34] + "[%d]",config->getLiveView_HueWinSize());
	SetStaticText(IDC_TXT_HUEWINSIZE, buffer);

	SetupSliderControl(IDC_SatWinSize,0,5,config->getLiveView_SatWinSize(),1);
	SendMessage(getDlgItem(IDC_SatWinSize),TBM_SETPAGESIZE,0,1);
	sprintf(buffer,Lng->sSettingText[35] + "[%d]",config->getLiveView_SatWinSize());
	SetStaticText(IDC_TXT_SATWINSIZE, buffer);

	SetupSliderControl(IDC_Overlap,0,3,config->getLiveView_Overlap(),1);
	SendMessage(getDlgItem(IDC_Overlap),TBM_SETPAGESIZE,0,1);
	sprintf(buffer,Lng->sSettingText[36] + "[%d]",config->getLiveView_Overlap());
	SetStaticText(IDC_TXT_OVERLAP, buffer);

	SetupSliderControl(IDC_Saturation,0,300,config->getLiveView_Saturation(),15);
	SendMessage(getDlgItem(IDC_Saturation),TBM_SETPAGESIZE,0,1);
	sprintf(buffer,Lng->sSettingText[37] + "[%d]",config->getLiveView_Saturation());
	SetStaticText(IDC_TXT_SATURATION, buffer);

	SetupSliderControl(IDC_Sensitivity,0,50,config->getLiveView_Sensitivity(),5);
	SendMessage(getDlgItem(IDC_Sensitivity),TBM_SETPAGESIZE,0,1);
	sprintf(buffer,Lng->sSettingText[38] + "[%d]",config->getLiveView_Sensitivity());
	SetStaticText(IDC_TXT_SENSITIVITY, buffer);

	SetupSliderControl(IDC_Filter_MeanLength,10,5000,config->getLiveViewFilter_MeanLength(),0);
	SendMessage(getDlgItem(IDC_Filter_MeanLength),TBM_SETPAGESIZE,0,100);
	sprintf(buffer,Lng->sSettingText[39] + "[%d ms]",config->getLiveViewFilter_MeanLength());
	SetStaticText(IDC_TXT_Filter_MeanLength, buffer);

	SetupSliderControl(IDC_Filter_MeanThreshold,1,100,config->getLiveViewFilter_MeanThreshold(),0);
	SendMessage(getDlgItem(IDC_Filter_MeanThreshold),TBM_SETPAGESIZE,0,5);
	sprintf(buffer,Lng->sSettingText[40] + "[%d%%]",config->getLiveViewFilter_MeanThreshold());
	SetStaticText(IDC_TXT_Filter_MeanThreshold, buffer);

	SetupSliderControl(IDC_Filter_PercentNew,1,100,config->getLiveViewFilter_PercentNew(),0);
	SendMessage(getDlgItem(IDC_Filter_PercentNew),TBM_SETPAGESIZE,0,5);
	sprintf(buffer,Lng->sSettingText[41] + "[%d%%]",config->getLiveViewFilter_PercentNew());
	SetStaticText(IDC_TXT_Filter_PercentNew, buffer);

	SendMessage(getDlgItem(IDC_TXT_GDICAFRAM), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[42]));
	SendMessage(getDlgItem(IDC_TXT_FILMODE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[43]));
	SendMessage(getDlgItem(IDC_STATIC15), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[44]));
	SendMessage(getDlgItem(IDC_STATIC16), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[45]));
	SendMessage(getDlgItem(IDC_STATIC17), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[46]));
	SendMessage(getDlgItem(IDC_GRADIENT_EDIT), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[47]));
	SendMessage(getDlgItem(IDC_STATIC18), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[48]));
	SendMessage(getDlgItem(IDC_STATIC19), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[49]));
	SendMessage(getDlgItem(IDC_BU_SAVEPROFILE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[50]));
	SendMessage(getDlgItem(IDC_BU_DELETE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[51]));
	SendMessage(getDlgItem(IDC_STATIC20), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[52]));
	SendMessage(getDlgItem(IDC_BU_LOADPROVILE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[53]));
	SendMessage(getDlgItem(IDC_STATIC21), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sSettingText[54]));

	SetWindowPos(this->m_hDialog, HWND_TOPMOST, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE);

	return ATMO_FALSE;
}

void CAtmoSettingsDialog::HandleHorzScroll(int code,int position,HWND scrollBarHandle) 
{
	// slider handling...
	char buffer[100];
	int dlgItemId = GetDlgCtrlID(scrollBarHandle);
	CLanguage *Lng = new CLanguage;

	switch(dlgItemId) 
	{
	case IDC_EdgeWeighting: 
		{
			int EdgeWeighting = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[31] + "[%d]",EdgeWeighting);
			SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_EdgeWeighting(EdgeWeighting);
			break;
		}

	case IDC_BrightCorrect: 
		{
			int BrightCorrect = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[32] + "[%d%%]",BrightCorrect);
			SetStaticText(IDC_TXT_BRIGHTCORRECT, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_BrightCorrect(BrightCorrect);
			break;
		}
	case IDC_DarknessLimit:
		{
			int DarknessLimit = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[33] + "[%d]",DarknessLimit);
			SetStaticText(IDC_TXT_DARKNESSLIMIT, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_DarknessLimit(DarknessLimit);
			break;
		}

	case IDC_HueWinSize: 
		{
			int HueWinSize = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[34] + "[%d]",HueWinSize);
			SetStaticText(IDC_TXT_HUEWINSIZE, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_HueWinSize(HueWinSize);
			break;
		}

	case IDC_SatWinSize: 
		{
			int SatWinSize = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[35] + "[%d]",SatWinSize);
			SetStaticText(IDC_TXT_SATWINSIZE, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_SatWinSize(SatWinSize);
			break;
		}

	case IDC_Overlap: 
		{
			int Overlap = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[36] + "[%d]",Overlap);
			SetStaticText(IDC_TXT_OVERLAP, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_Overlap(Overlap);
			//int  CAP_ZONE_OVERLAP = m_pAtmoConfig->getLiveView_Overlap();
			this->m_pDynData->CalculateDefaultZones();
			break;
		}

	case IDC_Saturation:
		{
			int Saturation = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[37] + "[%d]",Saturation);
			SetStaticText(IDC_TXT_SATURATION, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_Saturation(Saturation);
			this->m_pDynData->CalculateDefaultZones();
			break;
		}

	case IDC_Sensitivity:
		{
			int Sensitivity = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer, Lng->sSettingText[38] + "[%d]",Sensitivity);
			SetStaticText(IDC_TXT_SENSITIVITY, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveView_Sensitivity(Sensitivity);
			this->m_pDynData->CalculateDefaultZones();
			break;
		}

	case IDC_Filter_MeanLength: 
		{
			int Filter_MeanLength = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[39] + "[%d ms]",Filter_MeanLength);
			SetStaticText(IDC_TXT_Filter_MeanLength, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveViewFilter_MeanLength(Filter_MeanLength);
			break;
		}

	case IDC_Filter_MeanThreshold:
		{
			int Filter_MeanThreshold = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer,Lng->sSettingText[40] + "[%d%%]",Filter_MeanThreshold);
			SetStaticText(IDC_TXT_Filter_MeanThreshold, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveViewFilter_MeanThreshold(Filter_MeanThreshold);
			break;
		}

	case IDC_Filter_PercentNew: 
		{
			int Filter_PercentNew = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			sprintf(buffer, Lng->sSettingText[41] + "[%d%%]",Filter_PercentNew);
			SetStaticText(IDC_TXT_Filter_PercentNew, buffer);
			this->m_pDynData->getAtmoConfig()->setLiveViewFilter_PercentNew(Filter_PercentNew);
			break;
		}

	default:
		return;

	}
}

void CAtmoSettingsDialog::HandleVertScroll(int code,int position,HWND scrollBarHandle) 
{
}

void CAtmoSettingsDialog::UpdateDeviceConnection(AtmoConnectionType conType)
{
	EffectMode oldEffect = CAtmoTools::SwitchEffect(m_pDynData, emDisabled);
	CLanguage *Lng = new CLanguage;

	CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
	pAtmoConfig->setConnectionType(conType);

	if(CAtmoTools::RecreateConnection(m_pDynData) == ATMO_TRUE)
	{

		CAtmoTools::SwitchEffect(m_pDynData, oldEffect);

	} 
	else 
	{
		CAtmoConnection *connection = m_pDynData->getAtmoConnection();
		if(connection) 
		{
			if(connection->ShowConfigDialog( m_hInst, m_hDialog, pAtmoConfig))
			{
				if(CAtmoTools::RecreateConnection(m_pDynData) == ATMO_TRUE) 
				{
					CAtmoTools::SwitchEffect(m_pDynData, oldEffect);
					MessageBox( this->m_hDialog, Lng->sMessagesText[2], Lng->sMessagesText[1], MB_ICONINFORMATION);
				}
			}
		}
	}
}


template <typename T>
void remove(std::vector<T>& vec, size_t pos)
{
	std::vector<T>::iterator it = vec.begin();
	std::advance(it, pos);
	vec.erase(it);
}


// WM_COMMAND handler...
ATMO_BOOL CAtmoSettingsDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	HWND hwndCtrl;
	HWND hwndCtrl2;
	char buffer1[200];
	char buffer2[200];
	CUtils *Utils = new CUtils;
	CLanguage *Lng = new CLanguage;

	switch(wmId) 
	{
		//save or add profile
	case IDC_BU_SAVEPROFILE: 
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			GString rslt;

			hwndCtrl = this->getDlgItem(IDC_CB_PROFILES);
			Edit_GetText(hwndCtrl,buffer1,200);
			bool found = false;

			const char *ptr =  buffer1;

			string Profile1 = GetProfile().GetStringOrDefault("Default", "profiles", "");
			char *buffer = new char[Profile1.length()+1];
			strcpy(buffer, Profile1.c_str());

			// serialize buffer
			GStringList lst("|", buffer);
			__int64 count = lst.GetCount();

			// check for entry if exist
			if (count >> 0)
			{
				for (int i=0; i<count;i++)
				{		
					rslt = lst.Serialize("|", i, 0);
					if (rslt == buffer1)
						found=true;
				}
			}
			// if found do nothing
			if (!found) 
			{

				if (Profile1 != "")
					Profile1 = Profile1 + "|" + string(buffer1);
				else
					Profile1 = string(buffer1);
				
				buffer = new char[Profile1.length()+1];
				strcpy(buffer, Profile1.c_str());

				pAtmoConfig->profiles.push_back(buffer1);
				pAtmoConfig->lastprofile = buffer1;
			}

			//combobox entry
			if (!found) 
			{
				hwndCtrl = this->getDlgItem(IDC_CB_PROFILES);
				ComboBox_AddString(hwndCtrl, buffer1);

				hwndCtrl = this->getDlgItem(IDC_CB_DEVPROFILES);
				ComboBox_AddString(hwndCtrl, buffer1);
			}

			// sort buffer alphabetically
			Profile1 = "";
			strcpy(buffer1, "");
			hwndCtrl = this->getDlgItem(IDC_CB_PROFILES);
			count = ComboBox_GetCount(hwndCtrl);
			for (int i=0; i<count;i++)
			{
				if (Profile1 != "")
				{
					ComboBox_GetLBText(hwndCtrl, i, buffer1);
					Profile1 = Profile1 + "|" + string(buffer1);
				}				
				else
				{
					ComboBox_GetLBText(hwndCtrl, i, buffer1);
					Profile1 = buffer1;
				}
			}
			GetProfile().SetConfig("Default", "profiles", Profile1.c_str());
			
			// cleanup ChannelAssignment
			for(int i=1;i<10;i++)
				pAtmoConfig->m_ChannelAssignments[i] = NULL;

			// should be saved now
			pAtmoConfig->SaveSettings(pAtmoConfig->lastprofile);
			break;
		}
		//delete profile
	case IDC_BU_PROFDELETE: 
		{
			char msg[MAX_PATH+128];
			ATMO_BOOL r = ATMO_FALSE;
			sprintf(msg, Lng->sMessagesText[12] + "\n\r" + Lng->sMessagesText[13], "");
			if(MessageBox(this->m_hDialog, msg ,Lng->sMessagesText[1],MB_ICONQUESTION | MB_YESNO) == IDNO)
				return(r);

			DeleteAllChannel();

			hwndCtrl2 = this->getDlgItem(IDC_CB_DEVPROFILES);
			Edit_GetText(hwndCtrl2,buffer2,200);
			hwndCtrl = this->getDlgItem(IDC_CB_PROFILES);
			Edit_GetText(hwndCtrl,buffer1,200);

			GString rslt;

			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();

			// section remove
			GetProfile().RemoveSection(buffer1);

			//edit combobox entry
			pAtmoConfig->lastprofile="";
			hwndCtrl = this->getDlgItem(IDC_CB_PROFILES);
			ComboBox_ResetContent(hwndCtrl);
			Edit_SetText(hwndCtrl,pAtmoConfig->lastprofile.data());	

			//get profiles from XML
			string Profile1 = GetProfile().GetStringOrDefault("Default", "profiles", "");
			char *buffer = new char[Profile1.length()+1];
			strcpy(buffer, Profile1.c_str());

			//serialize the buffer
			GStringList lst("|", buffer);
			__int64 count = lst.GetCount();

			// if entry same as buffer then delete it
			for (int i=0; i<count;i++)
			{		
				rslt = lst.Serialize("|", i, 0);
				if (rslt == buffer1)
				{
					lst.Remove(rslt, 1, 1);
					break;
				}
			}

			// Get new count after delete one
			// add to Combobox to
			count = lst.GetCount();
			for (int i=0; i<count;++i)
			{
				rslt = lst.Serialize("|", i, 0);
				ComboBox_AddString(hwndCtrl, rslt);
			}
			ComboBox_ResetContent(hwndCtrl2);

			for (int i=0; i<count;++i)
			{
				rslt = lst.Serialize("|", i, 0);
				ComboBox_AddString(hwndCtrl2, rslt);
			}

			if (buffer1==buffer2) 				
			{
				Edit_SetText(hwndCtrl2,"");
				pAtmoConfig->defaultprofile="";
			}	
			// Set new Profile
			count = lst.GetCount();
			string tmpProfile;
			for (int i=0; i<count;++i)
			{
				rslt = lst.Serialize("|", i, 0);
				Profile1 = rslt;
				if (count >> 0)
				{
					if (tmpProfile == "")
						tmpProfile = Profile1;
					else
					tmpProfile = tmpProfile + "|" + Profile1;
				}
			}
			strcpy(buffer, tmpProfile.c_str());
			GetProfile().SetConfig("Default", "profiles", buffer);

			// do not save any Channel settings if Profile deleted
			pAtmoConfig->m_ChannelDelete = true;
			// execute Default profile 
			pAtmoConfig->lastprofile = "AtmoWinX";
			// should be saved now 
			pAtmoConfig->SaveSettings(pAtmoConfig->lastprofile);			
			break;
		}
		//load profile
	case IDC_BU_LOADPROVILE: 
		{
			hwndCtrl = this->getDlgItem(IDC_CB_PROFILES);
			Edit_GetText(hwndCtrl,buffer1,200);
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			pAtmoConfig->lastprofile=buffer1;
			pAtmoConfig->LoadSettings(pAtmoConfig->lastprofile);

			EndDialog(this->m_hDialog, wmId);	
			EffectMode backupEffectMode = pAtmoConfig->getEffectMode();
			// Effect Thread Stoppen der gerade läuft...
			CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);

			// schnittstelle neu öffnen... könne ja testweise geändert wurden sein?
			CAtmoTools::RecreateConnection(this->m_pDynData);
			// Effect Programm wieder starten...
			CAtmoTools::SwitchEffect(this->m_pDynData, backupEffectMode);
			pAtmoConfig->LoadSettings(pAtmoConfig->lastprofile);

			CAtmoSettingsDialog *pSetupDlg = new CAtmoSettingsDialog(this->m_hInst,this->m_hDialog, this->m_pDynData);
			pSetupDlg->ShowModal();
			break;
		}
	case IDOK: 
		{
			SetWindowPos(this->m_hDialog, HWND_NOTOPMOST, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE);

			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();

			AtmoConnectionType conType = (AtmoConnectionType)ComboBox_GetCurSel(getDlgItem(IDC_DEVICETYPE));
			pAtmoConfig->setConnectionType( conType );		

      LivePictureSource backupLivePictureSource = m_pDynData->getLivePictureSource();
			EffectMode newEffectMode = (EffectMode)ComboBox_GetCurSel(getDlgItem(IDC_EFFECTS));
			pAtmoConfig->setEffectMode(newEffectMode);

			if(UpdateColorChangeValues(ATMO_FALSE) == ATMO_FALSE)   return 0;
			if(UpdateLrColorChangeValues(ATMO_FALSE) == ATMO_FALSE) return 0;
			if(UpdateLiveViewValues(ATMO_FALSE) == ATMO_FALSE) return 0;

			if(Button_GetCheck(this->getDlgItem(IDC_CHK_USESHUTDOWNCOLOR)) == BST_CHECKED)
				pAtmoConfig->SetSetShutdownColor(1);
			else
				pAtmoConfig->SetSetShutdownColor(0);


			CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);
			// schnittstelle neu öffnen... könne ja testweise geändert wurden sein?
			CAtmoTools::RecreateConnection(this->m_pDynData);

			// Weissabgleich in Hardware Permanent speichern...
			if(m_SaveHardWhiteSetup == ATMO_TRUE) 
			{
				CAtmoConnection *atmoConnection = m_pDynData->getAtmoConnection();
				if((atmoConnection != NULL) && (atmoConnection->isOpen())) 
				{
					atmoConnection->HardwareWhiteAdjust(pAtmoConfig->getHardware_global_gamma(), 
						pAtmoConfig->getHardware_global_contrast(), pAtmoConfig->getHardware_contrast_red(),
						pAtmoConfig->getHardware_contrast_green(),pAtmoConfig->getHardware_contrast_blue(),
						pAtmoConfig->getHardware_gamma_red(),pAtmoConfig->getHardware_gamma_green(),pAtmoConfig->getHardware_contrast_blue(), ATMO_TRUE );
				}
			}

			CAtmoTools::SwitchEffect(this->m_pDynData, newEffectMode);
      CAtmoTools::SwitchLiveSource(this->m_pDynData, backupLivePictureSource);

			hwndCtrl = this->getDlgItem(IDC_CB_DEVPROFILES);
			Edit_GetText(hwndCtrl,buffer2, 200);
			pAtmoConfig->defaultprofile = buffer2;

			string Profile1 = GetProfile().GetStringOrDefault("Default", "profiles", "");
			pAtmoConfig->SaveSettings(Profile1);

			pAtmoConfig->m_UpdateEdgeWeightningFlag = 1;

			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDCANCEL: 
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			pAtmoConfig->Assign(this->m_pBackupConfig);

			EffectMode backupEffectMode = pAtmoConfig->getEffectMode();

			// Effect Thread Stoppen der gerade läuft...
			CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);

			// schnittstelle neu öffnen... könne ja testweise geändert wurden sein?
			CAtmoTools::RecreateConnection(this->m_pDynData);

			// Weissabgleich Rückgängig machen...
			CAtmoConnection *atmoConnection = m_pDynData->getAtmoConnection();
			if((atmoConnection != NULL) && (atmoConnection->isOpen())) 
			{
				atmoConnection->HardwareWhiteAdjust(pAtmoConfig->getHardware_global_gamma(), 
					pAtmoConfig->getHardware_global_contrast(), pAtmoConfig->getHardware_contrast_red(),
					pAtmoConfig->getHardware_contrast_green(),pAtmoConfig->getHardware_contrast_blue(),
					pAtmoConfig->getHardware_gamma_red(),pAtmoConfig->getHardware_gamma_green(),pAtmoConfig->getHardware_contrast_blue(), ATMO_FALSE );
			}

			// Effect Programm wieder starten...
			CAtmoTools::SwitchEffect(this->m_pDynData, backupEffectMode);

			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDC_BU_CHANNELASSIGNMENTS: 
		{
			CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
			if (pAtmoConnection != NULL)
			{
				CAtmoEditChannelAssignment *editChannels = new CAtmoEditChannelAssignment(this->m_hInst, this->m_hDialog, this->m_pDynData);
				editChannels->ShowModal();

				SetForegroundWindow(this->m_hDialog); 
				delete editChannels;
			}
			break;
		}

	case IDC_BU_CONFIG_DEVICE: 
		{
			CAtmoConfig *pAtmoConfig        = m_pDynData->getAtmoConfig();
			CAtmoConnection *atmoConnection = m_pDynData->getAtmoConnection();

			if(!atmoConnection)
				break;

			if( atmoConnection->ShowConfigDialog(this->m_hInst, this->m_hDialog, pAtmoConfig) )
			{
				// reconnect ...
				EffectMode oldEffect = CAtmoTools::SwitchEffect(m_pDynData, emDisabled);

				if(CAtmoTools::RecreateConnection(m_pDynData) == ATMO_TRUE) 
				{
					CAtmoTools::SwitchEffect(m_pDynData, oldEffect);
					MessageBox( this->m_hDialog, Lng->sMessagesText[2], Lng->sMessagesText[1], MB_ICONINFORMATION);
				}
			}
			break;
		}

	case IDC_EFFECTS: 
		{
			if(wmEvent == CBN_SELCHANGE) 
			{
				EffectMode newEffectMode = (EffectMode)ComboBox_GetCurSel(hControl);
				CAtmoTools::SwitchEffect(this->m_pDynData, newEffectMode);
			}
			break;
		}

	case IDC_DEVICETYPE: 
		{
			if(wmEvent == CBN_SELCHANGE) 
			{
				int i = ComboBox_GetCurSel(hControl);
				if(i>=0) {
					AtmoConnectionType connectionType = (AtmoConnectionType)i; //
					if(connectionType != m_pDynData->getAtmoConfig()->getConnectionType() || 
						!m_pDynData->getAtmoConnection() ) 
					{
						UpdateDeviceConnection( connectionType );
					}
				}
			}
			break;
		}

	case IDC_BU_SELSHUTDOWNCOLOR: 
		{
			// 2
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			int r,g,b;
			r = pAtmoConfig->getShutdownColor_Red();
			g = pAtmoConfig->getShutdownColor_Green();
			b = pAtmoConfig->getShutdownColor_Blue();
			if(CAtmoColorPicker::Execute(this->m_hInst, this->m_hDialog, this->m_pDynData, r, g, b) == ATMO_TRUE) 
			{
				// werte aus r g b verarbeiten!
				pAtmoConfig->setShutdownColor_Red(r);
				pAtmoConfig->setShutdownColor_Green(g);
				pAtmoConfig->setShutdownColor_Blue(b);
			}
			break;
		}

	case IDC_BU_SELSTATICCOLOR: 
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			int r,g,b;
			r = pAtmoConfig->getStaticColor_Red();
			g = pAtmoConfig->getStaticColor_Green();
			b = pAtmoConfig->getStaticColor_Blue();
			if(CAtmoColorPicker::Execute(this->m_hInst, this->m_hDialog, this->m_pDynData, r, g, b) == ATMO_TRUE) 
			{
				// werte aus r g b verarbeiten!
				pAtmoConfig->setStaticColor_Red(r);
				pAtmoConfig->setStaticColor_Green(g);
				pAtmoConfig->setStaticColor_Blue(b);
				// wenn aktiver Effekt die statische Farbe ist - diese auch sofort anwenden
				if( pAtmoConfig->getEffectMode() == emStaticColor ) 
					CAtmoTools::SwitchEffect( this->m_pDynData, emStaticColor);
			}
			break;
		}

	case IDC_BU_WHITEADJ_SW: 
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			int r,g,b;
			ATMO_BOOL useSoftwareWhiteAdj = pAtmoConfig->isUseSoftwareWhiteAdj();
			ATMO_BOOL useSoftware2WhiteAdj = pAtmoConfig->isUseSoftware2WhiteAdj();
			ATMO_BOOL useColorKWhiteAdj = pAtmoConfig->isUseColorKWhiteAdj();
			ATMO_BOOL use3dlut = pAtmoConfig->isUse3dlut();
			ATMO_BOOL useinvert = pAtmoConfig->isUseinvert();

			r = pAtmoConfig->getWhiteAdjustment_Red();
			g = pAtmoConfig->getWhiteAdjustment_Green();
			b = pAtmoConfig->getWhiteAdjustment_Blue();
			if(CAtmoWhiteSetup::Execute(this->m_hInst, this->m_hDialog, this->m_pDynData, r, g, b, useSoftwareWhiteAdj, 
				useSoftware2WhiteAdj, useColorKWhiteAdj, use3dlut, useinvert ) == ATMO_TRUE) 
			{
				// werte aus r g b verarbeiten!
				pAtmoConfig->setWhiteAdjustment_Red(r);
				pAtmoConfig->setWhiteAdjustment_Green(g);
				pAtmoConfig->setWhiteAdjustment_Blue(b);
				pAtmoConfig->setUseSoftwareWhiteAdj(useSoftwareWhiteAdj);
				pAtmoConfig->setUseSoftware2WhiteAdj(useSoftware2WhiteAdj);
				pAtmoConfig->setUseColorKWhiteAdj(useColorKWhiteAdj);
				pAtmoConfig->setUse3dlut(use3dlut);
				pAtmoConfig->setUseinvert(useinvert);
			}
			break;
		}

	case IDC_BU_WHITEADJ_HW: 
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			if(CAtmoWhiteAdjHW::Execute(this->m_hInst,this->m_hDialog, this->m_pDynData, m_SaveHardWhiteSetup) == ATMO_TRUE) 
			{
				//  nothin special to do -> alles wichtige hat execute schon erledigt!
			}
			break;
		}

	case IDC_GRADIENT_EDIT: 
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			if( CAtmoGradients::Execute( this->m_hInst, this->m_hDialog, this->m_pDynData ) == ATMO_TRUE)
			{
				// slide edge weightning anpassen?
				char buffer[64];				
				sprintf(buffer,Lng->sSettingText[31] + "[%d]",pAtmoConfig->getLiveView_EdgeWeighting());
				SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);
				SendMessage(this->getDlgItem(IDC_EdgeWeighting), TBM_SETPOS, true, pAtmoConfig->getLiveView_EdgeWeighting() );
			}
			break;
		}

	case IDC_WIDESCREEN:
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			if(Button_GetCheck(hControl) == BST_CHECKED)
				pAtmoConfig->setLiveView_WidescreenMode(1);
			else
				pAtmoConfig->setLiveView_WidescreenMode(0);
			break;
		}

	case IDC_LIVEMODE: {
		CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
		int AtmoSetup_Mode = pAtmoConfig->getLiveView_Mode();
		if(Button_GetCheck(hControl) == BST_CHECKED)
			pAtmoConfig->setLiveView_Mode(1);
		else
			pAtmoConfig->setLiveView_Mode(0);
		break;
										 }

	case IDC_FILTERMODE: 
		{
			if(wmEvent == CBN_SELCHANGE) 
			{
				CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
				int i = ComboBox_GetCurSel(hControl);
				pAtmoConfig->setLiveViewFilterMode((AtmoFilterMode)i);
			}
			break;
		}

	case IDC_DISPLAYS:
		{
			if(wmEvent == CBN_SELCHANGE)
			{
				CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
				int i = ComboBox_GetCurSel(hControl);
				if(i != pAtmoConfig->getLiveView_DisplayNr()) 
				{
					UpdateLiveViewValues( pAtmoConfig->getEffectMode() == emLivePicture );
				}
			}
			break;
		}

	case IDC_ED_COLORCHANGE_DELAY:
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if(value>=10)
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						pAtmoConfig->setColorChanger_iDelay(value);
					} 
					else
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_GDI_INTERLACE: 
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=1) && (value<CAP_HEIGHT)) 
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						pAtmoConfig->setLiveView_RowsPerFrame( value );
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_GDI_CAPURE_RATE:
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=1) && (value<=50)) 
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						pAtmoConfig->setLiveView_GDI_FrameRate(value);
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_ED_ZONETOPC: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=128)) 
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						int z = pAtmoConfig->getZonesLRCount() * 2 + pAtmoConfig->getZonesBottomCount() + value;

						if(z > CAP_MAX_NUM_ZONES)
							MessageBeep(MB_ICONEXCLAMATION);
						else
							pAtmoConfig->setZonesTopCount(value);
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_ED_ZONEBOTTOMC: 
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=128)) 
					{ 
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						int z = pAtmoConfig->getZonesLRCount() * 2 + pAtmoConfig->getZonesTopCount() + value;
						if(z > CAP_MAX_NUM_ZONES)
							MessageBeep(MB_ICONEXCLAMATION);
						else
							pAtmoConfig->setZonesBottomCount(value);
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_ED_ZONELRC: 
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0)
				{
					int value = atoi(buffer);
					if((value>=0) && (value<=64))
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						int z = value * 2 + pAtmoConfig->getZonesTopCount() + pAtmoConfig->getZonesBottomCount();
						if(z > CAP_MAX_NUM_ZONES)
							MessageBeep(MB_ICONEXCLAMATION);
						else
							pAtmoConfig->setZonesLRCount(value);
					} 
					else
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_CBX_ZONESUM:
		{
			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			if(Button_GetCheck(hControl) == BST_CHECKED)
				pAtmoConfig->setZoneSummary(ATMO_TRUE);
			else
				pAtmoConfig->setZoneSummary(ATMO_FALSE);
			break;
		}

	case IDC_ED_COLORCHANGE_STEPS: 
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if(value>=1) 
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						pAtmoConfig->setColorChanger_iSteps(value);
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_ED_LRCOLORCHANGE_STEPS:
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0)
				{
					int value = atoi(buffer);
					if(value>=1) 
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						pAtmoConfig->setLrColorChanger_iSteps(value);
					} 
					else 
					{
						MessageBeep(MB_ICONEXCLAMATION);
					}
				}
			}
			break;
		}

	case IDC_ED_LRCOLORCHANGE_DELAY: 
		{
			if(wmEvent == EN_CHANGE)
			{
				char buffer[20];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{
					int value = atoi(buffer);
					if(value>=10) 
					{
						CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
						pAtmoConfig->setLrColorChanger_iDelay(value);
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

