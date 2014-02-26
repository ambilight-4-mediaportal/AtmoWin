#include "StdAfx.h"
#include ".\basicwindow.h"

CBasicWindow::CBasicWindow(HINSTANCE hInst,HWND parent) {
	this->m_hInst    = hInst;
    this->m_hParentWindow = parent;
    m_bAutoFree = ATMO_TRUE;
}


CBasicWindow::~CBasicWindow(void)
{
   // get nicht da hier virtual Functions nicht mehr korrekt aufgelöst werden ;)
   //  UnRegisterClass();
}

/*
LPCTSTR CBasicWindow::ClassName() {
       // return TEXT("YourWindowClassName");
        return NULL;
}
*/

void CBasicWindow::createWindow() {
    RegisterClass();
    POINT pos = getWindowPos();
    SIZE size = getWindowSize();
    CreateWindow(ClassName(), NULL, getWindowStyle(), pos.x, pos.x, size.cx, size.cy, m_hParentWindow, (HMENU)NULL, m_hInst, (LPVOID) this);
}

DWORD CBasicWindow::getWindowStyle()
{
  return WS_THICKFRAME;
}

SIZE CBasicWindow::getWindowSize()
{
  SIZE s;
  s.cx = CW_USEDEFAULT;
  s.cy = CW_USEDEFAULT;
  return s;
}

POINT CBasicWindow::getWindowPos()
{
  POINT p;
  p.x = CW_USEDEFAULT;
  p.y = CW_USEDEFAULT;
  return p;
}

void CBasicWindow::showWindow(int cmdShow) {
     ShowWindow(this->m_hWindow, cmdShow);
}

void CBasicWindow::hideWindow() {
     ShowWindow(this->m_hWindow, SW_HIDE);
}

void CBasicWindow::closeWindow() {
     CloseWindow(this->m_hWindow);
}

void CBasicWindow::destroyWindow() {
     closeWindow();
     DestroyWindow(this->m_hWindow);
}

void CBasicWindow::invalidate(ATMO_BOOL erase) {
     InvalidateRect(this->m_hWindow, NULL, erase);
}


void CBasicWindow::RegisterClass() {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)CBasicWindow::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInst;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= ClassName();
	wcex.hIconSm		= 0;

    CreateWindowClass(wcex);

	m_aWndClass = RegisterClassEx(&wcex);
}

void CBasicWindow::UnRegisterClass() {
    UnregisterClass(ClassName(), m_hInst);
}

void CBasicWindow::CreateWindowClass(WNDCLASSEX &wndclassex) {
     // Updae Properties of wndclassex as required...
}

/*
d:\CPP\AtmoWin\AtmoWinA\AtmoWinTrayIconHandler.cpp(98) : warning C4244: 'Argument': Konvertierung von 'LPARAM' in 'LONG', möglicher Datenverlust
d:\CPP\AtmoWin\AtmoWinA\AtmoWinTrayIconHandler.cpp(101) : warning C4312: 'reinterpret_cast': Konvertierung von 'LONG' in größeren Typ 'CTrayIconWindow *'
scheint ein bekanntes Problem zu sein - hab dazu mehrere Postings gefunden - ausser unterdrücken der beiden Warnings gibts wohl keine Lösung?
http://blog.strafenet.com/2006/09/08/getwindowlongptr/
*/


LRESULT CALLBACK CBasicWindow::WndProc(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam) {
	    CBasicWindow *self; // und täglich grüßt delphi!
		if (uMsg == WM_NCCREATE) {
		    LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
   		    self = reinterpret_cast<CBasicWindow *>(lpcs->lpCreateParams);
		    self->m_hWindow = hwnd;
		    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(self));
		} else {
		    self = reinterpret_cast<CBasicWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}
		if (self) {
		    return self->HandleMessage(uMsg, wParam, lParam);
		} else {
		    return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
}

LRESULT CBasicWindow::HandleWmCommand(HWND control, int wmId, int wmEvent) {
        // doDefault zeigt an das die Standard Behandlung durchgeführt werden soll..
    switch(wmId) {
         // ?
        case 0: //wegen warning
        default:
            return DefWindowProc(m_hWindow, WM_COMMAND, MAKELPARAM(wmId,wmEvent), (LPARAM)control);
    }
    return 0;
}
void CBasicWindow::HandleWmPaint(PAINTSTRUCT ps,HDC hdc) {
        // malen...
}
void CBasicWindow::HandleWmDestroy() {
     // PostQuitMessage(?)
}

void CBasicWindow::BeforeDestroyWindow() {
     UnRegisterClass();
}

LRESULT CBasicWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT lres;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
       case WM_NCDESTROY:
            lres = DefWindowProc(m_hWindow, message, wParam, lParam);
            SetWindowLongPtr(m_hWindow, GWLP_USERDATA, 0);
            BeforeDestroyWindow();
            if(m_bAutoFree == ATMO_TRUE) delete this;
            return lres;

	   case WM_COMMAND:
	  	    wmId    = LOWORD(wParam);
		    wmEvent = HIWORD(wParam);
            return HandleWmCommand((HWND)lParam, wmId, wmEvent);

	    case WM_PAINT:
		    hdc = BeginPaint(m_hWindow, &ps);
            HandleWmPaint(ps, hdc);
		    EndPaint(m_hWindow, &ps);
		    break;

	    case WM_DESTROY:
		    // führt indirekt zum freigeben dieser Klasseninstanz und zum Programmende! über WM_DESTROY!
		    HandleWmDestroy();
		    break;

	    default:
		    // für alle anderen Messages ... lass mal windows machen!
		    return DefWindowProc(m_hWindow, message, wParam, lParam);
	}
	return 0;
}
