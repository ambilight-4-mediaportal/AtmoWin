#pragma once
#include "basicwindow.h"

class CAtmoPacketQueueStatus :   public CBasicWindow
{
protected:
    int m_waitcounter;
    int m_skipcounter;
    int m_framecounter;
    int m_nullpackets;
    DWORD m_avgWait;
    DWORD m_avgDelay;
    ATMO_BOOL first;

protected:
    void CreateWindowClass(WNDCLASSEX &wndclassex);
    LPCTSTR ClassName();
    void HandleWmPaint(PAINTSTRUCT ps,HDC hdc);

    virtual DWORD getWindowStyle();
    virtual SIZE getWindowSize();
    virtual POINT getWindowPos();
    
public:
    CAtmoPacketQueueStatus(HINSTANCE hInst,HWND parent);
    ~CAtmoPacketQueueStatus(void);

    virtual void showWindow(int cmdShow);

    void UpdateValues( int waits, int skips, int frames, int nullpackets, DWORD avgWait, DWORD avgDelay);
};
