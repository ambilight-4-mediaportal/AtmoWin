/*
* AtmoDisplays.cpp: class for retrieving the current monitor and display setup... can handle up to four different screens on a computer
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/

#include "StdAfx.h"
#include "atmodisplays.h"

CAtmoDisplays::CAtmoDisplays(void)
{
	ReloadList();
}

CAtmoDisplays::~CAtmoDisplays(void)
{
}

void CAtmoDisplays::ReloadList() 
{
	m_DisplayCount = 0;
	ZeroMemory(&m_Displays,sizeof(TAtmoDisplayInfo) * 4);

	// enum Displays?
	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(displayDevice);
	DWORD dev = 0;

	// enumerate through all displays...
	while (EnumDisplayDevices(0, dev, &displayDevice, 0))	 
	{
		if (!(displayDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
		{
			DISPLAY_DEVICE ddMon;
			ZeroMemory(&ddMon, sizeof(ddMon));
			ddMon.cb = sizeof(ddMon);

			DWORD devMon = 0;
			while (EnumDisplayDevices(displayDevice.DeviceName, devMon, &ddMon, 0))  
			{
				if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE) break;
				devMon++;
			}

			DEVMODE dm;
			ZeroMemory(&dm, sizeof(dm));
			dm.dmSize = sizeof(dm);
			if (EnumDisplaySettingsEx(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &dm, 0) == ATMO_FALSE)
				EnumDisplaySettingsEx(displayDevice.DeviceName, ENUM_REGISTRY_SETTINGS, &dm, 0);

			m_Displays[m_DisplayCount].horz_res = dm.dmPelsWidth;
			m_Displays[m_DisplayCount].vert_res = dm.dmPelsHeight;
			m_Displays[m_DisplayCount].horz_ScreenPos = dm.dmPosition.x;
			m_Displays[m_DisplayCount].vert_ScreenPos = dm.dmPosition.y;
			sprintf(m_Displays[m_DisplayCount].infoText,"%s (%d x %d)", ddMon.DeviceString, 
				m_Displays[m_DisplayCount].horz_res, m_Displays[m_DisplayCount].vert_res);

			m_DisplayCount++;
		}
		dev++;
	}
}

int CAtmoDisplays::getCount() 
{
	return m_DisplayCount;
}

TAtmoDisplayInfo CAtmoDisplays::getDisplayInfo(int displayNr) 
{
	return  m_Displays[displayNr];
}
