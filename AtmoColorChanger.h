/*
 * AtmoColorChanger.h: class for the fun effect: random color fading on all five channels
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#pragma once
#include "atmothread.h"
#include "atmoConnection.h"
#include "atmoConfig.h"
#include "atmoBasicEffect.h"

class CAtmoColorChanger :	public CAtmoBasicEffect
{

protected:
	virtual DWORD Execute(void);

public:
	CAtmoColorChanger(CAtmoConnection *atmoConnection, CAtmoConfig *atmoConfig);
	virtual ~CAtmoColorChanger(void);
};
