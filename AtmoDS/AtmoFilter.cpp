//------------------------------------------------------------------------------
// Based on File: TransSmpte.cpp
//
// Desc: DirectShow sample code - implementation of RGBFilters sample filters
//
// Copyright (c) 2000-2002  Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "iAtmoFilters.h"
#include "AtmoFilter.h"
#include <stdarg.h>
#include <math.h>

#include "mtype.h"
#include "..\AtmoRegistry.h"

#include <comdef.h>
#include "..\AtmoWin_h.h"

#define ATMODS_REG_KEY     "SOFTWARE\\AtmoWinX\\dshow\\"

#define GUID_FMT           "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x "
#define GUID_PRINT(guid)   (guid).Data1,(guid).Data2,(guid).Data3,(guid).Data4[0],(guid).Data4[1],(guid).Data4[2],(guid).Data4[3],(guid).Data4[4],(guid).Data4[5],(guid).Data4[6],(guid).Data4[7]


/*
  some formats not defined in dshow headers
*/
const GUID MEDIASUBTYPE_I420 = {0x30323449, 0x0000, 0x0010,  {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_NV21 = {0x3132564E, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
                               
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudpPins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    }
};
const AMOVIESETUP_FILTER sudAtmoFilter =
{
    &CLSID_AtmoLightFilter,   // Filter CLSID
    L"AtmoLight",             // String name
    MERIT_DO_NOT_USE,         // Filter merit
    2,                        // Number pins
    sudpPins                  // Pin details
};

CUnknown * WINAPI CAtmoLightFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    ASSERT(phr);
    
    CUnknown *punk = new CAtmoLightFilter(lpunk, phr);
    if (punk == NULL) 
    {
        *phr = E_OUTOFMEMORY;
    }
    return punk;

}


IAtmoRemoteControl *getAtmoRemoteControl() 
{
    IUnknown             *pIUnknown = NULL;
    IAtmoRemoteControl   *pAtmoRemoteControl = NULL;
    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown != NULL) {
       pIUnknown->QueryInterface(IID_IAtmoRemoteControl,(void **)&pAtmoRemoteControl);  
    }
    return pAtmoRemoteControl;
}

IAtmoRemoteControl2 *getAtmoRemoteControl2() 
{
    IUnknown             *pIUnknown = NULL;
    IAtmoRemoteControl2   *pAtmoRemoteControl2 = NULL;
    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown != NULL) {
       pIUnknown->QueryInterface(IID_IAtmoRemoteControl2,(void **)&pAtmoRemoteControl2);  
    }
    return pAtmoRemoteControl2;
}

IAtmoLiveViewControl *getAtmoLiveViewControl()
{
    IUnknown              *pIUnknown = NULL;
    IAtmoLiveViewControl  *pAtmoLiveViewControl = NULL;
    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown != NULL) {
       pIUnknown->QueryInterface(IID_IAtmoLiveViewControl,(void **)&pAtmoLiveViewControl);  
    }
    return pAtmoLiveViewControl;
}

CAtmoLightFilter::CAtmoLightFilter(LPUNKNOWN punk,HRESULT *phr) 
    : CTransInPlaceFilter(TEXT("AtmoLight"), punk, CLSID_AtmoLightFilter, phr)
    , m_mediaSubType(GUID_NULL)
    , m_pixel_buffer(NULL)
    , m_bitmap_header(NULL)
    , m_FrameCounter(0)
    , m_AtmoInitialized(false)
    , m_pfTransform(NULL)
    , m_pszAtmoWin(NULL)
    , m_pszLogFileName(NULL)
    , m_pszImageLogPath(NULL)
    , m_CurrentAtmoEffect(cemUndefined)
{

    char pszWorkDir[MAX_PATH]; pszWorkDir[0] = 0;
    if( GetModuleFileName(g_hInst, pszWorkDir, MAX_PATH) != 0)
    {
        // pszDllName --> c:\AtmoWin\atmoDS.dll  
        size_t i = strlen(pszWorkDir);
        while ((i>0) && (pszWorkDir[i] != '\\')) i--;
        pszWorkDir[i+1] = 0; 
    }

    CAtmoRegistry *reg = new CAtmoRegistry(HKEY_LOCAL_MACHINE);

    m_LogLevel        = reg->ReadRegistryInt( ATMODS_REG_KEY ,"log_level", 0);
    m_pszLogFileName  = reg->ReadRegistryString( ATMODS_REG_KEY ,"log_file","");
    if(strlen(m_pszLogFileName) == 0) {
        
       free(m_pszLogFileName);
       m_pszLogFileName = (char *)malloc( strlen(pszWorkDir) +  16 );
       strcpy(m_pszLogFileName, pszWorkDir);
       strcat(m_pszLogFileName, "atmo_ds.log");
    }

    m_pszImageLogPath = reg->ReadRegistryString( ATMODS_REG_KEY , "log_path","");
    if(strlen(m_pszImageLogPath) == 0) {
       free(m_pszImageLogPath);
       m_pszImageLogPath = strdup( pszWorkDir );
    }

    if( m_LogLevel > 0 )
    {
       m_log_file = fopen( m_pszLogFileName , "at");
       if(m_log_file)
          setvbuf( m_log_file, NULL, _IONBF, 0 );
    } else {
       m_log_file = NULL; 
    }

    m_pszAtmoWin = reg->ReadRegistryString( ATMODS_REG_KEY ,"exe", "");
    if(strlen(m_pszAtmoWin) == 0) 
    {
       char pszExeName[MAX_PATH]; pszExeName[0] = 0;
       strcpy(pszExeName, pszWorkDir );
       strcat(pszExeName, "AtmoWinA.exe");

       free(m_pszAtmoWin);
       m_pszAtmoWin = strdup( pszExeName );
    }
    delete reg;

    m_atmo_capture_width  = 64;
    m_atmo_capture_height = 48;
    atmo_capture_height   = m_atmo_capture_height + 1;
    atmo_capture_width    = m_atmo_capture_width + 1;

    log("Filter Object created\n");

    OleInitialize(NULL);

    IAtmoRemoteControl *remoteControl = getAtmoRemoteControl();
    if(!remoteControl)
    {
      log("try Atmo: \"%s\"\n", m_pszAtmoWin );
      
      STARTUPINFO startupinfo;
      PROCESS_INFORMATION pinfo;
      memset(&startupinfo, 0, sizeof(STARTUPINFO));
      startupinfo.cb = sizeof(STARTUPINFO);
      if(CreateProcess(m_pszAtmoWin, NULL, NULL, NULL,
            FALSE, 0, NULL, NULL, &startupinfo, &pinfo) == TRUE)
        {
            log("launched AtmoWin Software...\n");
            WaitForInputIdle(pinfo.hProcess, 5000);
            
        } else {
           DWORD lasterror = GetLastError();  
           log("failed to launch AtmoWin code=0x%x\n",lasterror);           
        }
    } else {
        log("AtmoWin Software is running ... ok.\n");
        remoteControl->Release();
    }

    OleUninitialize();

}

