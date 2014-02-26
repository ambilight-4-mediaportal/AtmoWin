#include "StdAfx.h"
#include "atmodummyconnection.h"

CAtmoDummyConnection::CAtmoDummyConnection(HINSTANCE hInst, CAtmoConfig *pAtmoConfig) : CAtmoConnection(pAtmoConfig), CBasicWindow(hInst,(HWND)NULL)
{
     m_IsOpen = ATMO_FALSE;
     m_bAutoFree = ATMO_FALSE;

     m_CurrentValues = NULL;
}

CAtmoDummyConnection::~CAtmoDummyConnection(void)
{
    delete (char *)m_CurrentValues;
}


void CAtmoDummyConnection::CreateWindowClass(WNDCLASSEX &wndclassex) {
}

#define stripe_size 75
#define bordersize  5

int CAtmoDummyConnection::FindChannel(int zone)
{
  if(m_ChannelAssignment) {
      for(int i = 0 ; i < m_NumAssignedChannels; i++) 
      {
         if(m_ChannelAssignment[i] == zone)
            return i;
      }
  }
  return -1;
}

void CAtmoDummyConnection::HandleWmPaint(PAINTSTRUCT ps,HDC hdc) {
    RECT rect;
    RECT output;
    int x,y,x1,y1,x2,y2,wTop,wBottom, w, h, z;

    char zonetext[30];

    HPEN oldPen;
    HBRUSH oldBrush;

    Lock();

    if(!m_CurrentValues) {
       Unlock();
       return;
    }

    int HorzTop       = m_pAtmoConfig->getZonesTopCount();
    int HorzBottom    = m_pAtmoConfig->getZonesBottomCount();
    int Vert          = m_pAtmoConfig->getZonesLRCount();
    ATMO_BOOL sumZone = m_pAtmoConfig->getZoneSummary();

    GetClientRect(this->m_hWindow,&rect);

    w = (rect.right - rect.left);
    h = (rect.bottom - rect.top);

    w = w - (2*bordersize)-(2*stripe_size); // w = restbreite!
    h = h - (2*bordersize)-(2*stripe_size); // h = resthöhe!

    if(HorzTop)
       wTop = w / HorzTop; // grösse eines Stripes
    else
       wTop = w;  

    if(HorzBottom) 
       wBottom = w / HorzBottom;
    else
       wBottom = w;  

    if(Vert)
       h = h / Vert;

    oldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0,0,0)));

    z = 0;
    // oben von links nach rechts
    for(x=0;x<HorzTop;x++) 
    {
        x1 = bordersize + stripe_size + (x * wTop);
        x2 = x1 + wTop;
        y1 = bordersize;
        y2 = y1 + stripe_size; 
        output.left   = x1;
        output.right  = x2;
        output.top    = y1;
        output.bottom = y2;

        if(z < m_CurrentValues->numColors)
        {
          oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(m_CurrentValues->zone[z].r, m_CurrentValues->zone[z].g, m_CurrentValues->zone[z].b)));
          Rectangle(hdc, x1, y1, x2, y2);
          DeleteObject(SelectObject(hdc, oldBrush));
        }

        sprintf(zonetext, "Z:%d,C:%d", z, FindChannel(z));
        DrawText(hdc, zonetext, -1, &output, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
        z++;
    }

    // rechter Rand von oben nach unten
    for(y=0;y<Vert;y++) 
    {
        x1 = rect.right - bordersize - stripe_size;
        x2 = x1 + stripe_size;
        y1 = bordersize + stripe_size + (y * h);
        y2 = y1 + h;
        output.left   = x1;
        output.right  = x2;
        output.top    = y1;
        output.bottom = y2;

        if(z < m_CurrentValues->numColors)
        {
          oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(m_CurrentValues->zone[z].r, m_CurrentValues->zone[z].g, m_CurrentValues->zone[z].b)));
          Rectangle(hdc, x1, y1, x2, y2);
          DeleteObject(SelectObject(hdc, oldBrush));
        }

        sprintf(zonetext, "Z:%d,C:%d", z, FindChannel(z));
        DrawText(hdc, zonetext, -1, &output, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        z++;
    }

    // unten von rechts nach links
    for(x=(HorzBottom-1);x>=0;x--) 
    {
        x1 = bordersize + stripe_size + (x * wBottom);
        x2 = x1 + wBottom;
        y1 = rect.bottom - bordersize - stripe_size;
        y2 = y1 + stripe_size; 
        output.left   = x1;
        output.right  = x2;
        output.top    = y1;
        output.bottom = y2;

        if(z < m_CurrentValues->numColors)
        {
          oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(m_CurrentValues->zone[z].r, m_CurrentValues->zone[z].g, m_CurrentValues->zone[z].b)));
          Rectangle(hdc, x1, y1, x2, y2);
          DeleteObject(SelectObject(hdc, oldBrush));
        }

        sprintf(zonetext, "Z:%d,C:%d", z, FindChannel(z));
        DrawText(hdc, zonetext, -1, &output, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        z++;
    }
    // linker Rand von unten nach oben
    for(y=(Vert-1);y>=0;y--) 
    {
        x1 = bordersize;
        x2 = x1 + stripe_size;
        y1 = bordersize + stripe_size + (y * h);
        y2 = y1 + h;
        output.left   = x1;
        output.right  = x2;
        output.top    = y1;
        output.bottom = y2;

        if(z < m_CurrentValues->numColors)
        {
          oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(m_CurrentValues->zone[z].r, m_CurrentValues->zone[z].g, m_CurrentValues->zone[z].b)));
          Rectangle(hdc, x1, y1, x2, y2);
          DeleteObject(SelectObject(hdc, oldBrush));
        }

        sprintf(zonetext, "Z:%d,C:%d", z, FindChannel(z));
        DrawText(hdc, zonetext, -1, &output, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        z++;
    }

    if(sumZone)
    {
        x1 = (rect.right - rect.left) / 2 - stripe_size/2;
        x2 = x1 + stripe_size;
        y1 = (rect.bottom - rect.top) / 2 - stripe_size/2;
        y2 = y1 + stripe_size;
        output.left   = x1;
        output.right  = x2;
        output.top    = y1;
        output.bottom = y2;

        if(z < m_CurrentValues->numColors)
        {
          oldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(m_CurrentValues->zone[z].r, m_CurrentValues->zone[z].g, m_CurrentValues->zone[z].b)));
          Rectangle(hdc, x1, y1, x2, y2);
          DeleteObject(SelectObject(hdc, oldBrush));
        }

        sprintf(zonetext, "Z:%d,C:%d", z, FindChannel(z));
        DrawText(hdc, zonetext, -1, &output, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    DeleteObject(SelectObject(hdc, oldPen));

    Unlock();
}

