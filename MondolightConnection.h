#ifndef _MondolightConnection_h_
#define _MondolightConnection_h_

#include "AtmoDefs.h"
#include "AtmoConnection.h"
#include "AtmoConfig.h"

#if defined(WIN32)
#   include <windows.h>
#endif


class CMondolightConnection :  public CAtmoConnection
{
    private:
        HANDLE m_hComport;
        // 16bit outputbuffer! byte order? -> cross compile? problem!
        // output buffer contains PWM DutyCycles!
		unsigned short int m_OutputBuffer[16*3]; // 16 Kanäle RGB!

        /*
          on windows devName is COM1 COM2 etc.
          on linux devname my be /dev/ttyS0 or /dev/ttyUSB0
        */
       HANDLE OpenDevice(char *devName); 

    public:
       CMondolightConnection(CAtmoConfig *cfg);
       virtual ~CMondolightConnection(void);

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
       virtual char *getChannelName(int ch);

       virtual ATMO_BOOL ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg);  

       virtual const char *getDevicePath() { return "Mondolight"; }

       virtual ATMO_BOOL CreateDefaultMapping(CAtmoChannelAssignment *ca);
};

#endif

