/*
* AtmoDmxSerialConnection.cpp: Class for communication with a Simple DMX Dongle/Controller
* for hardware see also:
* http://www.dzionsko.de/elektronic/index.htm
* http://www.ulrichradig.de/ (search for dmx on his page)
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id: 51156a25febe471e8bcffe59d9e47e1fa8599c22 $
*/


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AtmoDefs.h"
#include "AtmoDmxSerialConnection.h"
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


CAtmoDmxSerialConnection::CAtmoDmxSerialConnection(CAtmoConfig *cfg) : CAtmoConnection(cfg) {
	m_hComport = INVALID_HANDLE_VALUE;


	memset(&DMXout, 0, sizeof(DMXout));
	// DMXout[0] = 0x5A;     // DMX Command Start Byte
	DMXout[0] = 0xA5;     // DMX Command Start Byte
	DMXout[1] = 0x5A;     // DMX Command Start Byte
	dmx_ch = cfg->getDMX_RGB_Channels();
	if (dmx_ch<65){
		DMXout[2] = 0xA1;     // DMX Controlcommand for 768 256 channels
		//DMXout[1] = 0xA1;     // DMX Controlcommand for 768 256 channels
		//DMXout[258] = 0xA5;   // end of block

		m_dmx_channels_base = ConvertDmxStartChannelsToInt( cfg->getDMX_RGB_Channels(), cfg->getDMX_BaseChannels(), 256-3);}
	else
	{
		DMXout[2] = 0xB0;     // DMX Controlcommand for 768 256 channels
		//DMXout[1] = 0xB0;     // DMX Controlcommand for 768 256 channels
		//DMXout[770] = 0xA5;   // end of block

		m_dmx_channels_base = ConvertDmxStartChannelsToInt( cfg->getDMX_RGB_Channels(), cfg->getDMX_BaseChannels(), 768-3);}

}


CAtmoDmxSerialConnection::~CAtmoDmxSerialConnection() {
	delete m_dmx_channels_base;
}

