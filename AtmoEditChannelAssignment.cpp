#include "StdAfx.h"
#include "AtmoEditChannelAssignment.h"
#include "Resource.h"
#include "Language.h"
#include "ObjectModel.h"
#include "AtmoTools.h"

CAtmoEditChannelAssignment::CAtmoEditChannelAssignment(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData) :
	CBasicDialog(hInst,IDD_CHANNELASSIGNMENT,parent) 
{
	this->m_pDynData = pDynData;
	m_pZoneBoxes = NULL;
	m_pChannelNames = NULL;
	m_hScrollbox = 0;
	m_MaxScrollPos = 0;
	m_ScrollPos = 0;

}

CAtmoEditChannelAssignment::~CAtmoEditChannelAssignment(void) 
{

	delete m_pZoneBoxes;
	delete m_pChannelNames;
}

void CAtmoEditChannelAssignment::EditAssignment(CAtmoChannelAssignment *ca)
{
	// load this assignment into the editor
	HWND ctrl;
	if(!ca) return;

	for(int ch = 0; ch < m_iChCount; ch++)
	{
		if(m_pZoneBoxes[ch]) 
		{
			int zone = ca->getZoneIndex(ch);
			if(zone >= 0)
				ComboBox_SetCurSel(m_pZoneBoxes[ch], zone + 1);   
			else
				ComboBox_SetCurSel(m_pZoneBoxes[ch], 0 );   
		}
	}

	ctrl = getDlgItem(IDC_EDT_NAME);
	Edit_SetText(ctrl, ca->getName() );

	ctrl = getDlgItem(IDC_BU_DELETE);
	EnableWindow(ctrl, !(ca->system));

	ctrl = getDlgItem(IDC_BU_MODIFY);
	EnableWindow(ctrl, ATMO_FALSE);

	ctrl = getDlgItem(IDC_BU_ADD);
	EnableWindow(ctrl, ATMO_FALSE);
}

void CAtmoEditChannelAssignment::SaveAssignment(CAtmoChannelAssignment *ca)
{
	char buffer[CAP_MAX_NUM_ZONES];
	HWND ctrl;
	if(!ca) return;

	Edit_GetText(getDlgItem(IDC_EDT_NAME), buffer, sizeof(buffer) - 1);
	ca->setName( buffer );
	ca->system = ATMO_FALSE;

	if(ca->getSize() < m_iChCount)
		ca->setSize(m_iChCount);

	for(int ch = 0; ch < m_iChCount; ch++)
	{
		if(m_pZoneBoxes[ch]) 
		{
			int idx = ComboBox_GetCurSel( m_pZoneBoxes[ch] );
			if(idx >= 0)
				ca->setZoneIndex(ch, idx - 1);
			else    
				ca->setZoneIndex(ch, -1);
		}
	}

	ctrl = getDlgItem(IDC_BU_MODIFY);
	EnableWindow(ctrl, ATMO_FALSE);

	ctrl = getDlgItem(IDC_BU_ADD);
	EnableWindow(ctrl, ATMO_FALSE);

	ctrl = getDlgItem(IDC_BU_DELETE);
	EnableWindow(ctrl, ATMO_TRUE);
}


