#pragma once
#include "basicdialog.h"
#include "atmodyndata.h"
#include "atmodefs.h"

class CAtmoEditChannelAssignment :
    public CBasicDialog
{
private:
    CAtmoDynData *m_pDynData;
    int m_iChCount;

    HWND *m_pZoneBoxes;
    HWND *m_pChannelNames;

    HWND m_hScrollbox;
    int m_MaxScrollPos;
    int m_ScrollPos;
protected:
    void RealignComboboxes(int startPos);
    void EditAssignment(CAtmoChannelAssignment *ca);
    void SaveAssignment(CAtmoChannelAssignment *ca);

protected:
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
    virtual void HandleVertScroll(int code,int position,HWND scrollBarHandle);

    void CleanupListbox();

public:
    CAtmoEditChannelAssignment(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData);
    virtual ~CAtmoEditChannelAssignment(void);
};
