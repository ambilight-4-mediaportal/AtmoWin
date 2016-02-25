/*
 * AtmoDefs.h: a lot of globals defines for the color computation - most of this file
 * is an one  to one copy of "defs.h" from Atmo VDR Plugin
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id: a7719a0133149529f19c0d15cf2546b22dc6d9ed $
 */

#ifndef _AtmoDefs_h_
#define _AtmoDefs_h_


#if defined(__LIBVLC__)

#   include <vlc_common.h>

/* some things need to be changed if this code is used inside VideoLan Filter Module */
#   define _ATMO_VLC_PLUGIN_
#   define get_time mdate()
#   define do_sleep(a) msleep(a)

#else

#   define MakeDword(ch1,ch2,ch3,ch4) ((((DWORD)(ch1)&255) << 24) | \
                                   (((DWORD)(ch2)&255) << 16) | \
                                   (((DWORD)(ch3)&255) << 8) | \
                                   (((DWORD)(ch4)&255)))

#  define get_time1 GetTickCount()
#  define do_sleep(a) Sleep(a)

#endif

#define ATMO_BOOL   bool
#define ATMO_TRUE   true
#define ATMO_FALSE  false


/*
  can't use the VLC_TWOCC macro because the byte order there is CPU dependent
  but for the use in Atmo I need for this single purpose Intel Byte Order
  every time!
*/

#define MakeIntelWord(ch1,ch2)  ((((int)(ch1)&255)<<8) | \
                           ((int)(ch2)&255))

// my own min max macros
#define ATMO_MIN(X, Y)  ((X) < (Y) ? (X) : (Y))
#define ATMO_MAX(X, Y)  ((X) > (Y) ? (X) : (Y))


#if !defined(WIN32)

#define INVALID_HANDLE_VALUE -1
typedef int HANDLE;
typedef unsigned long DWORD;

#define BI_RGB 0L

#if !defined(_BITMAPFILEHEADER_)
#define _BITMAPFILEHEADER_
typedef struct
#ifdef HAVE_ATTRIBUTE_PACKED
    __attribute__((__packed__))
