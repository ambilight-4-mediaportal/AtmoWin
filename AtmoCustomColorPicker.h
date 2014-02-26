/*
 * AtmoCustomColorPicker.h: basic dialog for color selection - used only for inheritance to CAtmoColorPicker and CAtmoWhiteSetup
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#pragma once

#include "basicdialog.h"
#include "AtmoDynData.h"

class CAtmoCustomColorPicker :
    public CBasicDialog {

protected:
    CAtmoDynData *m_pAtmoDynData;
    int m_iRed;
    int m_iGreen;
    int m_iBlue;

	int m_gRed[11];
	int m_gGreen[11];
	int m_gBlue[11];
	int m_cRed[3];
	int m_cGreen[3];
    int m_cBlue[3];
	int m_cYellow[3];
	int m_cMagenta[3];
    int m_cCyan[3];
   

protected:
    virtual void outputColor(int red,int green,int blue);
    void UpdateColorControls(ATMO_BOOL sliders,ATMO_BOOL edits);

    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
    virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);
    void out_wb_color();


public:
    CAtmoCustomColorPicker(HINSTANCE hInst, HWND parent, WORD dlgRessourceID, CAtmoDynData *pAtmoDynData, int red, int green, int blue);
    virtual ~CAtmoCustomColorPicker(void);
};