void CAtmoEditChannelAssignment::RealignComboboxes(int startPos)
{
	ATMO_BOOL visible;
	POINT topLeft;
	RECT r;

	if(!m_pZoneBoxes || !m_pChannelNames) return;

	GetWindowRect( getDlgItem(IDC_HW_ZONE_STATIC) , &r);
	topLeft.x = r.left;
	topLeft.y = r.top;
	ScreenToClient(m_hDialog, &topLeft);
	int comboLeft = topLeft.x;

	GetWindowRect( getDlgItem(IDC_HW_CHANNEL_STATIC) , &r);
	topLeft.x = r.left;
	topLeft.y = r.top;
	ScreenToClient(m_hDialog, &topLeft);

	int labelLeft = topLeft.x;

	GetWindowRect(m_hScrollbox, &r);
	topLeft.x = r.left;
	topLeft.y = r.top;
	ScreenToClient(m_hDialog, &topLeft);

	int top    = topLeft.y;

	topLeft.x = r.left;
	topLeft.y = r.bottom;
	ScreenToClient(m_hDialog, &topLeft);

	int bottom = topLeft.y;

	GetWindowRect(m_pZoneBoxes[0], &r);

	int controlHeight = (r.bottom - r.top) + 1;

	int y = top;

	for(int ch=0; ch < m_iChCount ; ch++)
	{
		visible = (ch>=startPos); // general Visible
		if(visible) 
		{
			y += controlHeight;
			visible = (y <= bottom);
		}
		if(visible) 
		{
			SetWindowPos(m_pZoneBoxes[ch],NULL,comboLeft,y - controlHeight,0,0,SWP_NOREPOSITION | SWP_NOSIZE | SWP_SHOWWINDOW);
			SetWindowPos(m_pChannelNames[ch],NULL,labelLeft,(y - controlHeight) + 4,0,0,SWP_NOREPOSITION | SWP_NOSIZE | SWP_SHOWWINDOW);
		}
		else
		{
			SetWindowPos(m_pZoneBoxes[ch],NULL,0,0,0,0,SWP_NOREPOSITION | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
			SetWindowPos(m_pChannelNames[ch],NULL,0,0,0,0,SWP_NOREPOSITION | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
		}
	}
}

ATMO_BOOL CAtmoEditChannelAssignment::InitDialog(WPARAM wParam) 
{
	CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
	CLanguage *Lng = new CLanguage;

	HWND ctrl;
	ctrl = getDlgItem(IDC_EDT_NAME);
	Edit_LimitText(ctrl,60);

	for(int i=0;i<10;i++) 
	{
		CAtmoChannelAssignment *src_ca = pAtmoConfig->getChannelAssignment(i);
		if(src_ca!=NULL) 
		{
			CAtmoChannelAssignment *temp = new CAtmoChannelAssignment(*src_ca);

			int index = ListBox_AddString(this->getDlgItem(IDC_LST_MAPPINGS),temp->getName());
			ListBox_SetItemData(this->getDlgItem(IDC_LST_MAPPINGS),index,(LPARAM)temp);
		}
	}


	CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();

	m_hScrollbox = getDlgItem(IDC_SCROLL);

	RECT rLabel, rScroller;

	GetWindowRect( getDlgItem(IDC_HW_ZONE_STATIC) , &rLabel);
	int comboLeft = rLabel.left;

	GetWindowRect(getDlgItem(IDC_HW_CHANNEL_STATIC), &rLabel);
	int labelWidth = comboLeft - rLabel.left - 2;

	GetWindowRect(m_hScrollbox, &rScroller);
	int comboWidth = rScroller.left - comboLeft - 4;
	int bottom     = rScroller.bottom - rScroller.top;

	m_iChCount = pAtmoConnection->getNumChannels();
	if(m_iChCount > 0) 
	{
		m_pZoneBoxes = new HWND[m_iChCount];
		m_pChannelNames = new HWND[m_iChCount];

		int zc = pAtmoConfig->getZoneCount();
		char buf[100];
		int y = 0;

		for(int ch=0; ch < m_iChCount; ch++)
		{
			m_pZoneBoxes[ch] = CreateWindow("COMBOBOX",NULL,
				CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL | WS_CHILD, 
				0, y, comboWidth, 180, m_hDialog, NULL, m_hInst, NULL);
			if(m_pZoneBoxes[ch])
			{
				SetWindowFont(m_pZoneBoxes[ch], GetWindowFont(m_hDialog), false);
				RECT comboRect;
				GetWindowRect(m_pZoneBoxes[ch], &comboRect);

				char *chName = pAtmoConnection->getChannelName(ch);

				m_pChannelNames[ch] = CreateWindow("STATIC", chName,
					WS_CHILD | SS_ENDELLIPSIS | SS_LEFTNOWORDWRAP, 
					0, y, labelWidth, comboRect.bottom-comboRect.top, m_hDialog, NULL, m_hInst, NULL);
				free(chName);

				SetWindowFont(m_pChannelNames[ch], GetWindowFont(m_hDialog), false);

				Lng->szCurrentDir[Lng->GetSpecialFolder(CSIDL_COMMON_APPDATA)];	
				sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

				GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

				// Read Buffer from IniFile
				sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

				Lng->XMLParse(Lng->szTemp, Lng->sChannelAssigmentText, "ChannelAssignment");

				ComboBox_AddString(m_pZoneBoxes[ch],Lng->sChannelAssigmentText[0]);
				int  zoneCounter = 0;
				for(int z = 0; z < pAtmoConfig->getZonesTopCount(); z++)
				{
					sprintf(buf,Lng->sChannelAssigmentText[1] +"[%d]",zoneCounter++);
					ComboBox_AddString(m_pZoneBoxes[ch], buf );
				}

				for(int z = 0; z < pAtmoConfig->getZonesLRCount(); z++)
				{
					sprintf(buf,Lng->sChannelAssigmentText[2] +"[%d]",zoneCounter++);
					ComboBox_AddString(m_pZoneBoxes[ch], buf );
				}

				for(int z = 0; z < pAtmoConfig->getZonesBottomCount(); z++)
				{
					sprintf(buf,Lng->sChannelAssigmentText[3] +"[%d]",zoneCounter++);
					ComboBox_AddString(m_pZoneBoxes[ch], buf );
				}

				for(int z = 0; z < pAtmoConfig->getZonesLRCount(); z++)
				{
					sprintf(buf,Lng->sChannelAssigmentText[4] +"[%d]",zoneCounter++);
					ComboBox_AddString(m_pZoneBoxes[ch], buf );
				}

				if(pAtmoConfig->getZoneSummary() == ATMO_TRUE)
				{
					ComboBox_AddString(m_pZoneBoxes[ch], Lng->sChannelAssigmentText[5]);
				}

				y += (comboRect.bottom - comboRect.top) + 1;
				if(y > bottom) 
					m_MaxScrollPos++;
			}
		}
		m_ScrollPos    = 0;

		SetScrollRange(m_hScrollbox, SB_CTL, 0, m_MaxScrollPos, false);

		SCROLLINFO ScrollInfo;
		memset(&ScrollInfo, 0, sizeof(ScrollInfo) );
		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.nPage  = 1;
		ScrollInfo.fMask  = SIF_PAGE;
		SetScrollInfo(m_hScrollbox, SB_CTL, &ScrollInfo, false);

		RealignComboboxes(0);
	}

	EditAssignment( pAtmoConfig->getChannelAssignment(0) );
	
	HWND ListBox	= getDlgItem(IDC_LST_MAPPINGS);
	if (pAtmoConfig->lastprofile != "")
	{
		char *buffer = new char[pAtmoConfig->lastprofile.length()+1];
		strcpy(buffer, pAtmoConfig->lastprofile.c_str());

		int selIndex = GetProfile().GetIntOrDefault(buffer, "CurrentChannelAssignment", 0);
		ListBox_SetCurSel(ListBox, selIndex);
		ExecuteCommand(ListBox, IDC_LST_MAPPINGS, LBN_SELCHANGE);
	}
	else
		ListBox_SetCurSel(ListBox, 0);

	SendMessage(getDlgItem(IDC_BU_DELETE), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[6]));
	SendMessage(getDlgItem(IDC_BU_MODIFY), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[7]));
	SendMessage(getDlgItem(IDC_BU_ADD), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[8]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[9]));
	SendMessage(getDlgItem(IDC_STATIC56), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[10]));
	SendMessage(getDlgItem(IDC_HW_CHANNEL_STATIC), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[11]));
	SendMessage(getDlgItem(IDC_HW_ZONE_STATIC), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[12]));
	SendMessage(getDlgItem(IDC_STATIC57), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[13]));
	SendMessage(this->m_hDialog, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sChannelAssigmentText[14]));

	return ATMO_FALSE;
}


