#include "StdAfx.h"
#include "AtmoGradients.h"
#include "resource.h"
#include "Language.h"

CAtmoGradients::CAtmoGradients(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData) : 
	CBasicDialog(hInst, IDD_GRADIENTS, parent)
{
	this->m_pDynData = pDynData;
	this->m_pConfig  = pDynData->getAtmoConfig();
	m_ZoneRadios = NULL;
	m_active_zone = -1;
	m_edge_weight = m_pConfig->getLiveView_EdgeWeighting();
	m_current_gradient = 0;
}


CAtmoGradients::~CAtmoGradients(void)
{
	if(m_current_gradient)
		DeleteObject( m_current_gradient );
	delete []m_ZoneRadios;  
}


ATMO_BOOL CAtmoGradients::Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData)
{
	CAtmoConnection *pAtmoConnection = pAtmoDynData->getAtmoConnection();
	CAtmoConfig *pAtmoConfig = pAtmoDynData->getAtmoConfig();

	EffectMode backupEffectMode = CAtmoTools::SwitchEffect(pAtmoDynData, emDisabled);

	CAtmoGradients *pAtmoGradients = new CAtmoGradients(hInst, parent, pAtmoDynData);

	ATMO_BOOL result (pAtmoGradients->ShowModal() == IDOK);
	if(result == ATMO_TRUE) 
	{  
		pAtmoConfig->setLiveView_EdgeWeighting( pAtmoGradients->m_edge_weight );
	} 
	else 
	{
		// altes Setup wieder Temporär in Controller laden!
		// flackert vermutlich furchtbar...aber wenns dann wirkt... ;-)
	}
	delete pAtmoGradients;

	CAtmoTools::SwitchEffect(pAtmoDynData, backupEffectMode);
	return result;
}

