/*
 * KarateConnection.h: Class for communication with the serial hardware of KarateLight,
 * opens and configures the serial port
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 */
#ifndef _KarateConnection_h_
#define _KarateConnection_h_

#include "AtmoDefs.h"
#include "AtmoConnection.h"
#include "AtmoConfig.h"

#if defined(WIN32)
#   include <windows.h>
#endif

#define KARATEBYTESPERFRAME	28
#define KARATENUMCHANNEL		8

class CKarateConnection : public CAtmoConnection {
    private:

#if defined(WIN32)
        DWORD  m_dwLastWin32Error;
    public:
        DWORD getLastError() { return m_dwLastWin32Error; }
#endif

    public:
       CKarateConnection(CAtmoConfig *cfg);
       virtual ~CKarateConnection(void);

  	   virtual ATMO_BOOL OpenConnection();

       virtual void CloseConnection();

       virtual ATMO_BOOL isOpen(void);

       virtual ATMO_BOOL SendData(pColorPacket data);

       virtual ATMO_BOOL HardwareWhiteAdjust(int global_gamma,
                                             int global_contrast,
                                             int contrast_red,
                                             int contrast_green,
                                             int contrast_blue,
                                             int gamma_red,
                                             int gamma_green,
                                             int gamma_blue,
                                             ATMO_BOOL storeToEeprom);

       virtual int getNumChannels() { return KARATENUMCHANNEL; }

       virtual ATMO_BOOL setChannelColor(int channel, tRGBColor color);


       virtual const char *getDevicePath() { return "karate"; }

#if !defined(_ATMO_VLC_PLUGIN_)
       virtual char *getChannelName(int ch);
       virtual ATMO_BOOL ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg);
#endif

       virtual ATMO_BOOL CreateDefaultMapping(CAtmoChannelAssignment *ca);
};

#endif
