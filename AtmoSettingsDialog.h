/*
* AtmoSettingsDialog.h: Setup Dialog for Atmo Win
*
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#ifndef _AtmoSettingsDialog_h_
#define _AtmoSettingsDialog_h_

#include "basicdialog.h"
#include "AtmoDynData.h"
#include "AtmoConfig.h"

class CAtmoSettingsDialog :  public CBasicDialog
{
private:
	CAtmoDynData *m_pDynData;
	CAtmoConfig *m_pBackupConfig;
	EffectMode m_eTempEffectMode;
private: // handles...
	HWND m_hCbxDevicetypes;
	HWND m_hCbxEffects;

	ATMO_BOOL m_SaveHardWhiteSetup;

protected:
	virtual ATMO_BOOL InitDialog(WPARAM wParam);
	virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
	virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);
	virtual void HandleVertScroll(int code,int position,HWND scrollBarHandle);

	ATMO_BOOL UpdateColorChangeValues(ATMO_BOOL showPreview);
	ATMO_BOOL UpdateLrColorChangeValues(ATMO_BOOL showPreview);
	ATMO_BOOL UpdateLiveViewValues(ATMO_BOOL showPreview);

	void LoadDisplayList();
	void DeleteAllChannel();
	void UpdateDeviceConnection(AtmoConnectionType conType);

public:
	CAtmoSettingsDialog(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData);
	virtual ~CAtmoSettingsDialog(void);
};

#endif