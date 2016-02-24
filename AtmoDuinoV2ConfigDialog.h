#ifndef _AtmoDuinoV2ConfigDialog_h_
#define _AtmoDuinoV2ConfigDialog_h_

#include "basicdialog.h"
#include "AtmoConfig.h"

class CAtmoDuinoV2ConfigDialog :
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
    CAtmoDuinoV2ConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig);
    ~CAtmoDuinoV2ConfigDialog(void);
};

#endif
