/*
# Copyright (C) 2012 Carsten Presser <c@rstenpresser.de>
#
# This file is part of karate_tools, a set of tools to control an debug
# the KarateLight hardware available at http://karatelight.de
#
# karate_tools is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# karate_tools is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with karate_tools; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA
#
*/


#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>

#define OPEN_DEVICE(name) CreateFile(name, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)
#define CLOSE_DEVICE(fd) CloseHandle(fd)
#define WRITE_DATA(fd, buf, len) WriteFile(fd, buf, (DWORD)len, &kl_h.byteswritten, NULL)
#define READ_DATA(fd,buf,len,bread) ReadFile(fd, buf, (DWORD)len, &bread, NULL)

#else
#include <termios.h>
#include <fcntl.h>
#include <sys/file.h>
#include <string.h>
#include <errno.h>

#define OPEN_DEVICE(name) open(name, O_WRONLY|O_NOCTTY)
#define CLOSE_DEVICE(fd) close(fd)
#define WRITE_DATA(fd, buf, len) kl_h.byteswritten = write(fd, buf, len); tcdrain(fd)
#define READ_DATA(fd,buf,len,bread) bread = read(fd, buf, len)


#endif

#include "libkarate.h"


#define MAX_CHANNELS  16

// private prototypes
int KL_createCommand (int cmd, uint8_t * data, int len);
int KL_readBack ();

//
struct KL {
  char * name;
#ifdef WIN32
  HANDLE fd;
  DWORD bytesread;
  DWORD byteswritten;
#else
  int fd;
  int bytesread;
  int byteswritten;
#endif

  int fw_version;
  int nChannels;

  uint8_t wr_buffer[CMD_MAX_LENGTH];
  uint8_t rd_buffer[CMD_MAX_LENGTH];

  unsigned int pwm_value;
  unsigned int ldr_value;

  uint8_t color_buffer[3*MAX_CHANNELS];
} kl_h;

int KL_getnChannels() {
  return kl_h.nChannels;
}

int KL_getFWVersion() {
  return kl_h.fw_version;
}


int KL_setPWMValue(unsigned int val) {
  uint8_t pp[2];

  kl_h.pwm_value=val;

  pp[0] = (kl_h.pwm_value & 0xFF);
  pp[1] = (kl_h.pwm_value>>8 & 0xFF);

  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_SET_TLC_PWM_VALUE,pp,2));
  if (kl_h.byteswritten != (CMD_DATA_START + 2)) {
    return KL_WRITEFAIL;
  }
  return KL_readBack();
}

unsigned int KL_getDEBUG0() {
  unsigned int j,d0;

  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(0x60,NULL,0));
  if (kl_h.byteswritten != CMD_DATA_START) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  if (j == 2) {
    d0=kl_h.rd_buffer[CMD_DATA_START];
    d0+=kl_h.rd_buffer[CMD_DATA_START+1]*256;
    return (unsigned int) d0;
  } else {
    printf("Error reading debug_0 value, readback returned:%d\n",j);
  }
  return j;
}

unsigned int KL_getDEBUG1() {
  unsigned int j,d0;

  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(0x61,NULL,0));
  if (kl_h.byteswritten != CMD_DATA_START) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  if (j == 2) {
    d0=kl_h.rd_buffer[CMD_DATA_START];
    d0+=kl_h.rd_buffer[CMD_DATA_START+1]*256;
    return (unsigned int) d0;
  } else {
    printf("Error reading debug_1 value, readback returned:%d\n",j);
  }
  return j;
}



unsigned int KL_getPWMValue() {
  unsigned int j;

  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_GET_TLC_PWM_VALUE,NULL,0));
  if (kl_h.byteswritten != CMD_DATA_START) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  if (j == 2) {
    kl_h.pwm_value=kl_h.rd_buffer[CMD_DATA_START];
    kl_h.pwm_value+=kl_h.rd_buffer[CMD_DATA_START+1]*256;
    return (unsigned int) kl_h.pwm_value;
  } else {
    printf("Error reading pwm value\n");
  }
  return j;
}

unsigned int KL_getLDRValue() {
  unsigned int j;

  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_READ_ADC0,NULL,0));
  if (kl_h.byteswritten != CMD_DATA_START) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  if (j == 2) {
    kl_h.ldr_value=kl_h.rd_buffer[CMD_DATA_START];
    kl_h.ldr_value+=kl_h.rd_buffer[CMD_DATA_START+1]*256;
    return (unsigned int) kl_h.ldr_value;
  } else {
    printf("Error reading ldr value\n");
  }
  return j;
}

