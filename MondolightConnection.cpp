
#include "MondolightConnection.h"
#include "AtmoClassicConfigDialog.h"

// I assume that short is a 16bit wide data type!
// Table to map the color to the required PWM Dutycycle!
unsigned short int lut[256] = {
   0, 1, 3, 4, 6, 8, 9, 11, 12, 14, 15, 17, 19, 20, 22, 23, 
   25, 27, 29, 30, 32, 34, 35, 37, 39, 41, 42, 44, 46, 48, 50, 52, 54, 55, 
   57, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 86, 88, 90, 92, 
   94, 96, 98, 101, 103, 105, 107, 110, 112, 114, 117, 119, 121, 124, 126, 129, 
   131, 133, 136, 138, 141, 143, 146, 149, 151, 154, 156, 159, 162, 164, 167, 170, 
   172, 175, 178, 181, 183, 186, 189, 192, 195, 198, 201, 203, 206, 209, 212, 215, 
   218, 222, 225, 228, 231, 234, 237, 240, 243, 247, 250, 253, 256, 260, 263, 266, 
   270, 273, 277, 280, 284, 287, 291, 294, 298, 301, 305, 308, 312, 316, 319, 323, 
   327, 331, 334, 338, 342, 346, 350, 354, 358, 362, 366, 370, 374, 378, 382, 386, 
   390, 394, 399, 403, 407, 411, 416, 420, 424, 429, 433, 438, 442, 447, 451, 456, 
   460, 465, 470, 474, 479, 484, 489, 493, 498, 503, 508, 513, 518, 523, 528, 533, 
   538, 543, 548, 554, 559, 564, 569, 575, 580, 585, 591, 596, 602, 607, 613, 618, 
   624, 630, 635, 641, 647, 652, 658, 664, 670, 676, 682, 688, 694, 700, 706, 712, 
   719, 725, 731, 737, 744, 750, 756, 763, 769, 776, 783, 789, 796, 802, 809, 816, 
   823, 830, 837, 843, 850, 857, 865, 872, 879, 886, 893, 900, 908, 915, 923, 930, 
   938, 945, 953, 960, 968, 976, 983, 991, 999, 1007, 1015, 1023
 };


CMondolightConnection::CMondolightConnection(CAtmoConfig *cfg) : CAtmoConnection( cfg )
{
  m_hComport = INVALID_HANDLE_VALUE;
  memset(m_OutputBuffer, 0, sizeof(m_OutputBuffer));
}

CMondolightConnection::~CMondolightConnection(void)
{
}

