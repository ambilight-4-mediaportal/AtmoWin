/*
 * infodialog.cpp: a simple info dialog showing who we are *g*
 *
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#include "StdAfx.h"
#include "resource.h"
#include "atmodefs.h"
#include "infodialog.h"

CInfoDialog::CInfoDialog(HINSTANCE hinst, HWND parentWindow ) : CBasicDialog(hinst,IDD_ABOUTBOX,parentWindow) {
}

CInfoDialog::~CInfoDialog(void)
{
}

ATMO_BOOL CInfoDialog::InitDialog(WPARAM wParam)
{
    char title[100],buf[200];
    GetWindowText(this->m_hDialog, title, sizeof(title));

    sprintf(buf,"%s (%d x %d Edition)", title, CAP_WIDTH, CAP_HEIGHT);
    
    SetWindowText( this->m_hDialog, buf);
    return CBasicDialog::InitDialog(wParam);
}


ATMO_BOOL CInfoDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {
  switch(wmId) {
        case IDOK: {
           EndDialog(this->m_hDialog, wmId);
           return ATMO_TRUE;
        }

        case IDCANCEL: {
           EndDialog(this->m_hDialog, wmId);
           return ATMO_TRUE;
        }
  }
  return ATMO_FALSE;
}

