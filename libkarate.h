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
#ifndef _libkarate_h_
#define _libkarate_h

#ifndef WIN32
#include <stdint.h>
#include <unistd.h>
#else
#define uint8_t unsigned char
#endif

#define LIBKARATE_VERSION "20120430"

int KL_setColor(uint8_t chan, uint8_t r, uint8_t g, uint8_t b);
int KL_updateColors();
int KL_init(const char * device);
int KL_close();
int KL_getnChannels(); 
int KL_getFWVersion(); 
int KL_enterBoot();
int KL_blank ();

int KL_setPWMValue(unsigned int val);
unsigned int KL_getPWMValue();
unsigned int KL_getLDRValue();
unsigned int KL_getDEBUG0();
unsigned int KL_getDEBUG1();

#define KL_OK			0
#define KL_ERROR		-1
#define KL_CHECKSUMFAIL		-2
#define KL_WRITEFAIL		-3

#define CMD_MAX_LENGTH		64

// address
#define CMD_HD_SYNC		0x00
#define CMD_HD_COMMAND		0x01
#define CMD_HD_CHECK		0x02
#define CMD_HD_LEN		0x03
#define CMD_DATA_START		0x04

// sync words
#define CMD_SYNC_SEND		0xAA
#define CMD_SYNC_RECV		0x55

// status
#define CMD_SYS_SYNC		0x00
#define CMD_SYS_ACK		0x01		
#define CMD_SYS_NACK		0x02
#define CMD_SYS_NIMP		0xFF
#define CMD_SYS_IR		0x10
#define CMD_SYS_DATA		0x20
#define CMD_SYS_NACK_LENGTH	0x03
#define CMD_SYS_NACK_CHECK	0x04

// commands
#define CMD_GET_VERSION		0x01

#define CMD_SET_TLC_DATA_RAW	0x10
#define CMD_SET_TLC_CHANNEL	0x11
#define CMD_SET_TLC_DATA	0x12
#define CMD_GET_TLC_CHANNELS	0x13
#define CMD_GET_TLC_PWM_VALUE	0x14
#define CMD_SET_TLC_PWM_VALUE	0x15

#define CMD_READ_ADC0		0x40

#define CMD_BOOT_REQUEST	0x80
#define CMD_BOOT_START		0x81

#endif
