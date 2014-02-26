#include "StdAfx.h"
#include "atmopacketqueuestatus.h"

CAtmoPacketQueueStatus::CAtmoPacketQueueStatus(HINSTANCE hInst,HWND parent) : CBasicWindow(hInst, parent)
{
    first  = ATMO_TRUE;
}

CAtmoPacketQueueStatus::~CAtmoPacketQueueStatus(void)
{
}

LPCTSTR CAtmoPacketQueueStatus::ClassName()
{
  return TEXT("AtmoPacketQueueStatusClass");
}

DWORD CAtmoPacketQueueStatus::getWindowStyle()
{
  return (WS_BORDER | WS_CAPTION);
}

SIZE CAtmoPacketQueueStatus::getWindowSize()
{
  SIZE s;
  s.cx = 200;
  s.cy = 50;
  return s;
}

POINT CAtmoPacketQueueStatus::getWindowPos()
{
  POINT p;
  p.x = 0;
  p.y = 0;
  return p;
}

void CAtmoPacketQueueStatus::showWindow(int cmdShow)
{
    CBasicWindow::showWindow(cmdShow);
    SetWindowText(this->m_hWindow,"Queue Status");
}

void CAtmoPacketQueueStatus::UpdateValues( int waits, int skips, int frames, int nullpackets, DWORD avgWait, DWORD avgDelay)
{
    this->m_avgDelay = avgDelay;
    this->m_avgWait = avgWait;
    this->m_waitcounter = waits;
    this->m_framecounter = frames;
    this->m_skipcounter = skips;
    this->m_nullpackets = nullpackets;
    invalidate(ATMO_TRUE);
}

void CAtmoPacketQueueStatus::HandleWmPaint(PAINTSTRUCT ps,HDC hdc)
{
  RECT r;
  SIZE size;
  char buf[100];
  
  GetClientRect(m_hWindow, &r);
  r.left += 5;
  r.top +=4;

  sprintf(buf,"Processed: %d",m_framecounter);
  TextOut(hdc, r.left, r.top, buf, (int)strlen(buf));
  GetTextExtentPoint32(hdc, buf, (int)strlen(buf), &size);
  r.top += size.cy + 1;

  if(m_waitcounter > 0)
     sprintf(buf,"Waits: %d, AvgWaits: %d ms",m_waitcounter, m_avgWait/m_waitcounter );
  else
     sprintf(buf,"Waits: %d, AvgWaits: %d ms",m_waitcounter, 0 );
  TextOut(hdc, r.left, r.top, buf, (int)strlen(buf));
  GetTextExtentPoint32(hdc, buf, (int)strlen(buf), &size);
  r.top += size.cy + 1;

  if(m_waitcounter > 0)
     sprintf(buf,"Skips: %d, AvgDelay: %d ms", m_waitcounter, m_avgDelay / m_waitcounter );
  else
     sprintf(buf,"Skips: %d, AvgDelay: %d ms", m_waitcounter, 0 );
  TextOut(hdc, r.left, r.top, buf, (int)strlen(buf));
  GetTextExtentPoint32(hdc, buf, (int)strlen(buf), &size);
  r.top += size.cy + 1;

  sprintf(buf,"RepeatPackets: %d", this->m_nullpackets );
  TextOut(hdc, r.left, r.top, buf, (int)strlen(buf));
  GetTextExtentPoint32(hdc, buf, (int)strlen(buf), &size);
  r.top += size.cy + 1;

  // first
  if(first) {
     first = ATMO_FALSE;
     SetWindowPos(this->m_hWindow, 0, 0, 0, r.right + 60, r.top + 30, SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOREPOSITION | SWP_NOSENDCHANGING | SWP_NOZORDER);
  }

}

void CAtmoPacketQueueStatus::CreateWindowClass(WNDCLASSEX &wndclassex) {
     // Updae Properties of wndclassex as required...
}
