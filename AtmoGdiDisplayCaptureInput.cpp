/*
* AtmoGdiDisplayCaptureInput.cpp: Implements the picture source which grabs the desktop as source for the live view mode
* poor performance ... through some GDI unsolved problems, if there are overlays active or some other applications...
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id$
*/
#include "StdAfx.h"

#include "atmoGdiDisplayCaptureInput.h"
#include "AtmoTools.h"


CAtmoGdiDisplayCaptureInput::CAtmoGdiDisplayCaptureInput(CAtmoDynData *pAtmoDynData) : CAtmoInput(pAtmoDynData)
{

}

CAtmoGdiDisplayCaptureInput::~CAtmoGdiDisplayCaptureInput(void)
{
}

// Opens the input-device. Parameters (e.g. the device-name) can be given in 'param'.
// Returns true if the input-device was opened successfully.
ATMO_BOOL CAtmoGdiDisplayCaptureInput::Open() {

	CAtmoConfig *pAtmoConfig = m_pAtmoDynData->getAtmoConfig();
	CAtmoDisplays *pAtmoDisplays = m_pAtmoDynData->getAtmoDisplays();

	m_rowsPerFrame = pAtmoConfig->getLiveView_RowsPerFrame();
	if(m_rowsPerFrame < 1) m_rowsPerFrame = 1;
	if(m_rowsPerFrame >= CAP_HEIGHT) m_rowsPerFrame = CAP_HEIGHT - 1;
	m_CurrentFrame = 0; // halbbild counter

	// clear temp hsv image 
	memset(&HSV_Img, 0, IMAGE_SIZE * sizeof(tHSVColor) );

#ifndef UseGdiDesktopGetPixel
	// setup handles and stuff for screen copy ...
	m_hdcScreen      = GetDC(NULL);

	// used as device context for temporary screenshot bitmap
	m_hTempBitmapDC  = CreateCompatibleDC(m_hdcScreen);
#endif

	int displayNr = pAtmoConfig->getLiveView_DisplayNr();
	if(displayNr >= pAtmoDisplays->getCount())
		displayNr = 0; // drop back to display 0 ;-)
	TAtmoDisplayInfo dislayInfo = pAtmoDisplays->getDisplayInfo(displayNr);


	// here TODO insert Screen! - in the config we have the display number to use
	// get resolution of this display and its position in the world of the current
	// windows desktop device context....!!! todo!
	m_ScreenSourceRect.left   = dislayInfo.horz_ScreenPos + pAtmoConfig->getLiveView_HOverscanBorder();
	m_ScreenSourceRect.top    = dislayInfo.vert_ScreenPos + pAtmoConfig->getLiveView_VOverscanBorder();
	m_ScreenSourceRect.right  = m_ScreenSourceRect.left + dislayInfo.horz_res - (2 * pAtmoConfig->getLiveView_HOverscanBorder());
	m_ScreenSourceRect.bottom = m_ScreenSourceRect.top  + dislayInfo.vert_res - (2 * pAtmoConfig->getLiveView_VOverscanBorder());

	// calculate lShift and tShift?
	/*
	for what?
	ok ... lets do some basics... our temp picutre has 64 columns?
	our source (my desktop) is 1600 columns wide?
	... so i Have to grab every 64th Pixel?
	a simple calculation would result into
	destination column 0:   --> source column 0
	destincation column 63: --> source column 1575 ... so we will lose 25 pixel to the right?
	so m_lShift is the value we have to move the "Grab Raster to right" so that
	destination column 0:   --> source column 12
	destincation column 63: --> source column 1587 ... so we will lose 12 pixel to the right and 12 to the left.. a nice small ignored Border around...

	that same happens to the row position...
	*/

	// precalculate Pixel Coordinates for Capture to save some cpu time?
	int capture_area_width  = m_ScreenSourceRect.right - m_ScreenSourceRect.left;
	int capture_area_height = m_ScreenSourceRect.bottom - m_ScreenSourceRect.top;
	for(int y=1;y<=CAP_HEIGHT;y++) {
		m_iSrcRows[y-1] = ((y * capture_area_height) / (CAP_HEIGHT+1) );
	}
	for(int x=1;x<=CAP_WIDTH;x++) {
		m_iSrcCols[x-1] = ((x * capture_area_width) / (CAP_WIDTH+1) );
	}
	// 25.09.2007 Igor: + m_ScreenSourceRect.left +m_ScreenSourceRect.top --> ist falsch
	// weil die Koordinaten in m_iSrcCols und m_iSrcRows - beziehen sich ja schon auf die mittels m_ScreenSourceRect erzeugte
	// Kopie des Desktops Device Contexts ... thanks to MacGyver for debugging.

#ifndef UseGdiGetPixel
	// should be enough memory for one row of pixels! in each Color Depth?
	m_PixelBuffer = (unsigned char *)malloc((capture_area_width+1) * 4);
#endif

	// run this thread
	this->Run();
	return true;
}


