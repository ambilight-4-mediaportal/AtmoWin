#pragma once
#include "basicdialog.h"
#include "AtmoConfig.h"

class CFnordlichtConfigDialog :
    public CBasicDialog
{
private:
    CAtmoConfig *m_pConfig;

private: // handles...
    HWND m_hCbxComports;
    HWND m_hEditNumChannels;

protected:
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);

public:
    CFnordlichtConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig);
    ~CFnordlichtConfigDialog(void);
};