CAtmoLightFilter::~CAtmoLightFilter( )
{
    OleInitialize(NULL); 

    log("destroying atmo filtern\n");

    if(m_AtmoInitialized)
    {
        IAtmoRemoteControl *remoteControl = getAtmoRemoteControl();
        if(remoteControl)
        {
        if( m_CurrentAtmoEffect != cemLivePicture)
        {
            remoteControl->setEffect( m_CurrentAtmoEffect, &m_CurrentAtmoEffect );
        } else {
            IAtmoLiveViewControl *liveControl = getAtmoLiveViewControl();
            if(liveControl)
                {
                    liveControl->setLiveViewSource( lvsGDI );
                    liveControl->Release();
                }
        }
        remoteControl->Release(); 
        }
        if(m_pixel_buffer)
           SafeArrayDestroy( m_pixel_buffer );

        if(m_bitmap_header)
           SafeArrayDestroy( m_bitmap_header );
    }

    OleUninitialize();
    log("atmo filter destroyed\n\n\n");

    if(m_log_file)
       fclose( m_log_file );
    
    free( m_pszAtmoWin );
    free( m_pszLogFileName );
    free( m_pszImageLogPath );
}

void CAtmoLightFilter::log( const char *psz_format, ... )
{
    if(m_log_file != NULL)
    {
        va_list args;
        va_start( args, psz_format );
        vfprintf( m_log_file, psz_format, args );
        va_end( args );
    }
}

void CAtmoLightFilter::LogMediaType(char *pszPrefix,const CMediaType *temp)
{
    BITMAPINFOHEADER *bmiHeader = NULL;

    GUID format = *temp->FormatType();
    log("%s: MediaType.FormatType: " GUID_FMT "\n", pszPrefix, GUID_PRINT(format) );

    if( IsEqualGUID( *temp->FormatType(), FORMAT_VideoInfo) ) 
    {
        VIDEOINFO* pVI = (VIDEOINFO*) temp->Format( );

        log("%s: FORMAT_VideoInfo Image Size Source(%d,%d,%d,%d) Target: (%d,%d,%d,%d) \n", pszPrefix,
               pVI->rcSource.left, pVI->rcSource.top, pVI->rcSource.right, pVI->rcSource.bottom,
               pVI->rcTarget.left, pVI->rcTarget.top, pVI->rcTarget.right, pVI->rcTarget.bottom
           ) ;


        if(pVI->bmiHeader.biSize >= sizeof(BITMAPINFOHEADER)) 
           bmiHeader = &pVI->bmiHeader;
    } else
    if( IsEqualGUID( *temp->FormatType(), FORMAT_VideoInfo2) ) 
    {
        VIDEOINFOHEADER2* pVI = (VIDEOINFOHEADER2*) temp->Format( );
        

        log("%s: FORMAT_VideoInfo2 Image Size Source(%d,%d,%d,%d) Target: (%d,%d,%d,%d) Aspect: %d : %d\n", pszPrefix,
               pVI->rcSource.left, pVI->rcSource.top, pVI->rcSource.right, pVI->rcSource.bottom,
               pVI->rcTarget.left, pVI->rcTarget.top, pVI->rcTarget.right, pVI->rcTarget.bottom,
               pVI->dwPictAspectRatioX,
               pVI->dwPictAspectRatioY
           ) ;

        if(pVI->bmiHeader.biSize >= sizeof(BITMAPINFOHEADER))
           bmiHeader = &pVI->bmiHeader;
    }

    GUID tmp = *temp->Subtype();
    LogMediaSubType( pszPrefix, tmp );

    if(bmiHeader)
    {
        log("%s: biSize: %d, biSizeImage: %d, Width: %d, Height: %d, Planes: %d, BitCount: %d, Fourcc: %4.4s, \n", 
            pszPrefix, 
            bmiHeader->biSize, bmiHeader->biSizeImage,
            bmiHeader->biWidth, bmiHeader->biHeight, 
            bmiHeader->biPlanes, 
            bmiHeader->biBitCount,
            (char *)&bmiHeader->biCompression);
    }
}