void CAtmoEditChannelAssignment::CleanupListbox() 
{
	HWND listBox = getDlgItem(IDC_LST_MAPPINGS);
	int c = ListBox_GetCount(listBox);
	for(int i=0;i<c;i++) 
	{
		CAtmoChannelAssignment *ca = (CAtmoChannelAssignment *)ListBox_GetItemData(listBox, i);
		delete ca;
	}
	ListBox_ResetContent(listBox);
}

void CAtmoEditChannelAssignment::HandleVertScroll(int code,int position,HWND scrollBarHandle)
{
	SCROLLINFO ScrollInfo;

	if(scrollBarHandle == m_hScrollbox) 
	{ // 
		switch(code) 
		{
			// Scroll to top. 
		case SB_TOP: 
			m_ScrollPos = 0;
			SetScrollPos(scrollBarHandle, SB_CTL, m_ScrollPos, true);
			break;

			// Scroll to bottom. 
		case SB_BOTTOM:   
			m_ScrollPos = m_MaxScrollPos;
			SetScrollPos(scrollBarHandle, SB_CTL, m_ScrollPos, true);
			break;

			// End scroll. 
		case SB_ENDSCROLL: 
			{
				// allways the last event after a position change of the scrollbar
				ScrollInfo.cbSize = sizeof(ScrollInfo);
				ScrollInfo.fMask = SIF_ALL;
				GetScrollInfo(scrollBarHandle, SB_CTL, &ScrollInfo);
				RealignComboboxes(ScrollInfo.nPos);
				break;
			}

			//Scroll one line down.  
		case SB_LINEDOWN:
			m_ScrollPos++;
			if(m_ScrollPos > m_MaxScrollPos) m_ScrollPos = m_MaxScrollPos;
			SetScrollPos(scrollBarHandle, SB_CTL, m_ScrollPos, true);
			if(m_ScrollPos < m_MaxScrollPos)
				RealignComboboxes(m_ScrollPos);
			break;

			// Scroll one line up. 
		case SB_LINEUP:
			m_ScrollPos--;
			if(m_ScrollPos < 0) m_ScrollPos = 0;
			SetScrollPos(scrollBarHandle, SB_CTL, m_ScrollPos, true);
			if(m_ScrollPos > 0)
				RealignComboboxes(m_ScrollPos);
			break;

			// Scroll one page down. 
		case SB_PAGEDOWN:
			m_ScrollPos++;
			if(m_ScrollPos > m_MaxScrollPos) m_ScrollPos = m_MaxScrollPos;
			SetScrollPos(scrollBarHandle, SB_CTL, m_ScrollPos, true);
			break;

			// Scroll one page up. 
		case SB_PAGEUP:
			m_ScrollPos--;
			if(m_ScrollPos < 0) m_ScrollPos = 0;
			SetScrollPos(scrollBarHandle, SB_CTL, m_ScrollPos, true);
			break;

			// Scroll to the absolute position. The current position is provided in nPos. 
		case SB_THUMBPOSITION:
			// Drag scroll box to specified position. The current position is provided in nPos. 
		case SB_THUMBTRACK:
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			GetScrollInfo(scrollBarHandle, SB_CTL, &ScrollInfo);
			SetScrollPos(scrollBarHandle, SB_CTL, ScrollInfo.nTrackPos, true);
			break;
		}
	}

}

