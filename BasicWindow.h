#pragma once
#include "stdafx.h"
#include "AtmoDefs.h"

class CBasicWindow
{
protected:
      HWND m_hWindow;
      HWND m_hParentWindow;
      HINSTANCE m_hInst;
      ATOM m_aWndClass;
      ATMO_BOOL m_bAutoFree;

protected:
       static LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
       virtual void RegisterClass();
	   virtual void UnRegisterClass();

	   virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
       virtual LRESULT HandleWmCommand(HWND control, int wmId, int wmEvent);
       virtual void HandleWmDestroy();
       virtual void HandleWmPaint(PAINTSTRUCT ps,HDC hdc);

       virtual LPCTSTR ClassName() = 0;

       virtual void CreateWindowClass(WNDCLASSEX &wndclassex);
       virtual void BeforeDestroyWindow();

       virtual DWORD getWindowStyle();
       virtual SIZE getWindowSize();
       virtual POINT getWindowPos();

public:
    CBasicWindow(HINSTANCE hInst, HWND parent);
    virtual ~CBasicWindow(void);

    virtual void createWindow();

    virtual void hideWindow();
    virtual void showWindow(int cmdShow);
    virtual void closeWindow();
    virtual void invalidate(ATMO_BOOL erase);
    virtual void destroyWindow();
};