void CAtmoLightFilter::LogMediaSubType(char *pszPrefix, GUID &subType)
{
    log("%s: Subtype " GUID_FMT "(aka Pixelformat)\n",pszPrefix, GUID_PRINT(subType) );

    if(IsEqualGUID( subType, MEDIASUBTYPE_CLPL ))
        log("%s: MediaSubType: MEDIASUBTYPE_CLPL\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_YV12 ))
       log("%s: MediaSubType: MEDIASUBTYPE_YV12 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_YUYV ))    // done! = YUY2
       log("%s: MediaSubType: MEDIASUBTYPE_YUYV should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_IYUV )) // done I420 Planar!
       log("%s: MediaSubType: MEDIASUBTYPE_IYUV should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_I420 )) // done I420 Planar!
       log("%s: MediaSubType: MEDIASUBTYPE_I420 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_YVU9 )) // done!
       log("%s: MediaSubType: MEDIASUBTYPE_YVU9 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_Y411 )) // done!
       log("%s: MediaSubType: MEDIASUBTYPE_Y411 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_Y41P ))
       log("%s: MediaSubType: MEDIASUBTYPE_Y41P should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_NV12 ))
        log("%s: MediaSubType: MEDIASUBTYPE_NV12 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_NV21 ))
        log("%s: MediaSubType: MEDIASUBTYPE_NV21 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_YUY2 )) // done!
       log("%s: MediaSubType: MEDIASUBTYPE_YUY2 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_YVYU ))
       log("%s: MediaSubType: MEDIASUBTYPE_YVYU should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_UYVY ))
       log("%s: MediaSubType: MEDIASUBTYPE_UYVY should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_Y211 ))
       log("%s: MediaSubType: MEDIASUBTYPE_Y211 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_CLJR ))
       log("%s: MediaSubType: MEDIASUBTYPE_CLJR should work\n", pszPrefix); 
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_IF09 )) // ?
       log("%s: MediaSubType: MEDIASUBTYPE_IF09\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_CPLA )) // ?
       log("%s: MediaSubType: MEDIASUBTYPE_CPLA\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_MJPG )) // ?
       log("%s: MediaSubType: MEDIASUBTYPE_MJPG\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_TVMJ ))
       log("%s: MediaSubType: MEDIASUBTYPE_TVMJ\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_WAKE ))
       log("%s: MediaSubType: MEDIASUBTYPE_WAKE\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_CFCC ))
       log("%s: MediaSubType: MEDIASUBTYPE_CFCC\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_IJPG ))
       log("%s: MediaSubType: MEDIASUBTYPE_IJPG\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_Plum ))
       log("%s: MediaSubType: MEDIASUBTYPE_Plum\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_DVCS ))
       log("%s: MediaSubType: MEDIASUBTYPE_DVCS\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_DVSD ))
       log("%s: MediaSubType: MEDIASUBTYPE_DVSD\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_MDVF ))
       log("%s: MediaSubType: MEDIASUBTYPE_MDVF\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_RGB1 ))
       log("%s: MediaSubType: MEDIASUBTYPE_RGB1\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_RGB4 ))
       log("%s: MediaSubType: MEDIASUBTYPE_RGB4\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_RGB8 ))
       log("%s: MediaSubType: MEDIASUBTYPE_RGB8\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_RGB565 ))
       log("%s: MediaSubType: MEDIASUBTYPE_RGB565 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_RGB555 ))
       log("%s: MediaSubType: MEDIASUBTYPE_RGB555 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_RGB24 ))
       log("%s: MediaSubType: MEDIASUBTYPE_RGB24 should work\n", pszPrefix);
    else
    if(IsEqualGUID( subType, MEDIASUBTYPE_RGB32 ))
       log("%s: MediaSubType: MEDIASUBTYPE_RGB32 should work\n", pszPrefix);
    else
       log("%s: MediaSubType seems to be unknown to me??\n", pszPrefix);
}

LPAMOVIESETUP_FILTER CAtmoLightFilter::GetSetupData()
{
    return (LPAMOVIESETUP_FILTER) &sudAtmoFilter;
}

HRESULT CAtmoLightFilter::CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin)
{
    HRESULT temp = CTransInPlaceFilter::CompleteConnect( dir, pReceivePin );
    if(temp == NOERROR)
    {
       // log("CAtmoLightFilter::CompleteConnect\n");
       // LogMediaType("CompleteConnect", &m_pInput->CurrentMediaType());
    }
    return temp;
}

HRESULT CAtmoLightFilter::StartStreaming()
{
    log("\n\nCAtmoLightFilter::StartStreaming\n");

    if(!m_AtmoInitialized)
    {
        OleInitialize(NULL);

        IAtmoRemoteControl *remoteControl = getAtmoRemoteControl();
        if(remoteControl)
        {
            log("atmo: found running atmowin\n");
            remoteControl->getEffect( &m_CurrentAtmoEffect );
            if(m_CurrentAtmoEffect != cemLivePicture) {
                remoteControl->setEffect( cemLivePicture, &m_CurrentAtmoEffect );
                log("set live mode\n");
            }

            IAtmoRemoteControl2 *remoteControl2 = getAtmoRemoteControl2();
            if(remoteControl2)
            {
                remoteControl2->getLiveViewRes(&m_atmo_capture_width,&m_atmo_capture_height);
                remoteControl2->Release();
                atmo_capture_height = m_atmo_capture_height + 1;
                atmo_capture_width  = m_atmo_capture_width + 1;
                log("atmo: got res %d x %d\n", m_atmo_capture_width, m_atmo_capture_height );
            }

            IAtmoLiveViewControl *liveControl = getAtmoLiveViewControl();
            if(liveControl)
            {
                liveControl->setLiveViewSource( lvsExternal );
                liveControl->Release();
                log("atmo: set live source extern\n");
            }

            remoteControl->Release();

        //  m_atmo_capture_width  = 512;
        //    m_atmo_capture_height = 384;


            m_pixel_buffer  = SafeArrayCreateVector(VT_UI1,0, m_atmo_capture_width * m_atmo_capture_height * 4);
            if(!m_pixel_buffer)
                log("Error: SafeArrayCreateVector Pixelbuffer failed.");
            m_bitmap_header = SafeArrayCreateVector(VT_UI1,0,sizeof(BITMAPINFOHEADER));
            if(!m_bitmap_header)
                log("Error: SafeArrayCreateVector BitmapHeader failed.");

            BITMAPINFOHEADER *header;
            SafeArrayAccessData(m_bitmap_header,(void **)&header);
            header->biSize        = sizeof(BITMAPINFOHEADER);
            header->biWidth       = m_atmo_capture_width;
            header->biHeight      = m_atmo_capture_height;
            header->biBitCount    = 32;
            header->biCompression = BI_RGB;
            SafeArrayUnaccessData(m_bitmap_header);
            
            m_AtmoInitialized = true;
        }
        OleUninitialize();
    
    }


  return CTransInPlaceFilter::StartStreaming();
}

HRESULT CAtmoLightFilter::StopStreaming()
{
  log("CAtmoLightFilter::StopStreaming\n\n");
  return CTransInPlaceFilter::StopStreaming();
}

HRESULT CAtmoLightFilter::SetMediaType( PIN_DIRECTION pindir, const CMediaType *pMediaType)
{
    CheckPointer(pMediaType,E_POINTER);

/*
    1B81BE68-A0C7-11D3-B984-00C04F2E73C5   dxva h264_E
    604F8E68-4951-4c54-88FE-ABD25C15B3D6   DXVADDI_Intel_ModeH264_E  
*/

    LogMediaType("SetMediaType", pMediaType );

    m_mediaSubType = *pMediaType->Subtype();

    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_YUY2) || IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_YUYV) )
        m_pfTransform = &CAtmoLightFilter::Transform_Packed_YUY2_YUYV;
    else  
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_Y411) )
        m_pfTransform = &CAtmoLightFilter::Transform_Packed_Y411;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_Y41P) )
        m_pfTransform = &CAtmoLightFilter::Transform_Packed_Y41P;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_YVYU) )
        m_pfTransform = &CAtmoLightFilter::Transform_Packed_YVYU;
    else 
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_NV12) )
        m_pfTransform = &CAtmoLightFilter::Transform_Planar_NV12;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_NV21) )
        m_pfTransform = &CAtmoLightFilter::Transform_Planar_NV21;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_UYVY) )
        m_pfTransform = &CAtmoLightFilter::Transform_Packed_UYVY;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_Y211) )
        m_pfTransform = &CAtmoLightFilter::Transform_Packed_Y211;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_CLJR) )
        m_pfTransform = &CAtmoLightFilter::Transform_Packed_CLJR;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_IYUV ) || IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_I420 ) ) 
        m_pfTransform = &CAtmoLightFilter::Transform_Planar_IYUV_I420;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_YV12 ) )  // YVU
        m_pfTransform = &CAtmoLightFilter::Transform_Planar_YV12;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_YVU9 ) )  // YVU9 I411 format 
        m_pfTransform = &CAtmoLightFilter::Transform_Planar_YVU9;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_RGB565 ) )  // RGB 16bit 5-6-5
        m_pfTransform = &CAtmoLightFilter::Transform_RGB_565;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_RGB555 ) )  // RGB 15bit 5-5-5
        m_pfTransform = &CAtmoLightFilter::Transform_RGB_555;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_RGB24 ) )  // RGB 24bit 8-8-8
        m_pfTransform = &CAtmoLightFilter::Transform_RGB_888;
    else
    if( IsEqualGUID( m_mediaSubType, MEDIASUBTYPE_RGB32 ) )  // RGB 24bit 8-8-8+Alpha?
        m_pfTransform = &CAtmoLightFilter::Transform_RGB_8888;
    else
        m_pfTransform = NULL;

    return CTransInPlaceFilter::SetMediaType( pindir, pMediaType );
}