ATMO_BOOL CAtmoGradients::InitDialog(WPARAM wParam) 
{
	HWND groupBox = getDlgItem( IDC_ZONE_GROUP );
	CLanguage *Lng = new CLanguage;

	Lng->szCurrentDir[Lng->SetLngPath()];

	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);

	Lng->XMLParse(Lng->szTemp, Lng->sGradientsText, "Gradients");  	

	SetWindowLongPtr(groupBox, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	OrgGroupBoxProc = (WNDPROC)SetWindowLongPtr(groupBox, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(GroupBoxProc));

	int zonenIdx = 0;
	char zonename[100];
	int xpos, ypos;

	int zoneCount = m_pConfig->getZoneCount() + 1;
	m_ZoneRadios = new HWND[ zoneCount ];

	for(int i=0; i<zoneCount; i++)
		m_ZoneRadios[i] = 0;

#define radio_width 60
#define radio_height 20


	xpos = 10;
	ypos = 12;
	RECT rect, rrect;
	GetClientRect( groupBox, &rect );
	GetWindowRect( groupBox, &rrect );

	if(m_pConfig->getZonesTopCount() > 0) 
	{
		int radioWidth = (rect.right - rect.left - 20) /  (m_pConfig->getZonesTopCount() + 1);

		xpos = 10 + radioWidth -  radio_width/2;

		for( int zone=0; zone < m_pConfig->getZonesTopCount(); zone++)
		{
			sprintf( zonename, Lng->sGradientsText[0] + "%d", zonenIdx );
			HWND handle = CreateWindow("BUTTON", zonename,
				BS_AUTORADIOBUTTON  | BS_LEFTTEXT  | WS_VISIBLE | WS_TABSTOP | WS_CHILD, 
				xpos, ypos, radio_width, radio_height, groupBox, NULL, m_hInst, NULL);
			SetWindowFont(handle, GetWindowFont(m_hDialog), false);

			m_ZoneRadios[ zonenIdx ] = handle;
			zonenIdx++;
			xpos += radioWidth;
		}
	}

	if( m_pConfig->getZonesLRCount() > 0 )
	{
		int radioHeight = (rect.bottom - rect.top - 40) / (m_pConfig->getZonesLRCount() + 1);

		xpos = rect.right - 10 - radio_width;
		ypos = rect.top + 20 + radioHeight - radio_height / 2;

		for( int zone=0; zone < m_pConfig->getZonesLRCount(); zone++)
		{
			sprintf( zonename, Lng->sGradientsText[0] + "%d", zonenIdx );
			HWND handle = CreateWindow("BUTTON", zonename,
				BS_AUTORADIOBUTTON  | BS_LEFTTEXT  | WS_VISIBLE | WS_TABSTOP | WS_CHILD, 
				xpos, ypos, radio_width, radio_height, groupBox, NULL, m_hInst, NULL);
			SetWindowFont(handle, GetWindowFont(m_hDialog), false);


			m_ZoneRadios[ zonenIdx ] = handle;
			zonenIdx++;
			ypos += radioHeight;
		}
	}

	if(m_pConfig->getZonesBottomCount() > 0) 
	{
		int radioWidth = (rect.right - rect.left - 20) /  (m_pConfig->getZonesBottomCount() + 1);
		xpos = rect.right -  10 - radioWidth - radio_width/2 ;
		ypos = rect.bottom - 30;

		for( int zone=0; zone < m_pConfig->getZonesBottomCount(); zone++)
		{
			sprintf( zonename, Lng->sGradientsText[0] + "%d", zonenIdx );

			HWND handle = CreateWindow("BUTTON", zonename,
				BS_AUTORADIOBUTTON  | BS_LEFTTEXT  | WS_VISIBLE | WS_TABSTOP | WS_CHILD, 
				xpos, ypos, radio_width, radio_height, groupBox, NULL, m_hInst, NULL);
			SetWindowFont(handle, GetWindowFont(m_hDialog), false);


			m_ZoneRadios[ zonenIdx ] = handle;

			zonenIdx++;
			xpos -= radioWidth;
		}
	}

	if( m_pConfig->getZonesLRCount() > 0 )
	{
		int radioHeight = (rect.bottom - rect.top - 40) / (m_pConfig->getZonesLRCount() + 1);

		xpos = rect.left + 10;
		ypos = rect.bottom - 20 - radioHeight - radio_height / 2;

		for( int zone=0; zone < m_pConfig->getZonesLRCount(); zone++)
		{
			sprintf( zonename, Lng->sGradientsText[0] + "%d", zonenIdx );
			HWND handle = CreateWindow("BUTTON", zonename,
				BS_AUTORADIOBUTTON  | BS_RIGHT | WS_VISIBLE | WS_TABSTOP | WS_CHILD, 
				xpos, ypos, radio_width, radio_height, groupBox, NULL, m_hInst, NULL);
			SetWindowFont(handle, GetWindowFont(m_hDialog), false);


			m_ZoneRadios[ zonenIdx ] = handle;
			zonenIdx++;
			ypos -= radioHeight;
		}
	}

	if( m_pConfig->getZoneSummary() )
	{
		xpos = rect.left + 20 + radio_width;
		ypos = (rect.bottom - 50);

		sprintf( zonename, Lng->sGradientsText[1] + "%d", zonenIdx );
		HWND handle = CreateWindow("BUTTON", zonename,
			BS_AUTORADIOBUTTON  | BS_RIGHT | WS_VISIBLE | WS_TABSTOP | WS_CHILD, 
			xpos, ypos, 100, radio_height, groupBox, NULL, m_hInst, NULL);

		SetWindowFont(handle, GetWindowFont(m_hDialog), false);

		m_ZoneRadios[ zonenIdx ] = handle;
		zonenIdx++;
	}

	m_ZoneRadios[ zonenIdx ] = 0;

	this->SetupSliderControl(IDC_EDGE_WEIGHT,1,100,this->m_edge_weight,5); //Gradient slider
	SendMessage(getDlgItem(IDC_EDGE_WEIGHT),TBM_SETPAGESIZE,0,2);

	char buffer[32];
	sprintf(buffer,Lng->sGradientsText[2] + "[%d]",m_edge_weight);
	SetWindowText( getDlgItem(IDC_STATIC_EDGEW), buffer);

	if(zonenIdx > 0) {
		SetActiveZone( 0 );
		SendMessage( m_ZoneRadios[ 0 ], (UINT)BM_SETCHECK, BST_CHECKED, 0);
	}

	SendMessage(this->m_hDialog, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sGradientsText[15]));
	SendMessage(getDlgItem(IDCANCEL), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sGradientsText[16]));
	SendMessage(getDlgItem(IDC_ZONE_GROUP), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(Lng->sGradientsText[17]));

	return ATMO_FALSE;
}

