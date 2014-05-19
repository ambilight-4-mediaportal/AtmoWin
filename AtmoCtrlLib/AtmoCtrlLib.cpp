// AtmoCtrlLib.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
//

#include "stdafx.h"
#include <stdio.h>

#include <comdef.h>
#include "..\AtmoWin_h.h"

#if defined(_MSC_VER)
#define DLL extern "C" __declspec(dllexport)
#else
#define DLL
#endif

//IUnknown             *pIUnknown     = NULL;
//IAtmoRemoteControl   *pAtmoRemoteControl = NULL;
//IAtmoLiveViewControl *pAtmoLiveViewControl = NULL;
SAFEARRAY            *pixelDataArr = NULL; // = SafeArrayCreateVector(VT_UI1,0,size);
SAFEARRAY            *bitmapInfoArr = NULL; // = SafeArrayCreateVector(VT_UI1,0,sizeof(BITMAPINFOHEADER));



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

IAtmoRemoteControl *getAtmoRemoteControl() {
    IUnknown             *pIUnknown = NULL;
    IAtmoRemoteControl   *pAtmoRemoteControl = NULL;
    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown != NULL) {
       pIUnknown->QueryInterface(IID_IAtmoRemoteControl,(void **)&pAtmoRemoteControl);  
    }
    return pAtmoRemoteControl;
}

IAtmoRemoteControl2 *getAtmoRemoteControl2() {
    IUnknown             *pIUnknown = NULL;
    IAtmoRemoteControl2   *pAtmoRemoteControl2 = NULL;
    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown != NULL) {
       pIUnknown->QueryInterface(IID_IAtmoRemoteControl2,(void **)&pAtmoRemoteControl2);  
    }
    return pAtmoRemoteControl2;
}


IAtmoLiveViewControl *getAtmoLiveViewControl() {
    IUnknown              *pIUnknown = NULL;
    IAtmoLiveViewControl  *pAtmoLiveViewControl = NULL;
    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown != NULL) {
       pIUnknown->QueryInterface(IID_IAtmoLiveViewControl,(void **)&pAtmoLiveViewControl);  
    }
    return pAtmoLiveViewControl;
}



DLL int AtmoInitialize() {
    OleInitialize(NULL);
    if(getAtmoRemoteControl() == NULL) {
       OleUninitialize();
       return -1;
    } else {
       return 1;
    }
}

DLL void AtmoFinalize(int what) {
    if(what > 0) {
       if(pixelDataArr != NULL)
       SafeArrayDestroy(pixelDataArr); 
       pixelDataArr = NULL;
       if(bitmapInfoArr != NULL)
       SafeArrayDestroy(bitmapInfoArr); 
       bitmapInfoArr = NULL;
    }
    OleUninitialize();
}

DLL int AtmoSwitchEffect(int newMode) {
    ComEffectMode oldMode;
    IAtmoRemoteControl *pARC = getAtmoRemoteControl();
    if(pARC != NULL) {
       pARC->setEffect((ComEffectMode)newMode, &oldMode);
       pARC->Release();
       return (int)oldMode;
    } else {
        return -1;
    }
}

DLL int AtmoSetLiveSource(int newSource) {
    IAtmoLiveViewControl *pALVC = getAtmoLiveViewControl(); 
    if(pALVC!=NULL) {
       pALVC->setLiveViewSource((ComLiveViewSource)newSource);
       pALVC->Release();
       return 1;
    }
    return -1;
}

DLL void AtmoCreateTransferBuffers(int FourCC,int bytePerPixel,int width,int height) {
    if(pixelDataArr != NULL)
       SafeArrayDestroy(pixelDataArr); 
    if(bitmapInfoArr != NULL)
       SafeArrayDestroy(bitmapInfoArr); 

    int size = width * height * bytePerPixel;
    pixelDataArr  = SafeArrayCreateVector(VT_UI1,0,size);

    bitmapInfoArr = SafeArrayCreateVector(VT_UI1,0,sizeof(BITMAPINFOHEADER));

    BITMAPINFOHEADER *header;
    SafeArrayAccessData(bitmapInfoArr,(void **)&header);
    header->biSize = sizeof(BITMAPINFOHEADER);
    header->biWidth = width;
    header->biHeight = height;
    header->biBitCount = bytePerPixel*8;
    header->biCompression = FourCC;
    SafeArrayUnaccessData(bitmapInfoArr);
}

DLL void *AtmoLockTransferBuffer() {
    unsigned char *pixelData = NULL;
    if(pixelDataArr!=NULL)
       SafeArrayAccessData(pixelDataArr,(void **)&pixelData);
    return pixelData;
}

DLL void AtmoSendPixelData() {
    IAtmoLiveViewControl *pALVC = getAtmoLiveViewControl(); 
    if((pixelDataArr != NULL) && (pALVC!=NULL))  {
       SafeArrayUnaccessData(pixelDataArr);
       pALVC->setPixelData(bitmapInfoArr,pixelDataArr);
    }
    if(pALVC)
       pALVC->Release();
}

DLL void AtmoWinGetImageSize(int *width,int *height)
{ 
  IAtmoRemoteControl2 *control = getAtmoRemoteControl2();
  if(width && height) 
  {
    if(control != NULL) 
       control->getLiveViewRes(width, height);
    else {
       *width = 64;
       *height = 48;
    }
  }
  if(control) 
     control->Release();
}