HRESULT CAtmoLightFilter::CheckInputType(const CMediaType *mtIn)
{
    CheckPointer(mtIn,E_POINTER);

    log("CAtmoLightFilter::CheckInputType()\n");
    LogMediaType("CheckInputType", mtIn );

    GUID guid = *mtIn->Type();
    if( !IsEqualGUID(guid, MEDIATYPE_Video) )
    {
        log("CheckInputType: Mediatype is not MEDIATYPE_Video! ->" GUID_FMT "\n", GUID_PRINT(guid) );
        return E_INVALIDARG;
    }
    
    if( !IsEqualGUID( *mtIn->FormatType(), FORMAT_VideoInfo2) && !IsEqualGUID( *mtIn->FormatType(), FORMAT_VideoInfo) )
    {
       log("CheckInputType: FormatType is not VideoInfo2 or VideoInfo!\n");
       return E_INVALIDARG;
    }

    GUID mediaSubType = *mtIn->Subtype();

    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_YUY2) || IsEqualGUID( mediaSubType, MEDIASUBTYPE_YUYV) )
        return NOERROR;
    else  
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_Y411) )
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_Y41P) )
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_YVYU) )
        return NOERROR;
    else 
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_NV12) )
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_NV21) )
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_UYVY) )
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_Y211) )
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_CLJR) )
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_IYUV ) || IsEqualGUID( mediaSubType, MEDIASUBTYPE_I420 ) ) 
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_YV12 ) )  // YVU
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_YVU9 ) )  // YVU9 I411 format 
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_RGB565 ) )  // RGB 16bit 5-6-5
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_RGB555 ) )  // RGB 15bit 5-5-5
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_RGB24 ) )  // RGB 24bit 8-8-8
        return NOERROR;
    else
    if( IsEqualGUID( mediaSubType, MEDIASUBTYPE_RGB32 ) )  // RGB 24bit 8-8-8+Alpha?
        return NOERROR;
    else
        return E_INVALIDARG;
}

#define uint8_t unsigned char

static inline void yuv_to_rgb( uint8_t *r, uint8_t *g, uint8_t *b,
                              uint8_t y1, uint8_t u1, uint8_t v1 )
{
    /* macros used for YUV pixel conversions */
#   define SCALEBITS 10
#   define ONE_HALF  (1 << (SCALEBITS - 1))
#   define FIX(x)    ((int) ((x) * (1<<SCALEBITS) + 0.5))
#   define CLAMP( x ) (((x) > 255) ? 255 : ((x) < 0) ? 0 : (x));

    int y, cb, cr, r_add, g_add, b_add;

    cb = u1 - 128;
    cr = v1 - 128;
    r_add = FIX(1.40200*255.0/224.0) * cr + ONE_HALF;
    g_add = - FIX(0.34414*255.0/224.0) * cb
        - FIX(0.71414*255.0/224.0) * cr + ONE_HALF;
    b_add = FIX(1.77200*255.0/224.0) * cb + ONE_HALF;
    y = (y1 - 16) * FIX(255.0/219.0);
    *r = CLAMP((y + r_add) >> SCALEBITS);
    *g = CLAMP((y + g_add) >> SCALEBITS);
    *b = CLAMP((y + b_add) >> SCALEBITS);
}

#define MakeIntelWord(ch1,ch2)  ((((int)(ch1)&255)<<8) | \
                           ((int)(ch2)&255))