#endif
{
        uint16_t   bfType;
        uint32_t   bfSize;
        uint16_t   bfReserved1;
        uint16_t   bfReserved2;
        uint32_t   bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
#endif

#endif



// maximal Anzahl Kanaele... original 5!
#define CAP_MAX_NUM_ZONES  768
// #define CAP_MAX_NUM_ZONES_ATMO  300 // not used yet
// #define CAP_MAX_NUM_ZONES_DMX  256 // not used yet
// only for classic to avoid changing too much code!
// #define ATMO_MAX_NUM_CHANNELS 5

// capture width/height

// #define CAP_WIDTH    88

#ifdef CAP_16x9
# define CAP_WIDTH   96 // : 240 : 170 : 96 : 88 : 64 : 46
# define CAP_HEIGHT  54 // : 135 :  96 : 54 : 48 : 36 : 26
#else
# define CAP_WIDTH   64 //  46 : 128
# define CAP_HEIGHT  48 //  26 : 64
#endif

// imagesize
#define IMAGE_SIZE   (CAP_WIDTH * CAP_HEIGHT)

/*
  number of pixel the atmo zones should overlap - based on CAP_WIDTH and CAP_HEIGHT
*/
// typedef int CAP_ZONE_OVERLAP; //  0



enum AtmoConnectionType
{
      actClassicAtmo = 0,
      actDummy = 1,
      actDMX = 2,
      actNUL = 3,
      actMultiAtmo = 4,
      actMondolight = 5,
      actMoMoLight = 6,
      actFnordlicht = 7,
      actEnttecDMX = 8,
      actAtmoDuinoV2 = 9
};

static const unsigned char Dummy[96+3] = {
			0x5A, 0xA0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0 ,0xA5
		};

// feste Farbe schwarz
static const unsigned char Dummy1[12] = {
			0xA5, 0x5A, 0xFF, 0xC0, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x02, 0x01
		};

// kein Programm
static const unsigned char Dummy2[5] = {
			0xA5, 0x5A, 0xFF, 0xC0, 0x00
		};

static const unsigned char Dummy3[8] = {
			0xA5, 0x5A, 0xFF, 0xC0, 0x01, 0x00, 0x00, 0x00
		};

static const char AtmoDeviceTypes[10][16] = {
      "Atmoduino",
      "Dummy",
      "MiniDMX",
      "Nul-Device",
      "Multi-Atmo",
      "Mondolight",
      "MoMoLight",
      "Fnordlicht",
      "ENTTEC-DMX",
      "AtmoDuino v2"
  };
#define ATMO_DEVICE_COUNT 10

#if defined(_ATMO_VLC_PLUGIN_)
enum EffectMode {
      emUndefined = -1,
      emDisabled = 0,
      emStaticColor = 1,
      emLivePicture = 2
   };

enum LivePictureSource {
       lpsDisabled = 0,
       lpsExtern = 2
     };

#else

enum EffectMode {
      emUndefined = -1,
      emDisabled = 0,
      emStaticColor = 1,
			emLivePicture = 2,
      emColorChange = 3,
      emLrColorChange = 4,
	    emColorMode = 5
   };

enum LivePictureSource {
       lpsDisabled = 0,
       lpsScreenCapture = 1,
       lpsExtern = 2
     };

#endif

enum AtmoGammaCorrect {
     agcNone = 0,
     agcPerColor = 1,
     agcGlobal = 2
};

enum AtmoFilterMode {
     afmNoFilter,
     afmCombined,
     afmPercent
};
// /* 3d point */
 typedef struct 
 {
	 double x, y, z; 
 } Point3Struct;

// --- tRGBColor --------------------------------------------------------------
typedef struct
{
  unsigned char r, g, b;
} tRGBColor;

// --- tColorPacket -----------------------------------------------------------
typedef struct
{
   int numColors;
   tRGBColor zone[1];
} xColorPacket;
typedef xColorPacket* pColorPacket;
#define AllocColorPacket(packet, numColors_) packet = (pColorPacket)new char[sizeof(xColorPacket) + (numColors_)*sizeof(tRGBColor)]; \
                                             packet->numColors = numColors_;

#define DupColorPacket(dest, source) dest = NULL; \
                                     if(source) { \
                                         dest = (pColorPacket)new char[sizeof(xColorPacket) + (source->numColors)*sizeof(tRGBColor)]; \
                                         memcpy(dest, source, sizeof(xColorPacket) + (source->numColors)*sizeof(tRGBColor)); \
                                     }

#define CopyColorPacket( source, dest)  memcpy(dest, source, sizeof(xColorPacket) + (source->numColors)*sizeof(tRGBColor) );


#define ZeroColorPacket( packet ) memset( &((packet)->zone[0]), 0, (packet->numColors)*sizeof(tRGBColor));

// --- tRGBColorLongInt -------------------------------------------------------
typedef struct
{
  long int r, g, b;
} tRGBColorLongInt;

// --- tColorPacketLongInt ----------------------------------------------------
typedef struct
{
  int numColors;
  tRGBColorLongInt longZone[1];
} xColorPacketLongInt;
typedef xColorPacketLongInt* pColorPacketLongInt;
#define AllocLongColorPacket(packet, numColors_) packet = (pColorPacketLongInt)new char[sizeof(xColorPacketLongInt) + (numColors_)*sizeof(tRGBColorLongInt)]; \
                                             packet->numColors = numColors_;

#define DupLongColorPacket(dest, source) dest = NULL; \
                                     if(source) { \
                                         dest = (pColorPacketLongInt)new char[sizeof(xColorPacketLongInt) + (source->numColors)*sizeof(tRGBColorLongInt)]; \
                                         memcpy(dest, source, sizeof(xColorPacketLongInt) + (source->numColors)*sizeof(tRGBColorLongInt)); \
                                     }
#define ZeroLongColorPacket( packet ) memset( &((packet)->longZone[0]), 0, (packet->numColors)*sizeof(tRGBColorLongInt));


// --- tWeightPacket ----------------------------------------------------------
/*
typedef struct
{
  int channel[CAP_MAX_NUM_ZONES];
} tWeightPacket;
*/

// --- tHSVColor --------------------------------------------------------------
typedef struct
{
  unsigned int h, s, v;
} tHSVColor;

#endif
