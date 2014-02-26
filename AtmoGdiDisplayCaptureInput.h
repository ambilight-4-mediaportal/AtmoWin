/*
 * AtmoGdiDisplayCaptureInput.h: Implements the picture source which grabs the desktop as source for the live view mode
 * poor performance ... through some GDI unsolved problems, if there are overlays active or some other applications...
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#pragma once
#include "atmoinput.h"
#include "atmothread.h"
#include "atmoconfig.h"
#include "AtmoCalculations.h"
#include "AtmoDisplays.h"

// define this to use GdiGetPixel instead of GetDibBits();
// # define UseGdiGetPixel
// # define UseGdiDesktopGetPixel

// #define black_hole


class CAtmoGdiDisplayCaptureInput : public CAtmoInput
{
protected:
   	virtual DWORD Execute(void);
    void CalcColors();
    void CalcColorsNew();

private:
	tHSVColor HSV_Img[IMAGE_SIZE];

#ifndef UseGdiDesktopGetPixel 
	HDC m_hdcScreen;
    HDC m_hTempBitmapDC;
#endif

#ifndef UseGdiGetPixel
    unsigned char *m_PixelBuffer;
#endif

    RECT m_ScreenSourceRect;

    int m_iSrcRows[CAP_HEIGHT];
    int m_iSrcCols[CAP_WIDTH];
	int m_currentLine;
	int m_rowsPerFrame;

    int m_CurrentFrame;

public:
    CAtmoGdiDisplayCaptureInput(CAtmoDynData *pAtmoDynData);
    virtual ~CAtmoGdiDisplayCaptureInput(void);

    // Opens the input-device. Parameters (e.g. the device-name) can be given in 'param'.
    // Returns true if the input-device was opened successfully.
    virtual ATMO_BOOL Open(void);

    // Closes the input-device.
    // Returns true if the input-device was closed successfully.
    virtual ATMO_BOOL Close(void);
};
