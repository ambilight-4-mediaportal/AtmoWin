/*
 * AtmoDmxSerialConnection.h: Class for communication with the serial DMX Interface of dzionsko,
 * opens and configures the serial port
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id: 02103c01f86d06bb24add8a3f2519a439e9c5b4b $
 */
#ifndef _AtmoDmxSerialConnection_h_
#define _AtmoDmxSerialConnection_h_


#include "AtmoDefs.h"
#include "AtmoConnection.h"
#include "AtmoConfig.h"
#include "AtmoDynData.h"
#include "AtmoRegistryConfig.h"

#if defined(WIN32)
#   include <windows.h>
#endif

class CAtmoDmxSerialConnection : public CAtmoConnection {
	protected:
    CAtmoDynData *m_pAtmoDynData;

    private:
        //HANDLE m_hComport;
		HANDLE m_hComport;
        // DMX Channel Buffer including some Control Bytes for up to 512 DMX Channels
		unsigned char DMXout[768+3];
		
		
        // contains the DMX Start Adress of each Atmo-Dmx-Channel
        int *m_dmx_channels_base;

    public:
		
		int CAtmoDmxSerialConnection::trilinear(int x, int y, int z, int col);
		int dmx_ch;
       CAtmoDmxSerialConnection(CAtmoConfig *cfg);
       virtual ~CAtmoDmxSerialConnection(void);

  	   virtual ATMO_BOOL OpenConnection();


       virtual void CloseConnection();

       virtual ATMO_BOOL isOpen(void);

       virtual ATMO_BOOL SendData(pColorPacket data);

	   ATMO_BOOL SendSEDU(int data);

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

       virtual const char *getDevicePath() { return "dmx"; }

       virtual ATMO_BOOL CreateDefaultMapping(CAtmoChannelAssignment *ca);
};

#endif