void CAtmoGradients::HandleHorzScroll(int code,int position,HWND scrollBarHandle) 
{
	// slider handling...
	char buffer[100];
	int dlgItemId = GetDlgCtrlID(scrollBarHandle);
	CLanguage *Lng = new CLanguage;

	switch(dlgItemId) 
	{
	case IDC_EDGE_WEIGHT: 
		{
			m_edge_weight = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
			SetActiveZone( m_active_zone );
			HWND groupBox = getDlgItem( IDC_ZONE_GROUP );
			InvalidateRect( groupBox, NULL, false);
			sprintf(buffer,Lng->sGradientsText[2] + "[%d]",m_edge_weight);
			SetWindowText( getDlgItem(IDC_STATIC_EDGEW), buffer);
			break;
		}

	}
}

void CAtmoGradients::SetActiveZone(int zone)
{
	if(!m_ZoneRadios || (zone<0)) return;


	CAtmoZoneDefinition *azone = m_pConfig->getZoneDefinition( zone );
	if(!azone) return;

	m_active_zone = zone;

	int *destWeight = new int[CAP_WIDTH * CAP_HEIGHT];

	azone->UpdateWeighting(destWeight, m_pConfig->getLiveView_WidescreenMode(), m_edge_weight);

	BITMAP bmp;
	int byte = (CAP_WIDTH * 4);
	if(byte & 1) 
		byte++;

	unsigned char *pixel_buffer = new unsigned char[byte * CAP_HEIGHT];
	unsigned char *tmp = pixel_buffer;
	for(int y = 0; y < CAP_HEIGHT; y++) 
	{
		tmp = pixel_buffer + y * byte;
		for(int x = 0; x < CAP_WIDTH; x++) 
		{
			*tmp = 0; tmp++; // R
			*tmp = (unsigned char)destWeight[y * CAP_WIDTH + x]; tmp++; // G
			*tmp = 0; tmp++; // B
			*tmp = 0; tmp++; // A
		}
	}

	bmp.bmType = 0;
	bmp.bmBitsPixel = 32;
	bmp.bmHeight = CAP_HEIGHT;
	bmp.bmWidth  = CAP_WIDTH;
	bmp.bmWidthBytes = byte;
	bmp.bmPlanes = 1;
	bmp.bmBits   = pixel_buffer;

	if(m_current_gradient)
	{
		DeleteObject( m_current_gradient );
		m_current_gradient = 0;
	}
	m_current_gradient = CreateBitmapIndirect(&bmp);
	DWORD error = GetLastError();

	delete []destWeight;
	delete []pixel_buffer;
}