// Closes the input-device.
// Returns true if the input-device was closed successfully.
ATMO_BOOL CAtmoGdiDisplayCaptureInput::Close(void) {

	// stop the thread
	this->Terminate();

#ifndef UseGdiDesktopGetPixel
	DeleteDC(m_hTempBitmapDC);
	ReleaseDC(NULL, m_hdcScreen);
#endif


#ifndef UseGdiGetPixel
	if(m_PixelBuffer!=NULL)
		free(m_PixelBuffer);
#endif

	return ATMO_TRUE;
}

//[TF] new method for calculating colors - instead of the whole desktop now only the necessary lines are captured
/*
void CAtmoGdiDisplayCaptureInput::CalcColorsNew()
{
tRGBColor pixelColor;
int xx = 0;
unsigned int col = 0;
int capture_area_width = (m_ScreenSourceRect.right-m_ScreenSourceRect.left);
HBITMAP hTempBitmap = CreateCompatibleBitmap(m_hdcScreen, capture_area_width, 1);
HGDIOBJ hOldBmp = SelectObject(m_hTempBitmapDC, hTempBitmap);

BITMAPINFO bmpInfo;
ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
GetDIBits(m_hTempBitmapDC, hTempBitmap, 0, 1, NULL, &bmpInfo, DIB_RGB_COLORS);
bmpInfo.bmiHeader.biCompression = BI_RGB;

switch (bmpInfo.bmiHeader.biBitCount)
{
case 8: // untested!
{
int nColors = bmpInfo.bmiHeader.biClrUsed ? bmpInfo.bmiHeader.biClrUsed : 1 << bmpInfo.bmiHeader.biBitCount;
for (int l = 0; l < m_rowsPerFrame; l++)
{
BitBlt(m_hTempBitmapDC, 0, 0, capture_area_width, 1, m_hdcScreen, m_ScreenSourceRect.left, m_iSrcRows[m_currentLine], SRCCOPY);
GetDIBits(m_hTempBitmapDC, hTempBitmap, 0, 1, m_PixelBuffer, &bmpInfo, DIB_RGB_COLORS);
int index = CAP_WIDTH * m_currentLine;
for (int x = 0; x < CAP_WIDTH; x++)
{
xx           = m_iSrcCols[x];
col			 = m_PixelBuffer[xx] % nColors;
pixelColor.b = bmpInfo.bmiColors[col].rgbBlue;
pixelColor.g = bmpInfo.bmiColors[col].rgbGreen;
pixelColor.r = bmpInfo.bmiColors[col].rgbRed;
HSV_Img[index++] = RGB2HSV(pixelColor);
}
}
break;
}
case 16:
{
for (int l = 0; l < m_rowsPerFrame; l++)
{
BitBlt(m_hTempBitmapDC, 0, 0, capture_area_width, 1, m_hdcScreen, m_ScreenSourceRect.left, m_iSrcRows[m_currentLine], SRCCOPY);
GetDIBits(m_hTempBitmapDC, hTempBitmap, 0, 1, m_PixelBuffer, &bmpInfo, DIB_RGB_COLORS);
int index = CAP_WIDTH * m_currentLine;
for (int x = 0; x < CAP_WIDTH; x++)
{
xx           = m_iSrcCols[x] * 2;
col			 = m_PixelBuffer[xx] + (m_PixelBuffer[xx+1]<<8);
pixelColor.b = (col & 0x1F)<<3;
pixelColor.g = (col & 0x3E0)>>2;
pixelColor.r = (col & 0x7C00)>>7;
HSV_Img[index++] = RGB2HSV(pixelColor);
}
}
break;
}
case 24:
{
for (int l = 0; l < m_rowsPerFrame; l++)
{
BitBlt(m_hTempBitmapDC, 0, 0, capture_area_width, 1, m_hdcScreen, m_ScreenSourceRect.left, m_iSrcRows[m_currentLine], SRCCOPY);
GetDIBits(m_hTempBitmapDC, hTempBitmap, 0, 1, m_PixelBuffer, &bmpInfo, DIB_RGB_COLORS);
int index = CAP_WIDTH * m_currentLine;
for (int x = 0; x < CAP_WIDTH; x++)
{
xx           = m_iSrcCols[x] * 3;
pixelColor.b = m_PixelBuffer[xx++];
pixelColor.g = m_PixelBuffer[xx++];
pixelColor.r = m_PixelBuffer[xx++];
HSV_Img[index++] = RGB2HSV(pixelColor);
}
m_currentLine++;
if (m_currentLine >= CAP_HEIGHT) m_currentLine = 0;
}
break;
}
case 32:
{
for (int l = 0; l < m_rowsPerFrame; l++)
{
BitBlt(m_hTempBitmapDC, 0, 0, capture_area_width, 1, m_hdcScreen, m_ScreenSourceRect.left, m_iSrcRows[m_currentLine], SRCCOPY);
GetDIBits(m_hTempBitmapDC, hTempBitmap, 0, 1, m_PixelBuffer, &bmpInfo, DIB_RGB_COLORS);
int index = CAP_WIDTH * m_currentLine;
for (int x = 0; x < CAP_WIDTH; x++)
{
xx           = m_iSrcCols[x] * 4;
pixelColor.b = m_PixelBuffer[xx++];
pixelColor.g = m_PixelBuffer[xx++];
pixelColor.r = m_PixelBuffer[xx++];
HSV_Img[index++] = RGB2HSV(pixelColor);
}
m_currentLine++;
if (m_currentLine >= CAP_HEIGHT) m_currentLine = 0;
}
break;
}

};

SelectObject(m_hTempBitmapDC, hOldBmp);
DeleteObject(hTempBitmap);
m_pAtmoDynData->getLivePacketQueue()->AddPacket(m_pAtmoColorCalculator->AnalyzeHSV(HSV_Img));
}
*/

