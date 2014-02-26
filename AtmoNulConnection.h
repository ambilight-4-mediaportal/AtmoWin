#ifndef _AtmoNulConnection_h_
#define _AtmoNulConnection_h_

#include "atmoconnection.h"

class CAtmoNulConnection :
    public CAtmoConnection
{
public:
  	CAtmoNulConnection(CAtmoConfig *cfg);
    virtual ~CAtmoNulConnection(void);

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

   virtual int getNumChannels() { return -1; }

   virtual const char *getDevicePath() { return "null"; }
   virtual ATMO_BOOL CreateDefaultMapping(CAtmoChannelAssignment *ca) { return true; }
};

#endif