void SaveBitmap(char *fileName,unsigned char *pBuf, int w, int h)
{
     if(!fileName || !pBuf) return;
     FILE *fp = NULL;
     fp = fopen(fileName,"wb");
     if(fp != NULL) 
     {
        BITMAPINFO bmpinfo;
        // bitmapinfoheader 
        BITMAPFILEHEADER  bmpfileheader;
        ZeroMemory(&bmpinfo, sizeof(BITMAPINFO));
        bmpinfo.bmiHeader.biSize=sizeof(BITMAPFILEHEADER);

            
        bmpinfo.bmiHeader.biHeight = -h;
        bmpinfo.bmiHeader.biWidth  = w;
        bmpinfo.bmiHeader.biSizeImage = abs(bmpinfo.bmiHeader.biHeight) * bmpinfo.bmiHeader.biWidth * 4;

        bmpinfo.bmiHeader.biCompression = BI_RGB;
        bmpinfo.bmiHeader.biPlanes = 1;
        bmpinfo.bmiHeader.biBitCount = 32;

        bmpfileheader.bfReserved1=0;
        bmpfileheader.bfReserved2=0;
        bmpfileheader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO)+bmpinfo.bmiHeader.biSizeImage;
        bmpfileheader.bfType = MakeIntelWord('M','B');
        bmpfileheader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO);


        fwrite(&bmpfileheader,sizeof(BITMAPFILEHEADER),1,fp);
        fwrite(&bmpinfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
        fwrite(pBuf, bmpinfo.bmiHeader.biSizeImage, 1, fp);
        fclose(fp);
     }
}


HRESULT CAtmoLightFilter::Transform(IMediaSample *pSample)
{
    CheckPointer(pSample,E_POINTER);
    if(!m_bitmap_header || !m_bitmap_header)
        return NOERROR;

    BITMAPINFOHEADER *bmiHeader = NULL;
    RECT rect;

    AM_MEDIA_TYPE *pMT;
    if((pSample->GetMediaType(&pMT) == S_OK) && pMT)
    {
       log("CAtmoLightFilter::Transform: MediaType changed!\n");

       CMediaType temp(*pMT, NULL);

       LogMediaType("From",&m_pInput->CurrentMediaType());
       LogMediaType("Changed to", &temp);

       m_pInput->SetMediaType( &temp );
       
       DeleteMediaType(pMT);
    }

    
    if( IsEqualGUID( *m_pInput->CurrentMediaType().FormatType(), FORMAT_VideoInfo) ) 
    {
        VIDEOINFO* pVI = (VIDEOINFO*) m_pInput->CurrentMediaType( ).Format( );
        CheckPointer(pVI,E_UNEXPECTED);

        rect = pVI->rcSource;

        if(pVI->bmiHeader.biSize >= sizeof(BITMAPINFOHEADER)) 
           bmiHeader = &pVI->bmiHeader;
        else
           return NOERROR;  
    } else
    if( IsEqualGUID( *m_pInput->CurrentMediaType().FormatType(), FORMAT_VideoInfo2) ) 
    {
        VIDEOINFOHEADER2* pVI = (VIDEOINFOHEADER2*) m_pInput->CurrentMediaType( ).Format( );
        CheckPointer(pVI, E_UNEXPECTED);

        rect = pVI->rcSource;

        if(pVI->bmiHeader.biSize >= sizeof(BITMAPINFOHEADER))
           bmiHeader = &pVI->bmiHeader;
        else
           return NOERROR;  
    } else {
        log("Error: no Format_VideoInfo structure.");
        return NOERROR;
    }
    
    unsigned char *pBuffer = NULL;
    if( pSample->GetPointer( (LPBYTE*) &pBuffer ) != S_OK || !pBuffer )
        return NOERROR;


    m_FrameCounter++;


    OleInitialize( NULL ); 

    
    int stride = (((bmiHeader->biWidth * bmiHeader->biBitCount) + 31) & ~31) >> 3;
  
    //http://msdn.microsoft.com/en-us/library/aa904813(VS.80).aspx
    if( m_pfTransform )
    {
        IAtmoLiveViewControl *liveControl = getAtmoLiveViewControl();
        if(liveControl)
        {
            unsigned char *rgb_buffer;
            if(SafeArrayAccessData(m_pixel_buffer,(void **)&rgb_buffer) == S_OK)
            {
               (this->*m_pfTransform)( pSample, pBuffer, rect, bmiHeader, stride, rgb_buffer );

                if(m_LogLevel>=2 && (m_FrameCounter % 25) == 0) 
                {
                   char fn[MAX_PATH];
                   sprintf( fn, "%simg_%d.bmp", m_pszImageLogPath, m_FrameCounter/25+100000 );
                   SaveBitmap( fn, rgb_buffer, m_atmo_capture_width, m_atmo_capture_height);
                }
                
                SafeArrayUnaccessData( m_pixel_buffer );
                liveControl->setPixelData(m_bitmap_header, m_pixel_buffer);
            }

            liveControl->Release();
        }     
    }
    

    OleUninitialize();

    return NOERROR;
}


void CAtmoLightFilter::Transform_Packed_YUY2_YUYV( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    /* is a packed format */
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    int row_len = (bmiHeader->biWidth / 2) * 4; 

    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        unsigned char *yuy2_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / (m_atmo_capture_height+1)) * row_len ;     
        for( int col = 1; col < m_atmo_capture_width; col ++ )
        {
                int pixel = (col * bmiHeader->biWidth) / m_atmo_capture_width;
                unsigned char *yuy2_pixel = yuy2_line + (int)(pixel / 2) * 4;
                unsigned char y = ((pixel & 1)==1) ? yuy2_pixel[2] : yuy2_pixel[0];
                unsigned char u = yuy2_pixel[1];
                unsigned char v = yuy2_pixel[3];
                yuv_to_rgb(b, g, r, y, u, v);
                r+=4; g+=4; b+=4;
        }
    }

}