ATMO_BOOL CAtmoDmxSerialConnection::OpenConnection() {
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
	DCB dcb; // fuer comport-parameter
	dcb.DCBlength = sizeof(DCB);
	GetCommState (m_hComport, &dcb); // ger current serialport settings

	switch(m_pAtmoConfig->getDMX_BaudrateIndex())
	{
	case 1:   
		dcb.BaudRate  = 250000; //CBR_256000; // set higher speed  250000 ?
		break; 
	default: 
		dcb.BaudRate  = CBR_115200; // set default speed
	}
	dcb.ByteSize  = 8;            // set databits
	dcb.Parity    = NOPARITY;     // set parity
	dcb.StopBits  = ONESTOPBIT;   // set one stop bit
	SetCommState (m_hComport, &dcb);    // apply settings

#else

	switch(m_pAtmoConfig->getDMX_BaudrateIndex())
	{
	case 1:   
		bconst  = 250000; /// B256000; // set higher speed  250000 ?
		break; 
	default: 
		bconst  = B115200; // set default speed
	}

	m_hComport = open(serdevice, O_RDWR |O_NOCTTY);
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

void CAtmoDmxSerialConnection::CloseConnection() {
	if(m_hComport!=INVALID_HANDLE_VALUE) {
#if defined(WIN32)
		CloseHandle(m_hComport);
#else
		close(m_hComport);
#endif
		m_hComport = INVALID_HANDLE_VALUE;
	}
}

ATMO_BOOL CAtmoDmxSerialConnection::isOpen(void) {
	return (m_hComport != INVALID_HANDLE_VALUE);
}

ATMO_BOOL CAtmoDmxSerialConnection::HardwareWhiteAdjust(int global_gamma,
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




ATMO_BOOL CAtmoDmxSerialConnection::SendSEDU(int data) {
	if(m_hComport == INVALID_HANDLE_VALUE)
		return ATMO_FALSE;

	DWORD iBytesWritten;
	if (data ==1 )	 WriteFile(m_hComport, Dummy2, 5, &iBytesWritten, NULL);
	if (data ==0 )	 WriteFile(m_hComport, Dummy3, 8, &iBytesWritten, NULL);

	return ((iBytesWritten == 5)  || (iBytesWritten == 8)) ? ATMO_TRUE : ATMO_FALSE;

}


ATMO_BOOL CAtmoDmxSerialConnection::SendData(pColorPacket data) {
	if(m_hComport == INVALID_HANDLE_VALUE)
		return ATMO_FALSE;

	int iBuffer = 3;
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
				iBuffer = m_dmx_channels_base[z] + 3;
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

				/* CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();

				DMXout[iBuffer]   = config->red_whiteAdj[data->zone[zoneIdx].r];
				DMXout[iBuffer+1] = config->green_whiteAdj[data->zone[zoneIdx].g];
				DMXout[iBuffer+2] = config->blue_whiteAdj[data->zone[zoneIdx].b];*/



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
	if (dmx_ch<65)
	{
		//WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, DMXout, 256+3, &iBytesWritten, NULL); // send to COM-Port
		WriteFile(m_hComport, DMXout, 256+3, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
	}
	else
	{

		//WriteFile(m_hComport, Dummy3, 2, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, Dummy3, 2, &iBytesWritten, NULL); // send to COM-Port
		WriteFile(m_hComport, DMXout, 768+3, &iBytesWritten, NULL); // send to COM-Port
		// WriteFile(m_hComport, DMXout, 768+3, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, Dummy3, 2, &iBytesWritten, NULL); // send to COM-Port

	}

#else
	iBytesWritten = write(m_hComport, DMXout, 768+3);
	tcdrain(m_hComport);
#endif
	Unlock();

	if (dmx_ch<65)
		return (iBytesWritten == 256+3) ? ATMO_TRUE : ATMO_FALSE;
	else
		return (iBytesWritten == 768+3) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CAtmoDmxSerialConnection::setChannelValues(int numValues,unsigned char *channel_values)
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
		dmxIndex = ((int)channel_values[i]) + 2;
		DMXout[dmxIndex] = channel_values[i+1];
	}
#if defined(WIN32)
	if (dmx_ch<65)
	{
		// WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, DMXout, 256+3, &iBytesWritten, NULL); // send to COM-Port
		WriteFile(m_hComport, DMXout, 256+3, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
	}
	else
	{ //WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, DMXout, 768+3, &iBytesWritten, NULL); // send to COM-Port
		// WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
		WriteFile(m_hComport, DMXout, 768+3, &iBytesWritten, NULL);
	}
#else
	iBytesWritten = write(m_hComport, DMXout, 768+3);
	tcdrain(m_hComport);
#endif

	Unlock();

	if (dmx_ch<65)
		return (iBytesWritten == 256+3) ? ATMO_TRUE : ATMO_FALSE;
	else
		return (iBytesWritten == 768+3) ? ATMO_TRUE : ATMO_FALSE;
	//return (iBytesWritten == 768+3) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CAtmoDmxSerialConnection::setChannelColor(int channel, tRGBColor color)
{
	DWORD iBytesWritten;

	Lock();

	DMXout[channel+0+3]=color.r;
	DMXout[channel+1+3]=color.g;
	DMXout[channel+2+3]=color.b;

#if defined(WIN32)
	//WriteFile(m_hComport, DMXout, 768+3, &iBytesWritten, NULL);
	if (dmx_ch<65)
	{
		//WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, DMXout, 256+3, &iBytesWritten, NULL); // send to COM-Port
		WriteFile(m_hComport, DMXout, 256+3, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
	}
	else
	{
		//WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, DMXout, 768+3, &iBytesWritten, NULL); // send to COM-Port
		WriteFile(m_hComport, DMXout, 768+3, &iBytesWritten, NULL); // send to COM-Port
		//WriteFile(m_hComport, Dummy1, 2, &iBytesWritten, NULL); // send to COM-Port
	}
#else
	iBytesWritten = write(m_hComport, DMXout, 768+3);
	tcdrain(m_hComport);
#endif


	Unlock();
	if (dmx_ch<65)
		return (iBytesWritten == 256+3) ? ATMO_TRUE : ATMO_FALSE;
	else
		return (iBytesWritten == 768+3) ? ATMO_TRUE : ATMO_FALSE;
	//return (iBytesWritten == 768+3) ? ATMO_TRUE : ATMO_FALSE;
}

ATMO_BOOL CAtmoDmxSerialConnection::CreateDefaultMapping(CAtmoChannelAssignment *ca)
{
	if(!ca) return ATMO_FALSE;
	ca->setSize( getNumChannels() );
	for(int i = 0; i < getNumChannels(); i++)
		ca->setZoneIndex(i , i);
	return ATMO_TRUE;
}

#if !defined(_ATMO_VLC_PLUGIN_)

char *CAtmoDmxSerialConnection::getChannelName(int ch)
{

	if(ch < 0) return NULL;
	char buf[30];
	int t =m_pAtmoConfig->getZonesTopCount();
	int lr = m_pAtmoConfig->getZonesLRCount();
	int b = m_pAtmoConfig->getZonesBottomCount();
	int mp = getNumChannels();

	sprintf(buf,"Channel [%d]",ch);
	if (ch<t ) sprintf(buf,"Top Channel [%d]",ch);
	if (ch>=t && ch<(t+lr) ) sprintf(buf,"Right Channel [%d]",ch);
	if (ch>=(t+lr) && ch<(t+lr+b) ) sprintf(buf,"Bottom Channel [%d]",ch);
	if (ch>=(t+lr+b) && ch<mp ) sprintf(buf,"Left Channel [%d]",ch);

	return strdup(buf);
}

ATMO_BOOL CAtmoDmxSerialConnection::ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg)
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


int CAtmoDmxSerialConnection::getNumChannels()
{
	return m_pAtmoConfig->getDMX_RGB_Channels();
}