int KL_updateColors() {
  int j;

  // write colors
  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_SET_TLC_DATA,kl_h.color_buffer,3*kl_h.nChannels));
  if (kl_h.byteswritten != (CMD_DATA_START + 3*kl_h.nChannels)) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  return j;
}

int KL_enterBoot() {
  int j;

  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_BOOT_REQUEST,NULL,0));
  if (kl_h.byteswritten != CMD_DATA_START) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  if (j == 2) {
    WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_BOOT_START,&kl_h.rd_buffer[CMD_DATA_START],2));
    if (kl_h.byteswritten != (CMD_DATA_START + 2)) {
      return KL_WRITEFAIL;
    }
    KL_close();
    // no more readback... just close the device
  }
  return KL_OK;
}

int KL_setColor(uint8_t chan, uint8_t r, uint8_t g, uint8_t b) {
  if (chan < kl_h.nChannels) {
    kl_h.color_buffer[chan*3 + 0] = g;
    kl_h.color_buffer[chan*3 + 1] = b;
    kl_h.color_buffer[chan*3 + 2] = r;
    return KL_OK;
  }
  return KL_ERROR;
}

int KL_calcChecksum (int len) {
  int i;

  // clear byte 2
  kl_h.wr_buffer[CMD_HD_CHECK] = 0;

  for (i=0; i<len; i++) {
    if (i !=CMD_HD_CHECK) {
      kl_h.wr_buffer[CMD_HD_CHECK]^= kl_h.wr_buffer[i];
    }
  }

  return len;
}

int KL_createCommand (int cmd, uint8_t * data, int len) {
  int i = 0;

  // maximum command lenght
  if (len > (CMD_MAX_LENGTH - CMD_DATA_START)) {
    printf("Error: Command is to long (%d > %d)\n",len, (CMD_MAX_LENGTH - CMD_DATA_START));
    return KL_ERROR;
  }

  // build header
  kl_h.wr_buffer[CMD_HD_SYNC]     = CMD_SYNC_SEND;
  kl_h.wr_buffer[CMD_HD_COMMAND]  = cmd;
  kl_h.wr_buffer[CMD_HD_LEN]      = len;

  // copy data
  while (len > i) {
    kl_h.wr_buffer[CMD_DATA_START+i] = data[i];
    i++;
  }
 
  // calc checksum
  i =  KL_calcChecksum(CMD_DATA_START+len);

#ifdef DEBUG
  int j;
  printf("write buffer (l=%2d) ",i);
  for (j=0; j<i; j++) {
    printf("0x%0X ",kl_h.wr_buffer[j]);
  }
  printf ("\n");
#endif
  
  return i;
}

int KL_readBack () {
  // read header 
  READ_DATA(kl_h.fd,kl_h.rd_buffer,CMD_DATA_START,kl_h.bytesread);

  if (kl_h.bytesread != CMD_DATA_START) {
#ifdef DEBUG
    printf("first read (header) returned %d bytes, expected 4 bytes\n", kl_h.bytesread);  
#endif
    return KL_ERROR;
  }
  
  // read sequential
  if (kl_h.rd_buffer[CMD_HD_LEN] > 0) {
    READ_DATA(kl_h.fd, &kl_h.rd_buffer[CMD_DATA_START], kl_h.rd_buffer[CMD_HD_LEN],kl_h.bytesread);
  } else {
    kl_h.bytesread = 0;
  }

#ifdef DEBUG
    int j;
    printf("read buffer  (l=%2d) ",(int)(kl_h.bytesread+CMD_DATA_START));
    for (j=0; j<(kl_h.bytesread+CMD_DATA_START); j++) {
      printf("0x%0X ",kl_h.rd_buffer[j]);
    }
    printf("\n");
#endif

  if (kl_h.bytesread == kl_h.rd_buffer[CMD_HD_LEN]) {
    // TODO verify checksum
    return (kl_h.bytesread);
  } else {
#ifdef DEBUG
    printf("number of bytes read(%d) does not match number of bytes expected(%d) \n", kl_h.bytesread, kl_h.rd_buffer[CMD_HD_LEN]);  
#endif
    return KL_CHECKSUMFAIL;
  }
}



int KL_close (){
#ifdef WIN32
  CloseHandle(kl_h.fd);
#else
  // remove lock and close file
  flock(kl_h.fd, LOCK_UN);
  tcflush(kl_h.fd,TCIOFLUSH);
  close(kl_h.fd);
#endif
  kl_h.fd=0;
#ifdef DEBUG
    printf("closed.\n");  
#endif
  return 0;
}

