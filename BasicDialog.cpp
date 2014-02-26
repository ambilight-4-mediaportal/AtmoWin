/*
 * BasicDialog.cpp: Baseclass of all dialogs inside atmo win - makes the dealing with native windows dialogs a little bit easier
 *
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#include "StdAfx.h"
#include "basicdialog.h"

CBasicDialog::CBasicDialog(HINSTANCE hinst, WORD ressourceID, HWND parentWindow) {
    this->m_hInst = hinst;
    this->m_DialogRessourceID = ressourceID;
    this->m_hParentWindow = parentWindow;
}

CBasicDialog::~CBasicDialog(void)
{
}

ATMO_BOOL CBasicDialog::SetupSliderControl(int dlgItemId, int min, int max, int position, int ticFrq) {
    HWND hwndCtrl = getDlgItem(dlgItemId);
    if(!hwndCtrl) return ATMO_FALSE;

//    SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, min);
//    SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, max);
    SendMessage(hwndCtrl, TBM_SETRANGE, 0, MAKELONG(min,max));

    if(ticFrq>0)
      SendMessage(hwndCtrl, TBM_SETTICFREQ, ticFrq, 0);

//#include    "commctrl.h"
    SendMessage(hwndCtrl, TBM_SETPOS, 1, position);

    return ATMO_TRUE;
}

void CBasicDialog::SetEditInt(int dlgItemId, int value) {
     SetEditInt(getDlgItem(dlgItemId), value);
}

ATMO_BOOL CBasicDialog::GetEditInt(int dlgItemId,int &value) {
     return GetEditInt(getDlgItem(dlgItemId), value);
}

void CBasicDialog::SetEditDouble(int dlgItemId, double value) {
     SetEditDouble(getDlgItem(dlgItemId), value);
}

ATMO_BOOL CBasicDialog::GetEditDouble(int dlgItemId, double &value) {
     return GetEditDouble(getDlgItem(dlgItemId), value);
}

void CBasicDialog::SetEditInt(HWND dlgItemHandle, int value) {
     char buf[200];
     if(!dlgItemHandle) return;
     sprintf(buf,"%d",value);
     Edit_SetText(dlgItemHandle, buf);
}

ATMO_BOOL CBasicDialog::GetEditInt(HWND dlgItemHandle,int &value) {
     char buf[200];
     if(!dlgItemHandle) return ATMO_FALSE;
     if(Edit_GetText(dlgItemHandle, buf, 200)<=0) return ATMO_FALSE;
     value = atoi(buf);
     return ATMO_TRUE;
}

void CBasicDialog::SetEditDouble(HWND dlgItemHandle, double value) {
     char buf[200];
     if(!dlgItemHandle) return;
     sprintf(buf,"%.2f",value);
     Edit_SetText(dlgItemHandle, buf);
}

ATMO_BOOL CBasicDialog::GetEditDouble(HWND dlgItemHandle, double &value) {
     char buf[200];
     if(!dlgItemHandle) return ATMO_FALSE;
     if(Edit_GetText(dlgItemHandle, buf, 200)<=0) return ATMO_FALSE;
     value = atof(buf);
     return ATMO_TRUE;
}

void CBasicDialog::SetStaticText(int dlgItemId, char *text) {
     HWND hwndCtrl = getDlgItem(dlgItemId);
     if(!hwndCtrl) return;
     SetWindowText(hwndCtrl, text);
}


INT_PTR CBasicDialog::ShowModal() {
    return DialogBoxParam(m_hInst, MAKEINTRESOURCE(m_DialogRessourceID), m_hParentWindow, (DLGPROC)CBasicDialog::DialogProc, (LPARAM)this);
}

HWND CBasicDialog::getDlgItem(int dlgID) {
    return GetDlgItem(this->m_hDialog, dlgID);
}

ATMO_BOOL CBasicDialog::InitDialog(WPARAM wParam) {
        return ATMO_FALSE;
}
ATMO_BOOL CBasicDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {
        return ATMO_FALSE;
}

void CBasicDialog::HandleHorzScroll(int code,int position,HWND scrollBarHandle) {
}
void CBasicDialog::HandleVertScroll(int code,int position,HWND scrollBarHandle) {
}

ATMO_BOOL CBasicDialog::HandleMessage(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;
    HWND hControl;

    switch(uMsg) {
       case WM_INITDIALOG:
            return InitDialog(wParam);

       case WM_COMMAND: {
	  	    wmId    = LOWORD(wParam);
		    wmEvent = HIWORD(wParam);
            hControl = (HWND)lParam;
            return ExecuteCommand(hControl, wmId, wmEvent);
       }

       case WM_HSCROLL: {
            HandleHorzScroll(LOWORD(wParam),HIWORD(wParam),(HWND)lParam);
            break;
       }

       case WM_VSCROLL: {
            HandleVertScroll(LOWORD(wParam),HIWORD(wParam),(HWND)lParam);
            break;
       }

       case WM_NCDESTROY:
            SetWindowLongPtr(this->m_hDialog, GWLP_USERDATA, 0);
            // delete this;
            break;

       default:
            return ATMO_FALSE;
	}

    return ATMO_FALSE;
}


INT_PTR CALLBACK CBasicDialog::DialogProc(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam) {
    CBasicDialog *self; // und täglich grüßt delphi!
    if(uMsg == WM_INITDIALOG) {
       self = (CBasicDialog *)lParam;
       self->m_hDialog = hwnd;
       SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(self));
    } else {
       self = reinterpret_cast<CBasicDialog *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (self) {
        if(self->HandleMessage(hwnd, uMsg, wParam, lParam) == ATMO_TRUE)
	       return TRUE;
        else
           return FALSE;
    } else {
        return FALSE;
    }
}

void InitDialog_ComPorts(HWND cbxComPort)
{
   char comport[32];
   int i;
   for(i=1;i<=64;i++) {
       sprintf(comport,"com%d",i);
       ComboBox_AddString( cbxComPort, comport );
   }
}
