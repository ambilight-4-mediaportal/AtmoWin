#pragma once
#include "basicdialog.h"
#include "AtmoConfig.h"

class CDmxConfigDialog :
    public CBasicDialog
{
private:
    CAtmoConfig *m_pConfig;

private: // handles...
    HWND m_hCbxComports;
    HWND m_hCbxBaudrate;
    int m_MaxChannels;

protected:
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);

public:
    CDmxConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig);
    ~CDmxConfigDialog(void);
};


