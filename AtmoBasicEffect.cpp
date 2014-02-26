/*
 * AtmoBasicEffect.cpp: base class for the fun effects like the ColorChangers....
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#include "StdAfx.h"
#include "atmobasiceffect.h"
#include "AtmoTools.h"


CAtmoBasicEffect::CAtmoBasicEffect(CAtmoConnection *atmoConnection, CAtmoConfig *atmoConfig) : CThread() {
	this->m_AtmoConnection  = atmoConnection;
	this->m_AtmoConfig      = atmoConfig;

    m_RGB_alt = NULL;
}

CAtmoBasicEffect::~CAtmoBasicEffect(void)
{
    delete (char *)m_RGB_alt; 
}

void CAtmoBasicEffect::Interpolation(int iSteps, int iPause, pColorPacket fadeTo)
{
    int s_1;  // zählvariablen
    int	s_2;

    pColorPacket ausgabe;
    pColorPacket output;

    // Stepsize for each fading step
    float *interpolation_r = new float[fadeTo->numColors]; // Differenz zw. 2 R/G/B werten zur Interpolation
    float *interpolation_g = new float[fadeTo->numColors];
    float *interpolation_b = new float[fadeTo->numColors];

    if(!m_RGB_alt || (m_RGB_alt->numColors != fadeTo->numColors) )
    {
   	   delete (char *)m_RGB_alt; 
       m_RGB_alt=NULL;

       for ( s_1 = 0; s_1 < fadeTo->numColors ; s_1++ ) {
            interpolation_r[s_1] = (float)(fadeTo->zone[s_1].r) / (float)iSteps;
 		    interpolation_g[s_1] = (float)(fadeTo->zone[s_1].g) / (float)iSteps;
 		    interpolation_b[s_1] = (float)(fadeTo->zone[s_1].b) / (float)iSteps;
       }

    } else {
   	   for ( s_1 = 0; s_1 < fadeTo->numColors ; s_1++ ) {
            interpolation_r[s_1] = (float)(fadeTo->zone[s_1].r - m_RGB_alt->zone[s_1].r) / (float)iSteps;
 		    interpolation_g[s_1] = (float)(fadeTo->zone[s_1].g - m_RGB_alt->zone[s_1].g) / (float)iSteps;
 		    interpolation_b[s_1] = (float)(fadeTo->zone[s_1].b - m_RGB_alt->zone[s_1].b) / (float)iSteps;
       }
    }

    AllocColorPacket(ausgabe, fadeTo->numColors);

    // fading... color...
	for ( s_2 = 1; s_2<=iSteps ; s_2++ ) // anfang interpolation und ausgabe
	{
        if(m_RGB_alt)
        {
		   for(int zone = 0; zone<fadeTo->numColors; zone++) {
  			   ausgabe->zone[zone].r = m_RGB_alt->zone[zone].r + (int)((float)s_2 * interpolation_r[zone]);
			   ausgabe->zone[zone].g = m_RGB_alt->zone[zone].g + (int)((float)s_2 * interpolation_g[zone]);
  			   ausgabe->zone[zone].b = m_RGB_alt->zone[zone].b + (int)((float)s_2 * interpolation_b[zone]);
           }
        } else  {
		   for(int zone = 0; zone<fadeTo->numColors; zone++) {
  			   ausgabe->zone[zone].r = (int)((float)s_2 * interpolation_r[zone]);
			   ausgabe->zone[zone].g = (int)((float)s_2 * interpolation_g[zone]);
  			   ausgabe->zone[zone].b = (int)((float)s_2 * interpolation_b[zone]);
           }
        }
        DupColorPacket(output, ausgabe);

        output = CAtmoTools::ApplyGamma(m_AtmoConfig, output);

        if(m_AtmoConfig->isUseSoftwareWhiteAdj())
           output = CAtmoTools::WhiteCalibration(m_AtmoConfig, output);

        m_AtmoConnection->SendData(output);

        delete (char *)output;

		if(ThreadSleep(iPause) == ATMO_FALSE)
		   break;
	}	

    // save last color as start color for next run
    delete (char *)m_RGB_alt;
    delete []interpolation_r;
    delete []interpolation_g;
    delete []interpolation_b;


    m_RGB_alt = ausgabe; 
}
