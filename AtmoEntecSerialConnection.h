/*
 * AtmoEntecSerialConnection.h: Class for communication with the serial DMX Interface of ENTTEC
 * http://www.enttec.com/index.php?main_menu=Products&pn=70303
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id: 53ef12a8702c0b1db5df453ec6e3f4e6577df813 $
 */
#ifndef _AtmoEntecSerialConnection_h_
#define _AtmoEntecSerialConnection_h_


#include "AtmoDefs.h"
#include "AtmoConnection.h"
#include "AtmoConfig.h"

#if defined(WIN32)
#   include <windows.h>
#endif

class CAtmoEntecSerialConnection : public CAtmoConnection {
    private:
        HANDLE m_hComport;
        // DMX Channel Buffer including some Control Bytes for up to 512 DMX Channels
		unsigned char DMXout[513];
        // contains the DMX Start Adress of each Atmo-Dmx-Channel
        int *m_dmx_channels_base;

    public:
       CAtmoEntecSerialConnection(CAtmoConfig *cfg);
       virtual ~CAtmoEntecSerialConnection(void);

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

	   virtual ATMO_BOOL setChannelColor(int channel, tRGBColor color);

       virtual ATMO_BOOL setChannelValues(int numValues,unsigned char *channel_values);

       virtual int getNumChannels();

#if !defined(_ATMO_VLC_PLUGIN_)
       virtual char *getChannelName(int ch);
       virtual ATMO_BOOL ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg);
#endif

       virtual const char *getDevicePath() { return "enttec"; }

       virtual ATMO_BOOL CreateDefaultMapping(CAtmoChannelAssignment *ca);
};

#endif
