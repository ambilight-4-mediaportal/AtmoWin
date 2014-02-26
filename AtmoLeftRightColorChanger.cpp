/*
 * AtmoLeftRightColorChanger.cpp:  a fun effect - which fades a color from the left channel to the right channel, then
 *    takes the next random color and starts again...
 *
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#include "StdAfx.h"
#include "atmoleftrightcolorchanger.h"

CAtmoLeftRightColorChanger::CAtmoLeftRightColorChanger(CAtmoConnection *atmoConnection, CAtmoConfig *atmoConfig) : CAtmoBasicEffect(atmoConnection, atmoConfig)
{
}

CAtmoLeftRightColorChanger::~CAtmoLeftRightColorChanger(void)
{
}

DWORD CAtmoLeftRightColorChanger::Execute(void)
{
	pColorPacket RGB_aus;
    AllocColorPacket(RGB_aus, m_AtmoConfig->getZoneCount());
    ZeroColorPacket(RGB_aus);

    int lrc = m_AtmoConfig->getZonesLRCount();

    if(lrc == 0)
    {
        while(!this->m_bTerminated) {
              this->ThreadSleep(200);
        };
        return 0;
    }

    int *left_zones = new int[ lrc ];
    int *right_zones = new int[ lrc ];

    int first = m_AtmoConfig->getZonesTopCount(); 
    int last = first + m_AtmoConfig->getZonesLRCount();
    for(int z=first; z < last; z++) right_zones[z-first] = z;

    first = m_AtmoConfig->getZonesTopCount() + m_AtmoConfig->getZonesLRCount() + m_AtmoConfig->getZonesBottomCount(); 
    last = first + m_AtmoConfig->getZonesLRCount();
    for(int z=first; z < last; z++) left_zones[z-first] = z;


    // 0 = SummenKanal
    // 1 = Links
    // 2 = Rechts
    // 3 = oben
    // 4 = unten?
    int r,g,b;

	while(!this->m_bTerminated) {
                   
	     r = rand() % 255 + 1;	// Kanal links neue Farbe zuordnen
	     g = rand() % 255 + 1;
	     b = rand() % 255 + 1;
         for(int z = 0; z < lrc; z++) {
             RGB_aus->zone[left_zones[z]].r = r;
             RGB_aus->zone[left_zones[z]].g = g;
             RGB_aus->zone[left_zones[z]].b = b;
         }
         Interpolation(m_AtmoConfig->getLrColorChanger_iSteps(), m_AtmoConfig->getLrColorChanger_iDelay(), RGB_aus );	
         if(this->m_bTerminated == ATMO_TRUE) break; // Quick stop!

         for(int z = 0; z < lrc; z++) {
             RGB_aus->zone[right_zones[z]].r = r;
             RGB_aus->zone[right_zones[z]].g = g;
             RGB_aus->zone[right_zones[z]].b = b;
         }
         Interpolation(m_AtmoConfig->getLrColorChanger_iSteps(), m_AtmoConfig->getLrColorChanger_iDelay(), RGB_aus );	
         if(this->m_bTerminated == ATMO_TRUE) break; // Quick stop!


	     r = rand() % 255 + 1;	// Kanal rechts neue Farbe zuordnen
	     g = rand() % 255 + 1;
	     b = rand() % 255 + 1;
         for(int z = 0; z < lrc; z++) {
             RGB_aus->zone[right_zones[z]].r = r;
             RGB_aus->zone[right_zones[z]].g = g;
             RGB_aus->zone[right_zones[z]].b = b;
         }
         Interpolation(m_AtmoConfig->getLrColorChanger_iSteps(), m_AtmoConfig->getLrColorChanger_iDelay(), RGB_aus );	
         if(this->m_bTerminated == ATMO_TRUE) break; // Quick stop!

         for(int z = 0; z < lrc; z++) { // Kanal links Farbe von rechts zuordnen
             RGB_aus->zone[left_zones[z]].r = r;
             RGB_aus->zone[left_zones[z]].g = g;
             RGB_aus->zone[left_zones[z]].b = b;
         }
         Interpolation(m_AtmoConfig->getLrColorChanger_iSteps(), m_AtmoConfig->getLrColorChanger_iDelay(), RGB_aus );	
         if(this->m_bTerminated == ATMO_TRUE) break; // Quick stop!
    }

    delete (char *)RGB_aus;
    delete []left_zones;
    delete []right_zones;

	return 0;
}