ATMO_BOOL CAtmoEditChannelAssignment::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	HWND ctrl;

	switch(wmId) 
	{
	case NULL:
		{
			if(m_pZoneBoxes && (wmEvent == CBN_SELCHANGE))
			{
				for(int i=0; i<m_iChCount; i++) 
				{
					if(m_pZoneBoxes[i] == hControl)
					{
						ctrl = getDlgItem(IDC_BU_MODIFY);
						EnableWindow(ctrl, ATMO_TRUE);

						ctrl = getDlgItem(IDC_BU_ADD);
						EnableWindow(ctrl, ATMO_TRUE);

						int selIndex = ListBox_GetCurSel(getDlgItem(IDC_LST_MAPPINGS));
						if(selIndex >= 0) 
						{
							CAtmoChannelAssignment *ca = (CAtmoChannelAssignment *)ListBox_GetItemData(getDlgItem(IDC_LST_MAPPINGS), selIndex);
							ctrl = getDlgItem(IDC_BU_MODIFY);
							EnableWindow(ctrl, (ca->system == ATMO_FALSE));
						}
						break;
					}
				}
			}
			break;
		}

	case IDCANCEL: 
		{
			CleanupListbox();
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDOK: 
		{
			// Ok. Copy Values to Config Object!
			CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
			pAtmoConfig->clearAllChannelMappings();
			HWND listBox = getDlgItem(IDC_LST_MAPPINGS);
			int c = ListBox_GetCount(listBox);
			for(int i=0;i<c;i++) 
			{
				CAtmoChannelAssignment *ca = (CAtmoChannelAssignment *)ListBox_GetItemData(listBox, i);
				pAtmoConfig->AddChannelAssignment(ca);
			}
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDC_LST_MAPPINGS: 
		{
			CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
			if(wmEvent == LBN_SELCHANGE) 
			{
				int selIndex = ListBox_GetCurSel(hControl);
				if(selIndex >= 0)
				{
					EditAssignment( (CAtmoChannelAssignment *)ListBox_GetItemData(hControl, selIndex) );
					CAtmoTools::SetChannelAssignment(m_pDynData, selIndex);		
					pAtmoConfig->SaveSettings(pAtmoConfig->lastprofile);				
				}
			}
			break;
		}

	case IDC_EDT_NAME:
		{
			if(wmEvent == EN_CHANGE) 
			{
				char buffer[CAP_MAX_NUM_ZONES];
				if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) 
				{

					ctrl = getDlgItem(IDC_BU_MODIFY);
					EnableWindow(ctrl, ATMO_TRUE);

					ctrl = getDlgItem(IDC_BU_ADD);
					EnableWindow(ctrl, ATMO_TRUE);

					int selIndex = ListBox_GetCurSel(getDlgItem(IDC_LST_MAPPINGS));
					if(selIndex >= 0) {
						CAtmoChannelAssignment *ca = (CAtmoChannelAssignment *)ListBox_GetItemData(getDlgItem(IDC_LST_MAPPINGS), selIndex);
						ctrl = getDlgItem(IDC_BU_MODIFY);
						EnableWindow(ctrl, (ca->system == ATMO_FALSE));
					}
				} 
				else 
				{
					ctrl = getDlgItem(IDC_BU_MODIFY);
					EnableWindow(ctrl, ATMO_FALSE);

					ctrl = getDlgItem(IDC_BU_ADD);
					EnableWindow(ctrl, ATMO_FALSE);
				}
			}
			break;
		}

	case IDC_BU_ADD: 
		{
			HWND listBox = getDlgItem(IDC_LST_MAPPINGS);
			CLanguage *Lng = new CLanguage;
			if(ListBox_GetCount(listBox)>=10) 
			{
				MessageBox(this->m_hDialog,Lng->sMessagesText[9], Lng->sMessagesText[1],MB_ICONINFORMATION | MB_OK);
			} 
			else 
			{
				CAtmoChannelAssignment *ca = new CAtmoChannelAssignment();
				int count = ListBox_GetCount(listBox);
				char buffer[64], buffer2[64];

				ctrl = getDlgItem(IDC_EDT_NAME);

				for(int i=0; i<count; i++) 
				{
					ListBox_GetText(listBox, i, buffer2);
					Edit_GetText(ctrl, buffer, sizeof(buffer));

					if (string(buffer) == string(buffer2))
					{
						MessageBox( this->m_hDialog, Lng->sMessagesText[10], Lng->sMessagesText[1], MB_ICONINFORMATION);
						return ATMO_FALSE;
						break;
					}
				}

				SaveAssignment(ca);

				int index = ListBox_AddString( listBox, ca->getName() );
				ListBox_SetItemData(listBox,index,(LPARAM)ca);
				ListBox_SetCurSel(listBox,index);
			}
			break;
		}

	case IDC_BU_MODIFY: 
		{
			HWND listBox = getDlgItem(IDC_LST_MAPPINGS);
			int listIndex = ListBox_GetCurSel(listBox);
			if (listIndex < 0) 
				listIndex = 0; 
			CAtmoChannelAssignment *ca = (CAtmoChannelAssignment *)ListBox_GetItemData(listBox, listIndex);
			if((ca!=NULL) && (ca->system == ATMO_FALSE)) 
			{

				SaveAssignment(ca);

				ListBox_DeleteString(listBox, listIndex);
				listIndex = ListBox_InsertString(listBox, listIndex, ca->getName() );
				ListBox_SetItemData(listBox, listIndex, (LPARAM)ca);
				ListBox_SetCurSel(listBox, listIndex);
			}
			break;
		}

	case IDC_BU_DELETE: 
		{
			HWND listBox = getDlgItem(IDC_LST_MAPPINGS);
			int listIndex = ListBox_GetCurSel(listBox);
			CUtils *Utils = new CUtils;

			CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
			char buffer2[64];

			char *XMLSectionName = new char[pAtmoConfig->lastprofile.length()];
			strcpy(XMLSectionName, pAtmoConfig->lastprofile.c_str());

			ListBox_GetText(listBox, listIndex, buffer2);
			string name = string(XMLSectionName) + "_ChannelAssignment_" + string(buffer2);				
			strcpy(XMLSectionName, name.c_str());

			GString section = GetProfile().GetStringOrDefault(XMLSectionName, "name", "?");

			if (string(buffer2) == section._str)
			{
				GetProfile().RemoveSection(XMLSectionName);
				pAtmoConfig->m_ChannelDelete = true;
			}

			ListBox_DeleteString(listBox, listIndex);
			ListBox_SetCurSel(listBox, 0);
			ExecuteCommand(listBox, IDC_LST_MAPPINGS, LBN_SELCHANGE);

			this->m_pDynData->getAtmoConfig()->setCurrentChannelAssignment(0);
			GetProfile().WriteCurrentConfig((const char *)Utils->strConfigFromFile, true);
			break;
		}
	default:
		return ATMO_FALSE;
	}
	return ATMO_TRUE;
}
