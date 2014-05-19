/*
* AtmoDisplays.h: class for retrieving the current monitor and display setup... can handle up to four different screens on a computer
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#ifndef _AtmoDisplays_h_
#define _AtmoDisplays_h_

#include "AtmoDefs.h"

typedef struct {
	int horz_res;
	int vert_res;
	int horz_ScreenPos;
	int vert_ScreenPos;
	char infoText[255];
} TAtmoDisplayInfo;

class CAtmoDisplays
{
protected:
	int m_DisplayCount;
	TAtmoDisplayInfo m_Displays[16];
public:
	CAtmoDisplays(void);
	~CAtmoDisplays(void);
	void ReloadList();
	int getCount();
	TAtmoDisplayInfo getDisplayInfo(int displayNr);
};

#endif