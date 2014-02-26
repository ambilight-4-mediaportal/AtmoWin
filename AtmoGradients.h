#pragma once
#include "basicdialog.h"
#include "AtmoConfig.h"
#include "AtmoConnection.h"
#include "AtmoTools.h"
#include "AtmoDynData.h"
#include "AtmoZoneDefinition.h"

class CAtmoGradients :
    public CBasicDialog
{
private:
    CAtmoDynData *m_pDynData;
    CAtmoConfig *m_pConfig;
    HWND *m_ZoneRadios;
    int m_active_zone;
    int m_edge_weight;
    HBITMAP m_current_gradient;

    WNDPROC OrgGroupBoxProc;

protected:
    static INT_PTR CALLBACK GroupBoxProc(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
    void SetActiveZone(int zone);


    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
    virtual ATMO_BOOL HandleMessage(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);

public:
    CAtmoGradients(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData);
    ~CAtmoGradients(void);

    static ATMO_BOOL Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData);
};
