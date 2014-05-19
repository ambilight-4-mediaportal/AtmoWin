/*
* AtmoColorPicker.h: Dialog for choosing a color by its components (red, green, blue) - with live preview on the hardware
*
* See the README file for copyright information and how to reach the author(s).
*
* $Id$
*/

#ifndef _AtmoColorPicker_h_
#define _AtmoColorPicker_h_

#include "basicdialog.h"
#include "AtmoDynData.h"
#include "AtmoCustomColorPicker.h"

class CAtmoColorPicker :  public CAtmoCustomColorPicker
{
protected:
	virtual void outputColor(int red,int green,int blue);
	virtual ATMO_BOOL InitDialog(WPARAM wParam);
	virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
	virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);

public:
	CAtmoColorPicker(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int red,int green,int blue);
	virtual ~CAtmoColorPicker(void);

	static ATMO_BOOL Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int &red, int &green, int &blue);

};

#endif