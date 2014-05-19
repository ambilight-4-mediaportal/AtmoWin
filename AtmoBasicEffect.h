/*
* AtmoBasicEffect.h: base class for the fun effects like the ColorChangers....
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/
#ifndef _AtmoBasicEffect_h_
#define _AtmoBasicEffect_h_

#include "atmothread.h"
#include "atmoConnection.h"
#include "atmoConfig.h"

class CAtmoBasicEffect :
	public CThread
{

protected:
	// int iaFadeTo_Red[],int iaFadeTo_Green[],int iaFadeTo_Blue[]
	void Interpolation(int iSteps, int iPause, pColorPacket fadeTo);

protected:
	CAtmoConnection *m_AtmoConnection;
	CAtmoConfig *m_AtmoConfig;

protected:
	//int   m_RGB_R_alt[5];  // "alte" werte für Interpolations routine!
	//int   m_RGB_G_alt[5];
	//int	  m_RGB_B_alt[5];

	pColorPacket m_RGB_alt;

public:
	CAtmoBasicEffect(CAtmoConnection *atmoConnection, CAtmoConfig *atmoConfig);
	virtual ~CAtmoBasicEffect(void);
};

#endif