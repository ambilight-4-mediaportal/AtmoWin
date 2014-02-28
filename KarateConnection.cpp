/*
 * KarateConnection.cpp: Class for communication with the serial hardware of
 * Atmo Light, opens and configures the serial port
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 */

#include "stdafx.h"

#include "AtmoDefs.h"
#include "KarateConnection.h"

#if !defined(_ATMO_VLC_PLUGIN_)
# include "AtmoClassicConfigDialog.h"
#endif

#include <stdio.h>
#include <fcntl.h>



 extern "C" {
  #include "libkarate.h"
 }

// constructor
CKarateConnection::CKarateConnection(CAtmoConfig *cfg) : CAtmoConnection(cfg) {
}

// destructor
CKarateConnection::~CKarateConnection() {
}


ATMO_BOOL CKarateConnection::OpenConnection() {
     int portNummer = m_pAtmoConfig->getComport();
     m_dwLastWin32Error = 0;
	 if(portNummer < 1) return ATMO_FALSE; // make no real sense;-)

	 CloseConnection();

#if !defined(_ATMO_VLC_PLUGIN_)
     char serdevice[16];  // com4294967295
     sprintf(serdevice,"com%d",portNummer);
#endif

    return (KL_init(serdevice) == KL_OK) ? ATMO_TRUE : ATMO_FALSE;
}

void CKarateConnection::CloseConnection() {
	KL_close();
}

ATMO_BOOL CKarateConnection::isOpen(void) {
	// TODO
	return ATMO_TRUE;
}

ATMO_BOOL CKarateConnection::HardwareWhiteAdjust(int global_gamma,
                                                     int global_contrast,
                                                     int contrast_red,
                                                     int contrast_green,
                                                     int contrast_blue,
                                                     int gamma_red,
                                                     int gamma_green,
                                                     int gamma_blue,
                                                     ATMO_BOOL storeToEeprom) {
     return ATMO_FALSE;
}


ATMO_BOOL CKarateConnection::SendData(pColorPacket data) {
   int ret = 0;
   int zoneIdx;

   Lock();

   for(int channel = 0; channel < KARATENUMCHANNEL ; channel++) {
       if(m_ChannelAssignment && (channel < m_NumAssignedChannels))
          zoneIdx = m_ChannelAssignment[ channel ];
       else
          zoneIdx = -1;


       if((zoneIdx>=0) && (zoneIdx<data->numColors))
       {
          if(m_pAtmoConfig->isWhiteAdjPerChannel()) 
          {
             int wr;
             int wg;
             int wb;
             m_pAtmoConfig->getChannelWhiteAdj( channel, wr, wg, wb);   
			 KL_setColor(channel, (data->zone[zoneIdx].r * wr / 255), (data->zone[zoneIdx].g * wg / 255), (data->zone[zoneIdx].b * wb / 255));

          } else {

			 KL_setColor(channel, data->zone[zoneIdx].r, data->zone[zoneIdx].g, data->zone[zoneIdx].b);

          }
       }
	   else
	   {
		 KL_setColor(channel, 0, 0, 0);
	   }

   }
   
   ret = KL_updateColors();

   Unlock();
   return (ret == KL_OK) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CKarateConnection::CreateDefaultMapping(CAtmoChannelAssignment *ca)
{
   int i;
   if(!ca) return ATMO_FALSE;
   ca->setSize(KARATENUMCHANNEL);
   for (i=0; i<KARATENUMCHANNEL; i++) {
      ca->setZoneIndex(i, i);
   }
   return ATMO_TRUE;
}

#if !defined(_ATMO_VLC_PLUGIN_)

ATMO_BOOL CKarateConnection::setChannelColor(int channel, tRGBColor color)
{
   int ret=0;
   Lock();
   KL_setColor(channel, color.r, color.g, color.b);
   ret = KL_updateColors();

   Unlock();
   return (ret == KL_OK) ? ATMO_TRUE : ATMO_FALSE;
}


char *CKarateConnection::getChannelName(int ch)
{
  if(ch < 0) return NULL;
  char buf[30];

  sprintf(buf,"Kanal [%d]",ch);
  return strdup(buf);
}

ATMO_BOOL CKarateConnection::ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg)
{
    CAtmoClassicConfigDialog *dlg = new CAtmoClassicConfigDialog(hInst, parent, cfg);

    INT_PTR result = dlg->ShowModal();

    delete dlg;

    if(result == IDOK)
      return ATMO_TRUE;
    else
      return ATMO_FALSE;
}

#endif