void CAtmoGdiDisplayCaptureInput::CalcColors() 
{
	tRGBColor pixelColor;
#ifdef UseGdiGetPixel
	COLORREF pixel;
#ifdef UseGdiDesktopGetPixel 
	HDC hdcScreen;
#endif
#endif

	int xx,yy;

	int capture_area_width  = (m_ScreenSourceRect.right-m_ScreenSourceRect.left);
	int capture_area_height = (m_ScreenSourceRect.bottom-m_ScreenSourceRect.top);

#ifndef UseGdiDesktopGetPixel
	HBITMAP hTempBitmap  = CreateCompatibleBitmap(m_hdcScreen, capture_area_width, capture_area_height);
	HGDIOBJ hOldBmp = SelectObject(m_hTempBitmapDC, hTempBitmap);
#endif

#ifndef UseGdiGetPixel
	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

	GetDIBits(m_hTempBitmapDC,hTempBitmap,0,1,NULL,&bmpInfo,DIB_RGB_COLORS);

	//  bmpInfo.bmiHeader.biWidth  = capture_area_width;
	//  bmpInfo.bmiHeader.biHeight = -capture_area_height;

	if(bmpInfo.bmiHeader.biSizeImage<=0)
		bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth * abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;

	bmpInfo.bmiHeader.biCompression=BI_RGB;
#endif


	//    have a look into VncDesktop.cpp :-) **g vncDesktop::CaptureScreen
	//    vncDesktop::EnableOptimisedBlits() vncDesktop::CopyToBuffer(
	//    http://cboard.cprogramming.com/archive/index.php/t-89037.html
	//    http://cboard.cprogramming.com/showthread.php?t=76907   das schaut gut aus!!!!!!
	//    damit spart man die GetDIBits(...) aufrufe... und bekommt mittelsBitBlit gleich alles
	//    ins eigene Ram Kopiert... Full Access to display ram... :-)))
	//

#ifndef UseGdiDesktopGetPixel
	BitBlt(m_hTempBitmapDC, 0, 0, capture_area_width, capture_area_height, m_hdcScreen, m_ScreenSourceRect.left, m_ScreenSourceRect.top, SRCCOPY);
#endif

	int index = (m_CurrentFrame * CAP_WIDTH);
	int indexSkip = ((m_rowsPerFrame-1) * CAP_WIDTH);
	unsigned int col = 0;

#ifdef UseGdiGetPixel
#ifdef UseGdiDesktopGetPixel
	hdcScreen      = GetDC(NULL);  
#endif


	for(int y=m_CurrentFrame; y<CAP_HEIGHT; y+=m_rowsPerFrame ) {
		// yy = (y * capture_area_height) / CAP_HEIGHT;
		// yy = yy + m_ScreenSourceRect.top + m_tShift;
		yy = m_iSrcRows[y];

		for(int x=0;x<CAP_WIDTH;x++) {
			// xx = (x * capture_area_width) / CAP_WIDTH;
			// xx = xx + m_ScreenSourceRect.left + m_lShift;
#ifndef UseGdiDesktopGetPixel
			pixel = GetPixel(m_hTempBitmapDC, m_iSrcCols[x], yy);
#else
			pixel = GetPixel(hdcScreen, m_iSrcCols[x], yy);
#endif
			pixelColor.r = GetRValue(pixel);
			pixelColor.g = GetGValue(pixel);
			pixelColor.b = GetBValue(pixel);

			HSV_Img[index++] = RGB2HSV(pixelColor);
		}
		index += indexSkip;
	}

#ifdef UseGdiDesktopGetPixel
	ReleaseDC(NULL, hdcScreen);
#endif


#else
	switch(bmpInfo.bmiHeader.biBitCount) {
	case  8: { // [TF] 8bit support added by Tobias Fleischer/Tobybear - still untested and experimental, might not work!
		int nColors = bmpInfo.bmiHeader.biClrUsed ? bmpInfo.bmiHeader.biClrUsed : 1 << bmpInfo.bmiHeader.biBitCount;
		for( int y=m_CurrentFrame; y<CAP_HEIGHT; y+=m_rowsPerFrame ) {
			// yy = (y * capture_area_height) / CAP_HEIGHT;
			// yy = yy + m_ScreenSourceRect.top + m_tShift;

			if(bmpInfo.bmiHeader.biHeight>0)
				yy = bmpInfo.bmiHeader.biHeight - m_iSrcRows[y] - 1;
			else
				yy = m_iSrcRows[y];


			// eine Bildzeile in meinen Arbeitsbuffer kopieren!
			// geht vielfach schneller als GetPixel aufrufen... wenn man dein Speicher dann direkt
			// zugreift... :-)
			GetDIBits(m_hTempBitmapDC,hTempBitmap,yy,1,m_PixelBuffer,&bmpInfo,DIB_RGB_COLORS);
			for(int x=0; x<CAP_WIDTH; x++) {
				xx           = m_iSrcCols[x];
				col			 = m_PixelBuffer[xx] % nColors;
				pixelColor.b = bmpInfo.bmiColors[col].rgbBlue;
				pixelColor.g = bmpInfo.bmiColors[col].rgbGreen;
				pixelColor.r = bmpInfo.bmiColors[col].rgbRed;
				HSV_Img[index++] = RGB2HSV(pixelColor);					
			}
			index += indexSkip;
		}
		break;
					 }

	case 16: { // [TF] 16bit support added by Tobias Fleischer/Tobybear - tested
		for(int y=m_CurrentFrame; y<CAP_HEIGHT; y+=m_rowsPerFrame ) {
			// yy = (y * capture_area_height) / CAP_HEIGHT;
			// yy = yy + m_ScreenSourceRect.top + m_tShift;

			if(bmpInfo.bmiHeader.biHeight>0)
				yy = bmpInfo.bmiHeader.biHeight - m_iSrcRows[y] - 1;
			else
				yy = m_iSrcRows[y];


			// eine Bildzeile in meinen Arbeitsbuffer kopieren!
			// geht vielfach schneller als GetPixel aufrufen... wenn man dein Speicher dann direkt
			// zugreift... :-)
			GetDIBits(m_hTempBitmapDC,hTempBitmap,yy,1,m_PixelBuffer,&bmpInfo,DIB_RGB_COLORS);
			for(int x=0;x<CAP_WIDTH;x++) {
				xx           = m_iSrcCols[x] * 2;
				col = m_PixelBuffer[xx] + (m_PixelBuffer[xx+1]<<8);
				pixelColor.b = (col & 0x1F)<<3;
				pixelColor.g = (col & 0x3E0)>>2;
				pixelColor.r = (col & 0x7C00)>>7;
				HSV_Img[index++] = RGB2HSV(pixelColor);
			}
			index += indexSkip;
		}
		break;
					 }

	case 24: {
		for(int y=m_CurrentFrame; y<CAP_HEIGHT; y+=m_rowsPerFrame ) {
			// yy = (y * capture_area_height) / CAP_HEIGHT;
			// yy = yy + m_ScreenSourceRect.top + m_tShift;


			if(bmpInfo.bmiHeader.biHeight>0)
				yy = bmpInfo.bmiHeader.biHeight - m_iSrcRows[y] - 1;
			else
				yy = m_iSrcRows[y];


			// eine Bildzeile in meinen Arbeitsbuffer kopieren!
			// geht vielfach schneller als GetPixel aufrufen... wenn man dein Speicher dann direkt
			// zugreift... :-)
			GetDIBits(m_hTempBitmapDC,hTempBitmap,yy,1,m_PixelBuffer,&bmpInfo,DIB_RGB_COLORS);
			for(int x=0;x<CAP_WIDTH;x++) {
				xx           = m_iSrcCols[x] * 3;
				pixelColor.b = m_PixelBuffer[xx++];
				pixelColor.g = m_PixelBuffer[xx++];
				pixelColor.r = m_PixelBuffer[xx++];
				HSV_Img[index++] = RGB2HSV(pixelColor);
			}
			index += indexSkip;
		}
		break;
					 }

	case 32: {
		for(int y=m_CurrentFrame; y<CAP_HEIGHT; y+=m_rowsPerFrame ) {
			if(bmpInfo.bmiHeader.biHeight>0)
				yy = bmpInfo.bmiHeader.biHeight - m_iSrcRows[y] - 1;
			else
				yy = m_iSrcRows[y];

			// eine Bildzeile in meinen Arbeitsbuffer kopieren!
			// geht vielfach schneller als GetPixel aufrufen... wenn man dein Speicher dann direkt
			// zugreift... :-)
			GetDIBits(m_hTempBitmapDC,hTempBitmap,yy,1,m_PixelBuffer,&bmpInfo,DIB_RGB_COLORS);

			for(int x=0;x<CAP_WIDTH;x++) {
				// xx = (x * capture_area_width) / CAP_WIDTH;
				// xx = xx + m_ScreenSourceRect.left + m_lShift;
				xx = m_iSrcCols[x] * 4;
				pixelColor.b = m_PixelBuffer[xx++];
				pixelColor.g = m_PixelBuffer[xx++];
				pixelColor.r = m_PixelBuffer[xx++];
				HSV_Img[index++] = RGB2HSV(pixelColor);
			}
			index += indexSkip;
		}
		break;
					 }
	}

#endif

#ifndef UseGdiDesktopGetPixel
	SelectObject(m_hTempBitmapDC, hOldBmp);
	DeleteObject(hTempBitmap);
#endif

	m_CurrentFrame++;
	if(m_CurrentFrame >= m_rowsPerFrame) 
		m_CurrentFrame = 0;

	m_pAtmoDynData->getLivePacketQueue()->AddPacket( m_pAtmoColorCalculator->AnalyzeHSV( HSV_Img ) );
}


DWORD CAtmoGdiDisplayCaptureInput::Execute(void) {
	// process Screen Capturing... every x ms ...
	DWORD tickCount;
	DWORD sleepMs; 
	CAtmoConfig *pAtmoConfig = m_pAtmoDynData->getAtmoConfig();
	int fps = pAtmoConfig->getLiveView_GDI_FrameRate();
	if (fps < 1) fps = 1;
	if (fps > 100) fps = 100;

	sleepMs = DWORD(1000 / fps);

	// in this loop the picture is read and the colors are calculated
	while (this->m_bTerminated == ATMO_FALSE)
	{
		tickCount = GetTickCount();

		// [TF] if new parameter "rowsPerFrame" is set, use new method for getting colors
		// if (m_rowsPerFrame > 0)
		//	CalcColorsNew();
		//else

		CalcColors(); // read picture and calculate colors

		tickCount = GetTickCount() - tickCount;

		if (tickCount < sleepMs) // ensure that this loop takes at least 50ms! so we will get 20 Frames per Second
		{
			if(this->ThreadSleep(sleepMs - tickCount) == ATMO_FALSE)  // sleep at least 10ms!
				break;// thread was terminated durring waiting... oops
		}
	}

	return 0;
}

