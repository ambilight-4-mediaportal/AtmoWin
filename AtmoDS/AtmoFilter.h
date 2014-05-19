//------------------------------------------------------------------------------
// File: AtmoFilter.h
//
// Desc: DirectShow sample code - header for RGBFilters component
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#include <comdef.h>
#include "..\AtmoWin_h.h"

extern const AMOVIESETUP_FILTER sudAtmoFilter;

class CAtmoLightFilter : public CTransInPlaceFilter
{
public:
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

    DECLARE_IUNKNOWN;

    HRESULT Transform(IMediaSample *pSample);
    HRESULT CheckInputType(const CMediaType *mtIn);
    HRESULT SetMediaType( PIN_DIRECTION pindir, const CMediaType *pMediaType);
    HRESULT StartStreaming();
    HRESULT StopStreaming();
    HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);

    LPAMOVIESETUP_FILTER GetSetupData();

    void log( const char *psz_format, ... );

private:
    // Constructor
    CAtmoLightFilter( LPUNKNOWN punk, HRESULT *phr );
    ~CAtmoLightFilter( );

    void LogMediaSubType(char *pszPrefix, GUID &subType);
    void LogMediaType(char *pszPrefix, const CMediaType *temp);

    bool m_AtmoInitialized;
    ComEffectMode m_CurrentAtmoEffect;
    int m_atmo_capture_width;
    int m_atmo_capture_height;
    int atmo_capture_width;
    int atmo_capture_height;
    char *m_pszAtmoWin;
    
    int m_LogLevel; // 0 none, 1 logfile, 2 logfile & sample images to logfile folder!
    char *m_pszLogFileName;
    char *m_pszImageLogPath;

    void (CAtmoLightFilter::*m_pfTransform)(IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );

    void Transform_Packed_YUY2_YUYV( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Packed_Y411( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Packed_Y41P( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Packed_YVYU( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Packed_UYVY( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer ); 
    void Transform_Packed_Y211( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Packed_CLJR( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    
    void Transform_Planar_IYUV_I420( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Planar_YV12( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Planar_YVU9( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Planar_NV12( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_Planar_NV21( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    

    void Transform_RGB_565( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_RGB_555( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_RGB_888( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );
    void Transform_RGB_8888( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer );


    SAFEARRAY            *m_pixel_buffer; 
    SAFEARRAY            *m_bitmap_header;

    FILE *m_log_file;


    GUID        m_mediaSubType;
    int         m_FrameCounter;
};

