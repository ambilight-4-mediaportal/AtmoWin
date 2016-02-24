/*
 * AtmoSerialConnection.cpp: Class for communication with the serial hardware of
 * Atmo Light, opens and configures the serial port
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id: edd37fb84029decae0236b3f1d787d16c779591e $
 */

#include "stdafx.h"

#include "AtmoDefs.h"
#include "AtmoDuinoV2Connection.h"

#if !defined(_ATMO_VLC_PLUGIN_)
# include "AtmoDuinoV2ConfigDialog.h"
#endif

#include <stdio.h>
#include <fcntl.h>

#if !defined(WIN32)
#include <termios.h>
#include <unistd.h>
#endif


CAtmoDuinoV2Connection::CAtmoDuinoV2Connection(CAtmoConfig *cfg) : CAtmoConnection(cfg) {
    m_hComport = INVALID_HANDLE_VALUE;
    memset(m_frame_buffer, 0, sizeof(m_frame_buffer));
    // Sync Header
    m_frame_buffer[0] = 0xD7;
    m_frame_buffer[1] = 0xEE;
    m_frame_buffer[2] = 0x23;
    // Number of LEDS - 1
    m_frame_buffer[3] = getNumChannels() - 1;
}

CAtmoDuinoV2Connection::~CAtmoDuinoV2Connection() {
}

ATMO_BOOL CAtmoDuinoV2Connection::OpenConnection() {
#if defined(_ATMO_VLC_PLUGIN_)
    char *serdevice = m_pAtmoConfig->getSerialDevice();
    if(!serdevice)
        return ATMO_FALSE;
#else
    int portNummer = m_pAtmoConfig->getComport();
    m_dwLastWin32Error = 0;
    if (portNummer < 1) return ATMO_FALSE; // make no real sense;-)
#endif

    CloseConnection();

#if !defined(_ATMO_VLC_PLUGIN_)
    char serdevice[16];  // com4294967295
    sprintf(serdevice, "com%d", portNummer);
#endif

#if defined(WIN32)

    m_hComport = CreateFile(serdevice, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (m_hComport == INVALID_HANDLE_VALUE) {
        //      we have a problem here can't open com port... somebody else may use it?
        m_dwLastWin32Error = GetLastError();
        return ATMO_FALSE;
    }
    /* change serial settings (Speed, stopbits etc.) */
    DCB dcb; // fuer comport-parameter
    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hComport, &dcb);   // ger current serialport settings
    
	//dcb.BaudRate = CBR_115200;        // set speed

	switch (m_pAtmoConfig->getAtmoV2_BaudrateIndex())
	{
				// set baud speed
		case 1: dcb.BaudRate = 115200;
		break;
		case 2: dcb.BaudRate = 230400;
		break;
		case 3: dcb.BaudRate = 250000;
		break;
		case 4: dcb.BaudRate = 345600;
		break;
		case 5: dcb.BaudRate = 460800;
		break;
		case 6: dcb.BaudRate = 500000;
		break;
		case 7: dcb.BaudRate = 576000;
		break;							
		case 8: dcb.BaudRate = 1000000;	
		break;							
		case 9: dcb.BaudRate = 2000000;
		break;
		case 10: dcb.BaudRate = 4000000;
		break;
		case 11: dcb.BaudRate = 6000000;
		break;
		case 12: dcb.BaudRate = 8000000;
		break;
		case 13: dcb.BaudRate = 10000000;
		break;
		default: dcb.BaudRate = CBR_115200; // set default speed
	}

    dcb.ByteSize = 8;                 // set databits
    dcb.Parity = NOPARITY;            // set parity
    dcb.StopBits = ONESTOPBIT;        // set one stop bit
    SetCommState(m_hComport, &dcb);   // apply settings

#else

    int bconst = B115200;
    m_hComport = open(serdevice,O_RDWR |O_NOCTTY);
    if(m_hComport < 0) {
        return ATMO_FALSE;
    }

    struct termios tio;
    memset(&tio,0,sizeof(tio));
    tio.c_cflag = (CS8 | CREAD | HUPCL | CLOCAL);
    tio.c_iflag = (INPCK | BRKINT);
    cfsetispeed(&tio, bconst);
    cfsetospeed(&tio, bconst);
    if(!tcsetattr(m_hComport, TCSANOW, &tio)) {
        tcflush(m_hComport, TCIOFLUSH);
    } else {
        // can't change parms
        close(m_hComport);
        m_hComport = -1;
        return false;
    }

#endif

    return true;
}

void CAtmoDuinoV2Connection::CloseConnection() {
    if (m_hComport != INVALID_HANDLE_VALUE) {
#if defined(WIN32)
        CloseHandle(m_hComport);
#else
        close(m_hComport);
#endif
        m_hComport = INVALID_HANDLE_VALUE;
    }
}

ATMO_BOOL CAtmoDuinoV2Connection::isOpen(void) {
    return (m_hComport != INVALID_HANDLE_VALUE);
}

