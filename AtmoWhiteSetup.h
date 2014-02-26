/*
 * AtmoWhiteSetup.h: Dialog for doing the white calibration of the LED Stripes in the software
 * works with all versions of the hardware but consumes some CPU ressources
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#pragma once

#include "atmocustomcolorpicker.h"
#include "atmodyndata.h"

class CAtmoWhiteSetup :  public CAtmoCustomColorPicker
{
protected:
    virtual void outputColor(int red,int green,int blue);
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
    virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);

protected:
    ATMO_BOOL m_UseSoftware;
	ATMO_BOOL m_UseSoftware2;
	ATMO_BOOL m_UseColorK;
	ATMO_BOOL m_Use3dlut;
	ATMO_BOOL m_Useinvert;

    int m_Global_Gamma;
    int m_Gamma_Red;
    int m_Gamma_Green;
    int m_Gamma_Blue;
    AtmoGammaCorrect m_GammaCorrect;

public:
    CAtmoWhiteSetup(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int red,int green,int blue,ATMO_BOOL useSoftware,ATMO_BOOL useSoftware2,ATMO_BOOL useColorK, ATMO_BOOL use3dlut, ATMO_BOOL useinvert);
    virtual ~CAtmoWhiteSetup(void);

    static ATMO_BOOL Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int &red, int &green, int &blue, ATMO_BOOL &useSoftware, ATMO_BOOL &useSoftware2, ATMO_BOOL &useColorK, ATMO_BOOL &use3dlut, ATMO_BOOL &useinvert);
};