HANDLE CMondolightConnection::OpenDevice(char *devName)
{
     HANDLE hComport;     

#if !defined(_ATMO_VLC_PLUGIN_)
     m_dwLastWin32Error = 0;
#endif

#if defined(WIN32)

     hComport = CreateFile(devName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
     if(hComport == INVALID_HANDLE_VALUE) {
#if !defined(_ATMO_VLC_PLUGIN_)
	    m_dwLastWin32Error = GetLastError();
#endif
	    return INVALID_HANDLE_VALUE;
     }
     /* change serial settings (Speed, stopbits etc.) */
     DCB dcb; // für comport-parameter
     dcb.DCBlength = sizeof(DCB);
     GetCommState (hComport, &dcb); // ger current serialport settings
     dcb.BaudRate  = 38400;        // set speed
     dcb.ByteSize  = 8;            // set databits
     dcb.Parity    = NOPARITY;     // set parity
     dcb.StopBits  = ONESTOPBIT;   // set one stop bit
     SetCommState (hComport, &dcb);    // apply settings

#else

     int bconst = B38400;
     hComport = open(devName,O_RDWR |O_NOCTTY);
     if(hComport < 0) {
	    return INVALID_HANDLE_VALUE;;
     }
     struct termios tio;
     memset(&tio,0,sizeof(tio));
     tio.c_cflag = (CS8 | CREAD | HUPCL | CLOCAL);
     tio.c_iflag = (INPCK | BRKINT);
     cfsetispeed(&tio, bconst);
     cfsetospeed(&tio, bconst);
     if(!tcsetattr(hComport, TCSANOW, &tio)) {
         tcflush(hComport, TCIOFLUSH);
     } else {
         // can't change parms
        close(hComport);
        return INVALID_HANDLE_VALUE;
     }
#endif

     return hComport;
}

ATMO_BOOL CMondolightConnection::OpenConnection()
{
#if defined(_ATMO_VLC_PLUGIN_)

    // config char *?

#else
    char devName[16];
    sprintf(devName,"com%d",m_pAtmoConfig->getComport());
    m_hComport = OpenDevice(devName);
#endif

   if(m_hComport != INVALID_HANDLE_VALUE)
      return ATMO_TRUE;
   else
      return ATMO_FALSE;
}

void CMondolightConnection::CloseConnection()
{
  if(m_hComport != INVALID_HANDLE_VALUE) {
     CloseHandle( m_hComport );
     m_hComport = INVALID_HANDLE_VALUE;
  }
}

ATMO_BOOL CMondolightConnection::isOpen(void)
{
    if(m_hComport != INVALID_HANDLE_VALUE)
        return ATMO_TRUE;
    else
        return ATMO_FALSE;
}

ATMO_BOOL CMondolightConnection::HardwareWhiteAdjust(int global_gamma,
                                             int global_contrast,
                                             int contrast_red,
                                             int contrast_green,
                                             int contrast_blue,
                                             int gamma_red,
                                             int gamma_green,
                                             int gamma_blue,
                                             ATMO_BOOL storeToEeprom)
{
    return(ATMO_FALSE);
}

ATMO_BOOL CMondolightConnection::SendData(pColorPacket data)
{
   if(m_hComport == INVALID_HANDLE_VALUE)
	  return ATMO_FALSE;

   int numChannels = this->getNumChannels();

   int zoneIdx;
   DWORD iBytesWritten;
   int iBuffer = 0;

   Lock();

   for(int channel = 0; channel < numChannels ; channel++) {
       if(m_ChannelAssignment && (channel < m_NumAssignedChannels))
          zoneIdx = m_ChannelAssignment[channel]; 
       else
          zoneIdx = -1;  

       if((zoneIdx>=0) && (zoneIdx<data->numColors)) {
          if(m_pAtmoConfig->isWhiteAdjPerChannel()) 
          {
             int wr;
             int wg;
             int wb;
             m_pAtmoConfig->getChannelWhiteAdj( channel, wr, wg, wb);   

             m_OutputBuffer[iBuffer]   = lut[ (data->zone[zoneIdx].r * wr ) / 256 ];
             m_OutputBuffer[iBuffer+1] = lut[ (data->zone[zoneIdx].g * wg ) / 256 ];
             m_OutputBuffer[iBuffer+2] = lut[ (data->zone[zoneIdx].b * wb ) / 256 ];

          } else {

             m_OutputBuffer[iBuffer]   = lut[ data->zone[zoneIdx].r ];
             m_OutputBuffer[iBuffer+1] = lut[ data->zone[zoneIdx].g ];
             m_OutputBuffer[iBuffer+2] = lut[ data->zone[zoneIdx].b ];

          }
       }

       iBuffer+=3;
   }

#if defined(WIN32)
   WriteFile(m_hComport, m_OutputBuffer, 16 * 2 * 3, &iBytesWritten, NULL); // send to COM-Port
#else
   iBytesWritten = write(m_hComport, m_OutputBuffer, 16 * 2 * 3);
   tcdrain(hComport);
#endif
   Unlock();

   return (iBytesWritten == (16 * 2 * 3)) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CMondolightConnection::setChannelColor(int channel, tRGBColor color)
{
  if((channel<0) || (channel>15) || (m_hComport == INVALID_HANDLE_VALUE))
     return ATMO_FALSE;
  DWORD iBytesWritten;

  Lock();
  m_OutputBuffer[channel*3]     = lut[color.r];
  m_OutputBuffer[channel*3 + 1] = lut[color.g];
  m_OutputBuffer[channel*3 + 2] = lut[color.b];

#if defined(WIN32)
  WriteFile(m_hComport, m_OutputBuffer, 16 * 2 * 3, &iBytesWritten, NULL); // send to COM-Port
#else
  iBytesWritten = write(m_hComport, m_OutputBuffer, 16 * 2 * 3);
  tcdrain(hComport);
#endif
  Unlock();
  return (iBytesWritten == (16 * 2 * 3)) ? ATMO_TRUE : ATMO_FALSE;
}

ATMO_BOOL CMondolightConnection::setChannelValues(int numValues,unsigned char *channel_values)
{
  if((m_hComport == INVALID_HANDLE_VALUE) || (numValues & 1) || !channel_values)
     return ATMO_FALSE;

  DWORD iBytesWritten;

  Lock();
  int Index = 0;
  for (int i = 0; i < numValues; i+=2) {
       Index = (int)channel_values[i];
       if(Index < (sizeof(m_OutputBuffer)/sizeof(short int)))
          m_OutputBuffer[Index] = lut[channel_values[i + 1]];
  }

#if defined(WIN32)
  WriteFile(m_hComport, m_OutputBuffer, 16 * 2 * 3, &iBytesWritten, NULL); // send to COM-Port
#else
  iBytesWritten = write(m_hComport, m_OutputBuffer, 16 * 2 * 3);
  tcdrain(hComport);
#endif
  Unlock();

  return (iBytesWritten == (16 * 2 * 3)) ? ATMO_TRUE : ATMO_FALSE;
}

int CMondolightConnection::getNumChannels()
{
  return(16);
}

char *CMondolightConnection::getChannelName(int ch)
{
  char buf[30];
  sprintf(buf, "Channel[%d]",ch);
  return strdup(buf);
}

ATMO_BOOL CMondolightConnection::CreateDefaultMapping(CAtmoChannelAssignment *ca)
{
  if(!ca) return ATMO_FALSE;
  ca->setSize(16); 
  for(int i = 0; i < 16; i++)
      ca->setZoneIndex(i, i);
  return ATMO_TRUE;
}

ATMO_BOOL CMondolightConnection::ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg)
{
  CAtmoClassicConfigDialog *dlg = new CAtmoClassicConfigDialog(hInst, parent, cfg);

  INT_PTR result = dlg->ShowModal();

  delete dlg;

  if(result == IDOK) 
     return ATMO_TRUE;
  else 
    return ATMO_FALSE;
}