ATMO_BOOL CAtmoDuinoV2Connection::HardwareWhiteAdjust(int global_gamma,
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

ATMO_BOOL CAtmoDuinoV2Connection::SendData(pColorPacket data) {
    if (m_hComport == INVALID_HANDLE_VALUE)
        return ATMO_FALSE;

    DWORD iBytesWritten;

    int iBuffer = 4;
    int zoneIdx;

    Lock();

    for (int channel = 0; channel < getNumChannels() ; channel++) {
        if (m_ChannelAssignment && (channel < m_NumAssignedChannels))
            zoneIdx = m_ChannelAssignment[channel];
        else
            zoneIdx = -1;


        if ((zoneIdx >= 0) && (zoneIdx < data->numColors))
        {
            if (m_pAtmoConfig->isWhiteAdjPerChannel())
            {
                int wr;
                int wg;
                int wb;
                m_pAtmoConfig->getChannelWhiteAdj(channel, wr, wg, wb);

                m_frame_buffer[iBuffer] = (data->zone[zoneIdx].r * wr) / 256;
                m_frame_buffer[iBuffer + 1] = (data->zone[zoneIdx].g * wg) / 256;
                m_frame_buffer[iBuffer + 2] = (data->zone[zoneIdx].b * wb) / 256;

            } else {

                m_frame_buffer[iBuffer] = data->zone[zoneIdx].r;
                m_frame_buffer[iBuffer + 1] = data->zone[zoneIdx].g;
                m_frame_buffer[iBuffer + 2] = data->zone[zoneIdx].b;

            }
        } else {
            m_frame_buffer[iBuffer] = 0;
            m_frame_buffer[iBuffer + 1] = 0;
            m_frame_buffer[iBuffer + 2] = 0;
        }

        iBuffer += 3;
    }

#if defined(WIN32)
    WriteFile(m_hComport, m_frame_buffer, getBufferSize(), &iBytesWritten, NULL); // send to COM-Port
#else
    iBytesWritten = write(m_hComport, m_frame_buffer, getBufferSize());
    tcdrain(m_hComport);
#endif

    Unlock();

    return (iBytesWritten == getBufferSize()) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CAtmoDuinoV2Connection::CreateDefaultMapping(CAtmoChannelAssignment *ca)
{
    if (!ca) return ATMO_FALSE;
    ca->setSize(getNumChannels());
    for (int i = 0; i < getNumChannels(); i++)
        ca->setZoneIndex(i, i);
    return ATMO_TRUE;
}

#if !defined(_ATMO_VLC_PLUGIN_)

ATMO_BOOL CAtmoDuinoV2Connection::setChannelColor(int channel, tRGBColor color)
{
    DWORD iBytesWritten = 0;
    if ((channel >= 0) && (channel < getNumChannels()))
    {
        Lock();

        m_frame_buffer[channel + 0 + 4] = color.r;
        m_frame_buffer[channel + 1 + 4] = color.g;
        m_frame_buffer[channel + 2 + 4] = color.b;

#if defined(WIN32)
        WriteFile(m_hComport, m_frame_buffer, getBufferSize(), &iBytesWritten, NULL); // send to COM-Port
#else
        iBytesWritten = write(m_hComport, m_frame_buffer, getBufferSize());
        tcdrain(m_hComport);
#endif
        Unlock();
    }
    return (iBytesWritten == getBufferSize()) ? ATMO_TRUE : ATMO_FALSE;
}

ATMO_BOOL CAtmoDuinoV2Connection::setChannelValues(int numValues, unsigned char *channel_values)
{
    DWORD iBytesWritten;
    if ((numValues & 1) || !channel_values)
        return ATMO_FALSE; // numValues must be even!

    /*
       the array shall contain
       */
    Lock();
    int Index;

    for (int i = 0; i < numValues; i += 2) {
        Index = ((int)channel_values[i]);
        if ((Index >= 0) && (Index < getNumChannels() * 3))
            m_frame_buffer[Index + 4] = channel_values[i + 1];
    }
#if defined(WIN32)
    WriteFile(m_hComport, m_frame_buffer, getBufferSize(), &iBytesWritten, NULL); // send to COM-Port
#else
    iBytesWritten = write(m_hComport, m_frame_buffer, getBufferSize());
    tcdrain(m_hComport);
#endif
    Unlock();

    return (iBytesWritten == getBufferSize()) ? ATMO_TRUE : ATMO_FALSE;
}

int CAtmoDuinoV2Connection::getNumChannels()
{
    return m_pAtmoConfig->getAtmoV2ClLeds();
}

int CAtmoDuinoV2Connection::getBufferSize(){
    return getNumChannels() * 3 + 4;
}

char *CAtmoDuinoV2Connection::getChannelName(int ch)
{
    if (ch < 0) return NULL;
    char buf[30];
    sprintf(buf, "LED [%d]", ch + 1);
    return strdup(buf);
}

ATMO_BOOL CAtmoDuinoV2Connection::ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg)
{
    CAtmoDuinoV2ConfigDialog *dlg = new CAtmoDuinoV2ConfigDialog(hInst, parent, cfg);

    INT_PTR result = dlg->ShowModal();

    delete dlg;

    if (result == IDOK)
        return ATMO_TRUE;
    else
        return ATMO_FALSE;
}

#endif