LPCTSTR CAtmoDummyConnection::ClassName() {
        return TEXT("AtmoDummyConnectionClass");
}

ATMO_BOOL CAtmoDummyConnection::OpenConnection() {
    if(m_IsOpen == ATMO_FALSE)
       this->createWindow();
    this->showWindow(SW_SHOW);
    SetWindowPos(this->m_hWindow,(HWND)NULL,0,0,400,300,SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOREPOSITION | SWP_NOZORDER);

    char buf[200];
    sprintf(buf,"AtmoLight Dummy Interface... (%d x %d Edition)", CAP_WIDTH, CAP_HEIGHT );

    SetWindowText(this->m_hWindow, buf );
    m_IsOpen = ATMO_TRUE;
    return ATMO_TRUE;
}

void CAtmoDummyConnection::CloseConnection() {
    this->destroyWindow();
    m_IsOpen = ATMO_FALSE;
}

ATMO_BOOL CAtmoDummyConnection::isOpen(void) {
     return m_IsOpen;
}

ATMO_BOOL CAtmoDummyConnection::SendData(pColorPacket data) {
     Lock();
     delete (char *)m_CurrentValues; 
     DupColorPacket(m_CurrentValues, data);
     Unlock();
     invalidate(ATMO_FALSE);
     return ATMO_TRUE;
}

ATMO_BOOL CAtmoDummyConnection::HardwareWhiteAdjust(int global_gamma, int global_contrast, int contrast_red, int contrast_green, int contrast_blue, int gamma_red, int gamma_green, int gamma_blue, ATMO_BOOL storeToEeprom) {
   // not implemented!
   return ATMO_TRUE;
}

ATMO_BOOL CAtmoDummyConnection::CreateDefaultMapping(CAtmoChannelAssignment *ca)
{
   // do not modify the current default channel mapping, to show the current channel mapping on the screen!
   if(ca && (ca->getSize() == 0)) {
       // doch zu einer Untat hinreissen lassen?
   }
   return ATMO_TRUE;
}

char *CAtmoDummyConnection::getChannelName(int ch)
{
  if(ch < 0) return NULL;
  char buf[30];
  sprintf(buf,"Kanal [%d]",ch);
  return strdup(buf);
}

int CAtmoDummyConnection::getNumChannels()
{
  return m_pAtmoConfig->getZoneCount();
}