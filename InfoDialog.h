/*
 * infodialog.h: a simple info dialog showing who we are *g*
 *
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#ifndef _InfoDialog_h_
#define _InfoDialog_h_

#include "basicdialog.h"

class CInfoDialog :
    public CBasicDialog
{
protected:
   virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);
   virtual ATMO_BOOL InitDialog(WPARAM wParam);


public:
    CInfoDialog(HINSTANCE hinst, HWND parentWindow);
    virtual ~CInfoDialog(void);
};

#endif