ATMO_BOOL CAtmoGradients::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
	ATMO_BOOL r = CBasicDialog::HandleMessage(hwnd, uMsg, wParam, lParam);
	HWND groupBox = getDlgItem( IDC_ZONE_GROUP );
	CLanguage *Lng = new CLanguage;

	if((hwnd == groupBox) && (uMsg == WM_PAINT)) 
	{
		HDC hdc = GetDC(hwnd);
		HDC memDC = CreateCompatibleDC(hdc);
		RECT r;
		GetClientRect( groupBox, &r );
		int cx = (r.right - r.left) / 2;
		int cy = (r.bottom - r.top) / 2;

		HGDIOBJ oldObject = SelectObject( memDC, m_current_gradient );
		DWORD error = GetLastError();

		cx = cx - (CAP_WIDTH * 3) / 2;
		cy = cy - (CAP_HEIGHT * 3) / 2;

		StretchBlt( hdc, cx, cy, CAP_WIDTH*3, CAP_HEIGHT*3, memDC, 0, 0, CAP_WIDTH, CAP_HEIGHT, SRCCOPY );
		error = GetLastError();

		SelectObject( memDC, oldObject );
		DeleteDC( memDC );
		ReleaseDC(hwnd, hdc);
	}

	if((hwnd == groupBox) && (uMsg == WM_DROPFILES) && (m_active_zone>=0))
	{
		char psz_filename[MAX_PATH];
		char psz_filename_dest[MAX_PATH];
		char msg[MAX_PATH+128];
		DragQueryFile((HDROP)wParam, 0, psz_filename, sizeof(psz_filename));
		int i;
		i = (int)strlen(psz_filename) - 1;
		while(i && psz_filename[i]!='.') i--;
		if(i>0) 
		{
			if(!stricmp(&psz_filename[i],".bmp"))
			{
				CAtmoZoneDefinition *azone = m_pConfig->getZoneDefinition( m_active_zone );
				if(azone)
				{

					sprintf(psz_filename_dest,"%s%s\\zone_%d.bmp",
						m_pDynData->getWorkDir(),
						m_pDynData->getAtmoConnection()->getDevicePath(),
						m_active_zone
						);

					FILE *fp=fopen(psz_filename_dest,"r");
					if(fp) 
					{
						fclose(fp);
						sprintf( msg, Lng->sGradientsText[3] + "%s" + Lng->sGradientsText[4], psz_filename_dest);
						if(MessageBox(m_hDialog, msg ,Lng->sGradientsText[5],MB_ICONQUESTION | MB_YESNO) == IDNO)
							return(r);
					}

					switch(azone->LoadGradientFromBitmap( psz_filename ))
					{
					case ATMO_LOAD_GRADIENT_OK:   
						SetActiveZone( m_active_zone );
						InvalidateRect( groupBox, NULL, false);
						if(!CopyFile( psz_filename, psz_filename_dest, false)) 
						{
							sprintf( msg, Lng->sGradientsText[3] + "%s" + Lng->sGradientsText[6], psz_filename_dest);
							MessageBox(m_hDialog, msg ,Lng->sGradientsText[7], MB_ICONERROR );
						} else
							MessageBox(m_hDialog, Lng->sGradientsText[8],Lng->sGradientsText[9],MB_OK);
						break;
					case ATMO_LOAD_GRADIENT_FILENOTFOND:   
						MessageBox(m_hDialog, Lng->sGradientsText[10],Lng->sGradientsText[7],MB_ICONERROR | MB_OK);
						break;
					case ATMO_LOAD_GRADIENT_FAILED_SIZE:   
						MessageBox(m_hDialog, Lng->sGradientsText[11],Lng->sGradientsText[7],MB_ICONERROR | MB_OK);
						break;
					case ATMO_LOAD_GRADIENT_FAILED_HEADER:   
						MessageBox(m_hDialog, Lng->sGradientsText[12],Lng->sGradientsText[7],MB_ICONERROR | MB_OK);
						break;
					case ATMO_LOAD_GRADIENT_FAILED_FORMAT:   
						MessageBox(m_hDialog, Lng->sGradientsText[13],Lng->sGradientsText[7],MB_ICONERROR | MB_OK);
						break;
					}
				}
				return(r); 
			}
		}
		MessageBox( m_hDialog, Lng->sGradientsText[14],Lng->sGradientsText[7],MB_ICONERROR | MB_OK);
	}
	return(r);
}
ATMO_BOOL CAtmoGradients::ExecuteCommand(HWND hControl,int wmId, int wmEvent) 
{
	for(int i=0; m_ZoneRadios && m_ZoneRadios[i]; i++) 
	{
		if( m_ZoneRadios[i] == hControl) 
		{
			if(wmEvent == BN_CLICKED) 
			{
				SetActiveZone( i );
				HWND groupBox = getDlgItem( IDC_ZONE_GROUP );
				InvalidateRect( groupBox, NULL, false);

			}
		}
	}

	switch(wmId) 
	{
	case IDOK: 
		{
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	case IDCANCEL: 
		{
			EndDialog(this->m_hDialog, wmId);
			break;
		}

	default:
		return ATMO_FALSE;

	}
	return ATMO_TRUE;
}


INT_PTR CALLBACK CAtmoGradients::GroupBoxProc(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CAtmoGradients *self;
	int wmId, wmEvent;
	HWND hControl;

	self = reinterpret_cast<CAtmoGradients *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (self) 
	{
		if(uMsg == WM_COMMAND) 
		{
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			hControl = (HWND)lParam;
			self->ExecuteCommand(hControl, wmId, wmEvent);
		}

		if(uMsg == WM_DROPFILES) 
		{
			INT_PTR result = CallWindowProc(self->OrgGroupBoxProc, hwnd, uMsg, wParam, lParam);
			self->HandleMessage( hwnd, uMsg, wParam, lParam);
			return(result);
		}

		if(uMsg == WM_PAINT) 
		{
			INT_PTR result = CallWindowProc(self->OrgGroupBoxProc, hwnd, uMsg, wParam, lParam);
			self->HandleMessage( hwnd, uMsg, wParam, lParam);
			return(result);
		}
		return CallWindowProc(self->OrgGroupBoxProc, hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
