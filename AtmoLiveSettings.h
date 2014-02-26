#pragma once

#include "basicdialog.h"
#include "AtmoDynData.h"
#include "AtmoConfig.h"

class CAtmoLiveSettings :
    public CBasicDialog
{
private:
    CAtmoDynData *m_pDynData;
    CAtmoConfig *m_pBackupConfig; 

protected:
    void LoadDisplayList();
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);
    virtual void HandleVertScroll(int code,int position,HWND scrollBarHandle);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
    ATMO_BOOL UpdateLiveViewValues(ATMO_BOOL showPreview);

public:
    CAtmoLiveSettings(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData);
    ~CAtmoLiveSettings(void);
};
