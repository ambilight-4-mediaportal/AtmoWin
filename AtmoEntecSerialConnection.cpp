/*
 * AtmoEntecSerialConnection.cpp: Class for communication with a ENTTEC DMX Dongle/Controller
 * for hardware see also:
 * http://www.enttec.com/index.php?main_menu=Products&pn=70303
 * 
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id: bc47035e81fe9e5f76786531dc2e4a21d5371b96 $
 */
#include "AtmoDefs.h"
#include "AtmoEntecSerialConnection.h"
#include "DmxTools.h"

#if !defined(_ATMO_VLC_PLUGIN_)
#include "DmxConfigDialog.h"
#endif

#include <stdio.h>
#include <fcntl.h>

#if !defined(WIN32)
#include <termios.h>
#include <unistd.h>
#endif


CAtmoEntecSerialConnection::CAtmoEntecSerialConnection(CAtmoConfig *cfg) : CAtmoConnection(cfg) {
    m_hComport = INVALID_HANDLE_VALUE;

    memset(&DMXout, 0, sizeof(DMXout));

    m_dmx_channels_base = ConvertDmxStartChannelsToInt( cfg->getDMX_RGB_Channels(), cfg->getDMX_BaseChannels(), 512-3);
}


CAtmoEntecSerialConnection::~CAtmoEntecSerialConnection() {
    delete m_dmx_channels_base;
}