void CAtmoLightFilter::Transform_Packed_Y411( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    int row_len = (bmiHeader->biWidth / 4)*6;  // 4 pixels are 6 bytes - so assume that width is allways a 4x :)
    // is die länge dword alligned?
        
    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        unsigned char *y411_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / (m_atmo_capture_height+1)) * row_len ;     
        for( int col = 1; col <= m_atmo_capture_width; col ++ )
        {
                int pixel = (col * bmiHeader->biWidth) / (m_atmo_capture_width+1);
                unsigned char *y411_pixel = y411_line + (int)(pixel / 4) * 6;
                unsigned char y;
                switch( pixel & 3 ) {
                    case 0:
                            y = y411_pixel[1];
                            break;
                    case 1: 
                            y = y411_pixel[2];
                            break;
                    case 2:
                            y = y411_pixel[4];
                            break;
                    case 3:  
                            y = y411_pixel[5];
                            break;
                }
                unsigned char u = y411_pixel[0];
                unsigned char v = y411_pixel[3];
                yuv_to_rgb(b, g, r, y, u, v);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_Packed_Y41P( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    // MEDIASUBTYPE_Y41P packed format 8 Pixel, 12 byte U0 Y0 V0 Y1 U4 Y2 V4 Y3 Y4 Y5 Y6 Y7
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    int row_len = (bmiHeader->biWidth / 8) * 12;  // 8 pixels are 12 bytes - so assume that width is allways a 8px :)
    // is die länge dword alligned?
    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        unsigned char *y41p_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / (m_atmo_capture_height+1)) * row_len ;     
        for( int col = 1; col <= m_atmo_capture_width; col ++ )
        {
                int pixel = (col * bmiHeader->biWidth) / (m_atmo_capture_width+1);
                unsigned char *y41p_pixel = y41p_line + (int)(pixel / 8) * 12;
                unsigned char y;
                switch( pixel & 7 ) {
                    case 0:
                            y = y41p_pixel[1];
                            break;
                    case 1: 
                            y = y41p_pixel[3];
                            break;
                    case 2:
                            y = y41p_pixel[5];
                            break;
                    case 3:  
                            y = y41p_pixel[7];
                            break;
                    case 4:  
                            y = y41p_pixel[8];
                            break;
                    case 5:  
                            y = y41p_pixel[9];
                            break;
                    case 6:  
                            y = y41p_pixel[10];
                            break;
                    case 7:  
                            y = y41p_pixel[11];
                            break;
                }
                // pixel 0..3 -> u0 0    v0 2
                // pixel 4..7 -> u0 4    v4 6
                unsigned char u = ((pixel & 7)<4) ? y41p_pixel[0] : y41p_pixel[4];
                unsigned char v = ((pixel & 7)<4) ? y41p_pixel[2] : y41p_pixel[6];
                yuv_to_rgb(b, g, r, y, u, v);
                r+=4; g+=4; b+=4;
        }
    }

}

void CAtmoLightFilter::Transform_Packed_YVYU( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    // MEDIASUBTYPE_YVYU packed format 2 pixel , 4 byte Y0 V0 Y1 U0 
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    int row_len = (bmiHeader->biWidth / 2) * 4; 

    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        unsigned char *yvyu_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / (m_atmo_capture_height+1)) * row_len ;     
        for( int col = 1; col <= m_atmo_capture_width; col ++ )
        {
                int pixel = (col * bmiHeader->biWidth) / (m_atmo_capture_width+1);
                unsigned char *yvyu_pixel = yvyu_line + (int)(pixel / 2) * 4;
                unsigned char y = ((pixel & 1)==1) ? yvyu_pixel[2] : yvyu_pixel[0];
                unsigned char u = yvyu_pixel[3];
                unsigned char v = yvyu_pixel[1];
                yuv_to_rgb(b, g, r, y, u, v);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_Packed_UYVY( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    // MEDIASUBTYPE_UYVY packed format 2 pixel , 4 byte U0 Y0 V0 Y1 
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    int row_len = (bmiHeader->biWidth / 2) * 4; 

    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        unsigned char *uyvy_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / (m_atmo_capture_height+1)) * row_len ;     
        for( int col = 1; col <= m_atmo_capture_width; col ++ )
        {
                int pixel = (col * bmiHeader->biWidth) / (m_atmo_capture_width+1);
                unsigned char *uyvy_pixel = uyvy_line + (int)(pixel / 2) * 4;
                unsigned char y = ((pixel & 1)==1) ? uyvy_pixel[3] : uyvy_pixel[1];
                unsigned char u = uyvy_pixel[0];
                unsigned char v = uyvy_pixel[2];
                yuv_to_rgb(b, g, r, y, u, v);
                r+=4; g+=4; b+=4;
        }
    }

}


void CAtmoLightFilter::Transform_Packed_Y211( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    // MEDIASUBTYPE_Y211 packed format 2 pixel , 4 byte Y0 U0 Y2 V0    (sind eigentlich 4 pixel 0,1 Y0,  2,3 Y2)
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    // 
    // is das richtig steht in biWidth die wirkliche breite? - oder nur das was auch wirklich
    // zählt bei diesem Format? 
    // ich gehe davon aus das im biWidth die wirkliche soll breite steht - aber das Image nur die halbe breite
    // als pixel beinhaltet!
    // sollte das falsch sein sehen wir uns wieder herr watson
    //
    int row_len = bmiHeader->biWidth;
    // dword alignment? or is width a multiple of 4? or 2?

    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        unsigned char *y211_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / (m_atmo_capture_height+1)) * row_len ;     
        for( int col = 1; col <= m_atmo_capture_width; col ++ )
        {
                int pixel = (col * bmiHeader->biWidth) / (m_atmo_capture_width+1);

                unsigned char *y211_pixel = y211_line + (int)(pixel / 4);

                unsigned char y = ((pixel & 3) < 2) ? y211_pixel[0] : y211_pixel[2];
                unsigned char u = y211_pixel[1];
                unsigned char v = y211_pixel[3];
                yuv_to_rgb(b, g, r, y, u, v);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_Packed_CLJR( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    // MEDIASUBTYPE_CLJR packed format 4 Pixel, 4 byte  Y[5], Y[5], Y[5], Y[5], U[6], V[6] []Bits! im dword!
    /*
        Y3  31 30 29 28 27
        Y2  26 25 24 23 22
        Y1  21 20 19 18 17
        Y0  16 15 14 13 12
        U   11 10 9  8  7  6
        V   5   4 3  2 1 0  
    */
#define cljr_v_mask  ((unsigned int)0x0000003F)
#define cljr_u_mask  ((unsigned int)0x00000FC0)

//after rotation
#define cljr_y_mask  ((unsigned int)0x000000F8) 
#define cljr_y0_mask ((unsigned int)0x0001F000)
#define cljr_y1_mask ((unsigned int)0x003E0000)
#define cljr_y2_mask ((unsigned int)0x07C00000)
#define cljr_y3_mask ((unsigned int)0xF8000000)

    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    int row_len = bmiHeader->biWidth;
    

    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        unsigned int *cljr_line = (unsigned int *)(pBuffer + ((row * abs(bmiHeader->biHeight)) / (m_atmo_capture_height+1)) * row_len) ;     
        for( int col = 1; col <= m_atmo_capture_width; col ++ )
        {
                int pixel = (col * bmiHeader->biWidth) / (m_atmo_capture_width+1);
                // beware on 64bit os is int still 32bit? :)
                unsigned int cljr_pixel = cljr_line[ (pixel / 4) ];
                
                unsigned char y =  (unsigned char)((cljr_pixel >> (9 + (pixel & 3) * 5)) & cljr_y_mask);
                
                unsigned char u = (cljr_pixel & cljr_u_mask) >> 4;
                unsigned char v = (cljr_pixel & cljr_v_mask) << 2;
                yuv_to_rgb(b, g, r, y, u, v);
                r+=4; g+=4; b+=4;
        }
    }
}




