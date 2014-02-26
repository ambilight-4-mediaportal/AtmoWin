/*
 * AtmoWhiteAdjHW.h: Dialog for doing the white calibration of the LED Stripes in the hardware
 * works only if your controllers are flashed with the most up to data software!
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#pragma once

#include "basicdialog.h"
#include "AtmoDynData.h"

class CAtmoWhiteAdjHW :  public CBasicDialog {

protected:
    CAtmoDynData *m_pAtmoDynData;

    int m_Hardware_global_gamma;
    int m_Hardware_global_contrast;
    int m_Hardware_contrast_red;
    int m_Hardware_contrast_green;
    int m_Hardware_contrast_blue;
    int m_Hardware_gamma_red;
    int m_Hardware_gamma_green;
    int m_Hardware_gamma_blue;
    ATMO_BOOL m_SaveToEeprom;


protected:
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);

private:
    void UpdateHardware();

public:
    CAtmoWhiteAdjHW(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData);
    virtual ~CAtmoWhiteAdjHW(void);

    static ATMO_BOOL Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, ATMO_BOOL &saveLaterToHardware);
};