int KL_init(const char * device) {
  int j;
	
#ifdef WIN32
  DCB dcbSerialParams = {0};
  COMMTIMEOUTS timeouts={0};

  kl_h.fd=CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if ((int)kl_h.fd <0) {
    printf("Error opening (%s)\n",device);
    return KL_ERROR;
  }
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  j = GetCommState(kl_h.fd, &dcbSerialParams);
  if (j) {
    dcbSerialParams.BaudRate = CBR_57600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    j = SetCommState(kl_h.fd, &dcbSerialParams);
  } 
  if (!j) {
    printf("Error configuring (%s) (j=%d)\n",device,j);
    return KL_ERROR;
  }
  
  timeouts.ReadIntervalTimeout=50;
  timeouts.ReadTotalTimeoutConstant=50;
  timeouts.ReadTotalTimeoutMultiplier=10;
  timeouts.WriteTotalTimeoutConstant=50;
  timeouts.WriteTotalTimeoutMultiplier=10;
  if(!SetCommTimeouts(kl_h.fd, &timeouts)){
    printf("Error setting Timeouts (%s)\n",device);
    return KL_ERROR;
  }
#else
  struct termios options;

  kl_h.fd = open(device, O_RDWR | O_NOCTTY);
  if (kl_h.fd <0) {
    printf("Error opening (%s)\n",device);
    return KL_ERROR;
  }

  /* Clear the line */
  tcflush(kl_h.fd,TCOFLUSH);

  memset(&options, 0, sizeof(options));

  cfsetispeed(&options, B115200);
  cfsetospeed(&options, B115200);

  //options.c_cflag = (CS8 | CSTOPB | CLOCAL | CREAD);
  options.c_cflag = (CS8 | CLOCAL | CREAD);

  //If MIN = 0 and TIME > 0, TIME serves as a timeout value. The read will be satisfied if a single character is read, or TIME is exceeded (t = TIME *0.1 s). If TIME is exceeded, no character will be returned. 
  options.c_cc[VTIME] = 1; 
  options.c_cc[VMIN]   = 0;


  /* Update the options and do it NOW */
  if (tcsetattr(kl_h.fd,TCSANOW,&options) != 0) {
    printf("Error opening (%s)\n",device);
    return KL_ERROR;
  } 

  if (flock(kl_h.fd, LOCK_EX | LOCK_NB) != 0) {
    printf("Error getting a lock on %s: %s. Maybe a other programm is accessing the device.\n",device,strerror(errno));
    return KL_ERROR;
  }

#endif

  // clear possible junk data still in the systems fifo
  do {
    READ_DATA(kl_h.fd,kl_h.rd_buffer,255,kl_h.bytesread);
    if (kl_h.bytesread > 0) {
      printf("cleared %d junkbytes\n",kl_h.bytesread);
#ifdef DEBUG
    int j;
    printf("read buffer  (l=%2d) ",(int)(kl_h.bytesread));
    for (j=0; j<kl_h.bytesread; j++) {
      printf("0x%0X ",kl_h.rd_buffer[j]);
    }
    printf("\n");
#endif
    }
  }
  while (kl_h.bytesread > 0);
 


  // read channels count
  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_GET_TLC_CHANNELS,NULL,0));
  if (kl_h.byteswritten != CMD_DATA_START) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  if (j == 1) {
    kl_h.nChannels = (int) kl_h.rd_buffer[CMD_DATA_START]/9*2;
  } else {
    printf("Error reading channels. got %d bytes\n",j);
    return j;
  }
    
  // read firmware version
  WRITE_DATA(kl_h.fd,kl_h.wr_buffer,KL_createCommand(CMD_GET_VERSION,NULL,0));
  if (kl_h.byteswritten != CMD_DATA_START) {
    return KL_WRITEFAIL;
  }

  j = KL_readBack();
  if (j == 1) {
    kl_h.fw_version= (int) kl_h.rd_buffer[CMD_DATA_START];
  } else {
    printf("Error reading firmware Version\n");
    return j;
  }

/*  if (kl_h.fw_version < 0x24) {
    printf("Firmware 0x%0x is to old!\n",kl_h.fw_version);
  } */
   
  // KL_Bank();

  return KL_OK;
}

int KL_blank () {
 int j;

  // set channels to black
  for (j=0; j < (MAX_CHANNELS*3); j++) {
    kl_h.color_buffer[j] = 0;
  }
  j = KL_updateColors();
  if (j != 0) {
    printf("Error sending color data\n");
    return j;
  }
  return KL_OK;
}


/*int main(int argc, char *argv[])
{
  KL_Init("/dev/ttyACM0");

  // open successfull
  if (kl_h.fd > 0) {
    printf ("Device reports %d channels\n",kl_h.nChannels);
    printf ("Device reports Firmware Version 0x%0X\n",kl_h.fw_version);

    KL_setColor(0,255,0,0);
    KL_updateColors();
  }
  
} */
