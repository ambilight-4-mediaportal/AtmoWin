#pragma once
#include "atmoconnection.h"
#include "atmoconfig.h"
#include "atmodefs.h"
#include "BasicWindow.h"

class CAtmoDummyConnection : public CAtmoConnection, public CBasicWindow
{
private:
     ATMO_BOOL m_IsOpen;
     pColorPacket m_CurrentValues;

protected:
    LPCTSTR ClassName();
    void HandleWmPaint(PAINTSTRUCT ps,HDC hdc);
    void CreateWindowClass(WNDCLASSEX &wndclassex);

    int FindChannel(int zone);

public:
    CAtmoDummyConnection(HINSTANCE hInst, CAtmoConfig *pAtmoConfig);
    virtual ~CAtmoDummyConnection(void);

public:
    virtual ATMO_BOOL OpenConnection();
    virtual void CloseConnection();
    virtual ATMO_BOOL isOpen(void);
    virtual ATMO_BOOL SendData(pColorPacket data);
    virtual ATMO_BOOL HardwareWhiteAdjust(int global_gamma, int global_contrast, int contrast_red, int contrast_green, int contrast_blue, int gamma_red, int gamma_green, int gamma_blue, ATMO_BOOL storeToEeprom);
    virtual int getNumChannels();
    virtual const char *getDevicePath() { return "Dummy"; }

    char *getChannelName(int ch);

    virtual ATMO_BOOL CreateDefaultMapping(CAtmoChannelAssignment *ca);
};
