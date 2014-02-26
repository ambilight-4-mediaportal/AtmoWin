/*
 * AtmoColorChanger.cpp: class for the fun effect: random color fading on all five channels
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#include "StdAfx.h"
#include "atmocolorchanger.h"

CAtmoColorChanger::CAtmoColorChanger(CAtmoConnection *atmoConnection, CAtmoConfig *atmoConfig) : CAtmoBasicEffect(atmoConnection,atmoConfig) {
}

CAtmoColorChanger::~CAtmoColorChanger(void)
{
}



DWORD CAtmoColorChanger::Execute(void)
{
	pColorPacket RGB_aus;
    AllocColorPacket(RGB_aus, this->m_AtmoConfig->getZoneCount());

	while(!this->m_bTerminated) {
		int rot  = rand() % 255 + 1;	// zufällige Farbe
		int gruen = rand() % 255 + 1;
		int blau = rand() % 255 + 1;
        for (int i=0; i< RGB_aus->numColors ; i++)  // 0..4
		{
            RGB_aus->zone[i].r = rot;		
			RGB_aus->zone[i].g = gruen;
			RGB_aus->zone[i].b = blau;
		}
        // Fade to new Random color ... starting value will be the last Color ... used by this thread...
        Interpolation(m_AtmoConfig->getColorChanger_iSteps(), m_AtmoConfig->getColorChanger_iDelay(), RGB_aus );	
	}

    delete (char *)RGB_aus;

	return 0;
}