ATMO_BOOL CAtmoEntecSerialConnection::OpenConnection() {
#if defined(_ATMO_VLC_PLUGIN_)
     char *serdevice = m_pAtmoConfig->getSerialDevice();
     if(!serdevice)
        return ATMO_FALSE;
#else
     int portNummer = m_pAtmoConfig->getComport();
     m_dwLastWin32Error = 0;
	 if(portNummer < 1) return ATMO_FALSE; // make no real sense;-)
#endif

     if(!m_dmx_channels_base)
        return ATMO_FALSE;

	 CloseConnection();

#if !defined(_ATMO_VLC_PLUGIN_)
     char serdevice[16];  // com4294967295
     sprintf(serdevice,"com%d",portNummer);
#endif

#if defined(WIN32)

     m_hComport = CreateFile(serdevice, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
     if(m_hComport == INVALID_HANDLE_VALUE) {
//      we have a problem here can't open com port... somebody else may use it?
	    m_dwLastWin32Error = GetLastError();
	    return ATMO_FALSE;
     }
     /* change serial settings (Speed, stopbits etc.) */
     DCB dcb; // für comport-parameter
     dcb.DCBlength = sizeof(DCB);
     GetCommState (m_hComport, &dcb); // ger current serialport settings

     dcb.BaudRate  = 250000; //CBR_256000; // set higher speed  250000 ?
     dcb.ByteSize  = 8;            // set databits
     dcb.Parity    = NOPARITY;     // set parity
     dcb.StopBits  = TWOSTOPBITS;   // set two stop bits
     if(!SetCommState (m_hComport, &dcb)) {    // apply settings
         m_dwLastWin32Error = GetLastError();
         CloseHandle(m_hComport);
         m_hComport = INVALID_HANDLE_VALUE;
         return ATMO_FALSE;
     }

#else
      !TODO!

     m_hComport = open(serdevice, O_RDWR |O_NOCTTY);
     if(m_hComport < 0) {
	    return ATMO_FALSE;
     }

     struct termios tio;
     memset(&tio,0,sizeof(tio));
     tio.c_cflag = (CS8 | CREAD | HUPCL | CLOCAL);
     tio.c_iflag = (INPCK | BRKINT);
     cfsetispeed(&tio, 250000);
     cfsetospeed(&tio, 250000);
     if(!tcsetattr(m_hComport, TCSANOW, &tio)) {
         tcflush(m_hComport, TCIOFLUSH);
     } else {
         // can't change parms
        close(m_hComport);
        m_hComport = INVALID_HANDLE_VALUE;
        return ATMO_FALSE;
     }

#endif

     return ATMO_TRUE;
}

void CAtmoEntecSerialConnection::CloseConnection() {
  if(m_hComport!=INVALID_HANDLE_VALUE) {
#if defined(WIN32)
     CloseHandle(m_hComport);
#else
     close(m_hComport);
#endif
	 m_hComport = INVALID_HANDLE_VALUE;
  }
}

ATMO_BOOL CAtmoEntecSerialConnection::isOpen(void) {
	 return (m_hComport != INVALID_HANDLE_VALUE);
}

ATMO_BOOL CAtmoEntecSerialConnection::HardwareWhiteAdjust(int global_gamma,
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


ATMO_BOOL CAtmoEntecSerialConnection::SendData(pColorPacket data) {
   if(m_hComport == INVALID_HANDLE_VALUE)
	  return ATMO_FALSE;

   int iBuffer = 1;
   DWORD iBytesWritten;

   Lock();

   int zoneIdx, z = 0;

   for(int channel = 0; channel < getNumChannels(); channel++) {
       if(m_ChannelAssignment && (channel < m_NumAssignedChannels))
         zoneIdx = m_ChannelAssignment[channel];
       else
         zoneIdx = -1;

       if((zoneIdx>=0) && (zoneIdx<data->numColors)) {
          if( m_dmx_channels_base[z] >= 0 )
              iBuffer = m_dmx_channels_base[z] + 1;
          else
              iBuffer += 3;

          if(m_pAtmoConfig->isWhiteAdjPerChannel()) 
          {
             int wr;
             int wg;
             int wb;
             m_pAtmoConfig->getChannelWhiteAdj( channel, wr, wg, wb);   

             DMXout[iBuffer]   = (data->zone[zoneIdx].r * wr) / 256;
             DMXout[iBuffer+1] = (data->zone[zoneIdx].g * wg) / 256;
             DMXout[iBuffer+2] = (data->zone[zoneIdx].b * wb) / 256;

          } else {

             DMXout[iBuffer]   = data->zone[zoneIdx].r;
             DMXout[iBuffer+1] = data->zone[zoneIdx].g;
             DMXout[iBuffer+2] = data->zone[zoneIdx].b;

          }
       }

       if( m_dmx_channels_base[z] >= 0 )
          z++;
   }

#if defined(WIN32)
   WriteFile(m_hComport, DMXout, 513, &iBytesWritten, NULL); // send to COM-Port
#else
   iBytesWritten = write(m_hComport, DMXout, 513);
   tcdrain(m_hComport);
#endif
   Unlock();

   return (iBytesWritten == 513) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CAtmoEntecSerialConnection::setChannelValues(int numValues,unsigned char *channel_values)
{
	DWORD iBytesWritten;
    if((numValues & 1) || !channel_values)
       return ATMO_FALSE; // numValues must be even!

    /*
       the array shall contain
    */

	Lock();
    int dmxIndex = 0;

    for (int i = 0; i < numValues; i+=2) {
         dmxIndex = ((int)channel_values[i]) + 1;
         DMXout[dmxIndex] = channel_values[i+1];
    }
#if defined(WIN32)
	WriteFile(m_hComport, DMXout, 513, &iBytesWritten, NULL);
#else
    iBytesWritten = write(m_hComport, DMXout, 513);
    tcdrain(m_hComport);
#endif

    Unlock();

	return (iBytesWritten == 513) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CAtmoEntecSerialConnection::setChannelColor(int channel, tRGBColor color)
{
	DWORD iBytesWritten;
	
    Lock();

    DMXout[channel+0+1]=color.r;
	DMXout[channel+1+1]=color.g;
	DMXout[channel+2+1]=color.b;

#if defined(WIN32)
	WriteFile(m_hComport, DMXout, 513, &iBytesWritten, NULL);
#else
    iBytesWritten = write(m_hComport, DMXout, 513);
    tcdrain(m_hComport);
#endif


    Unlock();

	return (iBytesWritten == 513) ? ATMO_TRUE : ATMO_FALSE;
}

ATMO_BOOL CAtmoEntecSerialConnection::CreateDefaultMapping(CAtmoChannelAssignment *ca)
{
    if(!ca) return ATMO_FALSE;
    ca->setSize( getNumChannels() );
    for(int i = 0; i < getNumChannels(); i++)
        ca->setZoneIndex(i , i);
    return ATMO_TRUE;
}

#if !defined(_ATMO_VLC_PLUGIN_)

char *CAtmoEntecSerialConnection::getChannelName(int ch)
{
  if(ch < 0) return NULL;
  char buf[30];

  switch(ch) {
      case 0:
          sprintf(buf,"Summenkanal [%d]",ch);
          break;
      case 1:
          sprintf(buf,"Linker Kanal [%d]",ch);
          break;
      case 2:
          sprintf(buf,"Rechter Kanal [%d]",ch);
          break;
      case 3:
          sprintf(buf,"Oberer Kanal [%d]",ch);
          break;
      case 4:
          sprintf(buf,"Unterer Kanal [%d]",ch);
          break;
      default:
          sprintf(buf,"Kanal [%d]",ch);
          break;
  }

  return strdup(buf);
}

ATMO_BOOL CAtmoEntecSerialConnection::ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg)
{
    CDmxConfigDialog *dlg = new CDmxConfigDialog(hInst, parent, cfg);

    INT_PTR result = dlg->ShowModal();

    delete dlg;

    if(result == IDOK)
      return ATMO_TRUE;
    else
      return ATMO_FALSE;
}

#endif


int CAtmoEntecSerialConnection::getNumChannels()
{
    return m_pAtmoConfig->getDMX_RGB_Channels();
}

