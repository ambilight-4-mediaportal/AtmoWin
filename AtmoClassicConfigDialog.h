#pragma once
#include "basicdialog.h"
#include "AtmoConfig.h"

class CAtmoClassicConfigDialog :
    public CBasicDialog
{
private:
    CAtmoConfig *m_pConfig;

private: // handles...
    HWND m_hCbxComports;
	HWND m_hCbxBaudrate;

protected:
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);

public:
    CAtmoClassicConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig);
    ~CAtmoClassicConfigDialog(void);
};


