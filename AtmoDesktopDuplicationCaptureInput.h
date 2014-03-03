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

#include <dxgi.h>
#include <DXGI1_2.h>
#include <D3D11.h>

class CAtmoDesktopDuplicationCaptureInput : public CAtmoInput
{
protected:
   	virtual DWORD Execute(void);

private:
    HRESULT GetOutput(LONG vertPos, LONG horzPost, IDXGIOutput1 **pOutput1, IDXGIAdapter1 **pAdapter1);
    void ReadImage(BYTE *pData, UINT Stride);

private:
    tHSVColor HSV_Img[IMAGE_SIZE];

    IDXGIOutput1 *m_pOutput;
    IDXGIOutputDuplication *m_pOutputDuplication;
    ID3D11Device *m_pD3D11Device;
    ID3D11DeviceContext *m_pD3D11DeviceContext;

    ID3D11Texture2D *m_pTexture;

    LONG m_lWidth;
    LONG m_lHeight;

    bool m_bThreadStarted;

public:
    CAtmoDesktopDuplicationCaptureInput(CAtmoDynData *pAtmoDynData);
    virtual ~CAtmoDesktopDuplicationCaptureInput(void);

    // Opens the input-device. Parameters (e.g. the device-name) can be given in 'param'.
    // Returns true if the input-device was opened successfully.
    virtual ATMO_BOOL Open(void);

    // Closes the input-device.
    // Returns true if the input-device was closed successfully.
    virtual ATMO_BOOL Close(void);
};
