/*
 * AtmoLeftRightColorChanger.h:  a fun effect - which fades a color from the left channel to the right channel, then
 *    takes the next random color and starts again...
 *
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#pragma once

#include "atmobasiceffect.h"

class CAtmoLeftRightColorChanger :  public CAtmoBasicEffect
{
protected:
	virtual DWORD Execute(void);

public:
    CAtmoLeftRightColorChanger(CAtmoConnection *atmoConnection, CAtmoConfig *atmoConfig);
    virtual ~CAtmoLeftRightColorChanger(void);
};
