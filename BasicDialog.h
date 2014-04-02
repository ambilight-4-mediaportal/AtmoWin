/*
 * BasicDialog.h: Baseclass of all dialogs inside atmo win - makes the dealing with native windows dialogs a little bit easier
 *
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

//#pragma once
#ifndef _BASICDIALOG_h_
#define _BASICDIALOG_h_

#include "StdAfx.h"
#include "AtmoDefs.h"

void InitDialog_ComPorts(HWND cbxComPort);

class CBasicDialog {
  protected:
    HINSTANCE m_hInst;
    WORD m_DialogRessourceID;
    HWND m_hParentWindow;
    HWND m_hDialog;
protected:
    static INT_PTR CALLBACK DialogProc(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    virtual ATMO_BOOL HandleMessage(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual ATMO_BOOL InitDialog(WPARAM wParam);

    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);

    virtual void HandleHorzScroll(int code,int position,HWND scrollBarHandle);
    virtual void HandleVertScroll(int code,int position,HWND scrollBarHandle);

    ATMO_BOOL SetupSliderControl(int dlgItemId, int min, int max, int position, int ticFrq);
    void SetStaticText(int dlgItemId, char *text);

    void SetEditInt(int dlgItemId, int value);
    ATMO_BOOL GetEditInt(int dlgItemId,int &value);

    void SetEditDouble(int dlgItemId, double value);
    ATMO_BOOL GetEditDouble(int dlgItemId, double &value);

    void SetEditInt(HWND dlgItemHandle, int value);
    ATMO_BOOL GetEditInt(HWND dlgItemHandle,int &value);
    void SetEditDouble(HWND dlgItemHandle, double value);
    ATMO_BOOL GetEditDouble(HWND dlgItemHandle, double &value);

    HWND getDlgItem(int dlgID);

public:
    CBasicDialog(HINSTANCE hinst, WORD ressourceID, HWND parentWindow);
    virtual ~CBasicDialog(void);

    virtual INT_PTR ShowModal();
};

#endif