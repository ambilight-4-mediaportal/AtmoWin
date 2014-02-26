#pragma once
#include "basicdialog.h"
#include "AtmoConfig.h"

class CMoMoConfigDialog :
    public CBasicDialog
{
private:
    CAtmoConfig *m_pConfig;

private: // handles...
    HWND m_hCbxComports;
    HWND m_hCbxDeviceType;

protected:
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);

public:
    CMoMoConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig);
    ~CMoMoConfigDialog(void);
};


