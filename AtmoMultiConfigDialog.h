#ifndef _AtmoMultiConfigDialog_h_
#define _AtmoMultiConfigDialog_h_

#include "BasicDialog.h"
#include "AtmoConfig.h"

class CAtmoMultiConfigDialog :
	public CBasicDialog
{
private:
	CAtmoConfig *m_pConfig;

private: // handles...
	HWND m_hCbxComports[4];

protected:
	virtual ATMO_BOOL InitDialog(WPARAM wParam);
	virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);

public:
	CAtmoMultiConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig);
	~CAtmoMultiConfigDialog(void);
};

#endif