void CAtmoLightFilter::Transform_Planar_IYUV_I420( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    /* is a planar format */
    int y_stride  = (stride * 2) / 3;
    int uv_stride = y_stride / 2;

    int y_visible_width  = (rect.right - rect.left);
    int y_visible_height = (rect.bottom - rect.top);

    int uv_visible_width  = y_visible_width / 2;
    int uv_visible_height = y_visible_height / 2;

    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    // plane order in buffer: YUV 
    unsigned char *y_plane = pBuffer;
    unsigned char *u_plane = y_plane + (abs(bmiHeader->biHeight)     * y_stride );
    unsigned char *v_plane = u_plane + (abs(bmiHeader->biHeight) / 2 * uv_stride );
        
    for( int row = 1; row < atmo_capture_height; row ++ )
    {
        unsigned char *y_line = y_plane + ((row * y_visible_height)  / atmo_capture_height)   * y_stride;
        unsigned char *u_line = u_plane + ((row * uv_visible_height) / atmo_capture_height)  * uv_stride;
        unsigned char *v_line = v_plane + ((row * uv_visible_height) / atmo_capture_height)  * uv_stride;

        for( int col = 1; col < atmo_capture_width; col ++ )
        {
                int y_pixel  = (col * y_visible_width)   / atmo_capture_width;
                int uv_pixel = (col * uv_visible_width)  / atmo_capture_width;
                
                yuv_to_rgb(b, g, r, y_line[y_pixel], u_line[uv_pixel], v_line[uv_pixel]);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_Planar_YV12( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    // plane order in buffer: YVU 
    int y_stride  = (stride * 2) / 3;
    int uv_stride = y_stride / 2;

    int y_visible_width  = (rect.right - rect.left);
    int y_visible_height = (rect.bottom - rect.top);

    int uv_visible_width  = y_visible_width / 2;
    int uv_visible_height = y_visible_height / 2;

    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    unsigned char *y_plane = pBuffer;
    unsigned char *v_plane = y_plane + (abs(bmiHeader->biHeight) * y_stride );
    unsigned char *u_plane = v_plane + (abs(bmiHeader->biHeight) / 2 * uv_stride );

    for( int row = 1; row <  atmo_capture_height; row++ )
    {
        unsigned char *y_line = y_plane + ((row * y_visible_height)  / atmo_capture_height)  * y_stride;
        unsigned char *u_line = u_plane + ((row * uv_visible_height) / atmo_capture_height)  * uv_stride;
        unsigned char *v_line = v_plane + ((row * uv_visible_height) / atmo_capture_height)  * uv_stride;

        for( int col = 1; col <  atmo_capture_width; col++ )
        {
                int y_pixel  = (col * y_visible_width)   / atmo_capture_width;
                int uv_pixel = (col * uv_visible_width)  / atmo_capture_width;
                yuv_to_rgb(b, g, r, y_line[y_pixel], u_line[uv_pixel], v_line[uv_pixel]);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_Planar_NV21( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
 /* NV21 Planar
   2 Planes
   0: Y-Plane volle Auflösung
   1: UV Interleaved Plane in Zeile immer abwechselnd UV oder umgekehrt? (2x2 subsampled)
 */

    int y_stride  = (stride * 2) / 3;
    int uv_stride = y_stride;

    int y_visible_width  = (rect.right - rect.left);
    int y_visible_height = (rect.bottom - rect.top);

    int uv_visible_width  = y_visible_width / 2;
    int uv_visible_height = y_visible_height / 2;

    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    // plane order in buffer: YVU 
    unsigned char  *y_plane = pBuffer;
    unsigned char  *uv_plane = y_plane + (abs(bmiHeader->biHeight)     * y_stride );
        
    for( int row = 1; row < atmo_capture_height; row ++ )
    {
        unsigned char *y_line  = y_plane + ((row * y_visible_height)  / atmo_capture_height)   * y_stride;
        unsigned char *uv_line = uv_plane + ((row * uv_visible_height) / atmo_capture_height)  * uv_stride;

        for( int col = 1; col < atmo_capture_width; col ++ )
        {
                int y_pixel  = (col * y_visible_width)   / atmo_capture_width;
                int uv_pixel = ((col * uv_visible_width)  / atmo_capture_width) * 2;

                yuv_to_rgb(b, g, r, y_line[y_pixel], uv_line[uv_pixel], uv_line[uv_pixel+1]);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_Planar_NV12( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
  /* NV12 Planar */
  /* 
    2 Planes
    0: Y Plane voll auflösung
    1: VU Interleaved Plane in Zeile immer abwechselnd VU oder umgekehrt? (2x2 subsampled)
  */
    int y_stride  = (stride * 2) / 3;
    int vu_stride = y_stride;

    int y_visible_width  = (rect.right - rect.left);
    int y_visible_height = (rect.bottom - rect.top);

    int uv_visible_width  = y_visible_width / 2;
    int uv_visible_height = y_visible_height / 2;

    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    // plane order in buffer: YVU 
    unsigned char  *y_plane = pBuffer;
    unsigned char  *vu_plane = y_plane + (abs(bmiHeader->biHeight)     * y_stride );
        
    for( int row = 1; row < atmo_capture_height; row ++ )
    {
        unsigned char *y_line  = y_plane + ((row * y_visible_height)  / atmo_capture_height)   * y_stride;
        unsigned char *vu_line = vu_plane + ((row * uv_visible_height) / atmo_capture_height)  * vu_stride;

        for( int col = 1; col < atmo_capture_width; col ++ )
        {
                int y_pixel  = (col * y_visible_width)   / atmo_capture_width;
                int uv_pixel = ((col * uv_visible_width)  / atmo_capture_width) * 2;

                yuv_to_rgb(b, g, r, y_line[y_pixel], vu_line[uv_pixel+1], vu_line[uv_pixel]);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_Planar_YVU9( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    int y_stride  = (stride * 2) / 3;
    int uv_stride = y_stride / 2;

    int y_visible_width  = (rect.right - rect.left);
    int y_visible_height = (rect.bottom - rect.top);

    int uv_visible_width  = y_visible_width / 2;
    int uv_visible_height = y_visible_height / 2;

    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    // plane order in buffer: YVU 
    unsigned char *y_plane = pBuffer;
    unsigned char *v_plane = y_plane + (abs(bmiHeader->biHeight)     * y_stride );
    unsigned char *u_plane = v_plane + (abs(bmiHeader->biHeight) / 2 * uv_stride );
        
    for( int row = 1; row < atmo_capture_height; row ++ )
    {
        unsigned char *y_line = y_plane + ((row * y_visible_height)  / atmo_capture_height)   * y_stride;
        unsigned char *u_line = u_plane + ((row * uv_visible_height) / atmo_capture_height)  * uv_stride;
        unsigned char *v_line = v_plane + ((row * uv_visible_height) / atmo_capture_height)  * uv_stride;

        for( int col = 1; col < atmo_capture_width; col ++ )
        {
                int y_pixel  = (col * y_visible_width)   / atmo_capture_width;
                int uv_pixel = (col * uv_visible_width)  / atmo_capture_width;
                yuv_to_rgb(b, g, r, y_line[y_pixel], u_line[uv_pixel], v_line[uv_pixel]);
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_RGB_565( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    /* is a packed? format */
    // RGB 16bit 5-6-5
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    unsigned short *rgb16_buffer = (unsigned short *)pBuffer;
        
    for( int row = 1; row < atmo_capture_height; row ++ )
    {
        unsigned short *rgb_line = &rgb16_buffer[((row * abs(bmiHeader->biHeight)) / atmo_capture_height ) * bmiHeader->biWidth ];

        for( int col = 1; col < atmo_capture_width; col ++ )
        {
                unsigned short pixel  = rgb_line[ (col * bmiHeader->biWidth) / atmo_capture_width ];
                // 0000 0000 0000 0000
                // RRRR RGGG GGGB BBBB
                // 
                *r = (pixel & 0xF800) >> 8;
                *g = (pixel & 0x07E0) >> 3;
                *b = (pixel & 0x001F) << 3;
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_RGB_555( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    /* is a packed? format */
    // RGB 15bit 5-5-5
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    unsigned short *rgb16_buffer = (unsigned short *)pBuffer;
        
    for( int row = 1; row < atmo_capture_height; row ++ )
    {
        unsigned short *rgb_line = &rgb16_buffer[((row * abs(bmiHeader->biHeight)) / atmo_capture_height) * bmiHeader->biWidth ];

        for( int col = 1; col < atmo_capture_width; col ++ )
        {
                unsigned short pixel  = rgb_line[ (col * bmiHeader->biWidth) / atmo_capture_width ];
                // 0000000000000000
                // xRRRRRGGGGGBBBBB
                //        xRRRRR000
                //         GGGGG
                //         BBBBB
                *r = (pixel & 0x7C00) >> 7;
                *g = (pixel & 0x03E0) >> 2;
                *b = (pixel & 0x001F) << 3;
                r+=4; g+=4; b+=4;
        }
    }
}


void CAtmoLightFilter::Transform_RGB_888( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    /* is a packed? format */
    // RGB 24bit 8-8-8
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    unsigned short *rgb16_buffer = (unsigned short *)pBuffer;
        
    for( int row = 1; row <= m_atmo_capture_height; row ++ )
    {
        // ist row length dword / word alignment zu beachten?
        unsigned char *rgb_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / atmo_capture_height) * (bmiHeader->biWidth * 3);

        for( int col = 1; col <= m_atmo_capture_width; col ++ )
        {
                int x  = ((col * bmiHeader->biWidth) / m_atmo_capture_width ) * 3 ;
                *r = rgb_line[x];
                *g = rgb_line[x+1];
                *b = rgb_line[x+2];
                r+=4; g+=4; b+=4;
        }
    }
}

void CAtmoLightFilter::Transform_RGB_8888( IMediaSample *pSample, unsigned char *pBuffer, RECT &rect, BITMAPINFOHEADER *bmiHeader, int stride, unsigned char *rgb_buffer )
{
    // RGB 24bit 8-8-8+Alpha?
    unsigned char *r = rgb_buffer;
    unsigned char *g = rgb_buffer + 1;
    unsigned char *b = rgb_buffer + 2;

    unsigned short *rgb16_buffer = (unsigned short *)pBuffer;
        
    for( int row = 1; row < atmo_capture_height; row ++ )
    {
        // ist row length dword / word alignment zu beachten?
        unsigned char *rgb_line = pBuffer + ((row * abs(bmiHeader->biHeight)) / atmo_capture_height ) * (bmiHeader->biWidth * 4);

        for( int col = 1; col < atmo_capture_width; col ++ )
        {
                int x  = ((col * bmiHeader->biWidth) / atmo_capture_width) * 4 ;
                *r = rgb_line[x];
                *g = rgb_line[x+1];
                *b = rgb_line[x+2];
                r+=4; g+=4; b+=4;
        }
    }
}

