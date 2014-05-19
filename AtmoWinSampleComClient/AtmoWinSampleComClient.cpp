
#include "stdafx.h"
#include <comdef.h>		
#include <time.h>


#include "..\AtmoWin_h.h"



int _tmain(int argc, _TCHAR* argv[])
{
	IUnknown        *pIUnknown     = NULL;
	IAtmoRemoteControl *pAtmoRemoteControl = NULL;
    IAtmoLiveViewControl *pAtmoLiveViewControl = NULL;
    ComEffectMode oldEffect = cemUndefined;

    int i;
    DWORD t1,tsum;

    OleInitialize(NULL);

    BITMAPINFOHEADER *header;
    unsigned char *pixelData;


    /* dieser Code Block sollte in der Initialisierung des DirectShow Filters ausgeführt werden
       ich gehen mal in meinem Halbwissen davon aus -- das die Filter selbst bei der Wiedergabe
       von mehreren Filmen in einer Playlist - für jeden Film neu erstellt werden? 
    */
    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown!=NULL) {
       pIUnknown->QueryInterface(IID_IAtmoRemoteControl,(void **)&pAtmoRemoteControl);  
       if(pAtmoRemoteControl!=NULL) {
           pAtmoRemoteControl->setEffect(cemLivePicture,&oldEffect);
       }
       pIUnknown->QueryInterface(IID_IAtmoLiveViewControl,(void **)&pAtmoLiveViewControl);  
       if(pAtmoLiveViewControl!=NULL) {
           pAtmoLiveViewControl->setLiveViewSource(lvsExternal);
       }
    }


    srand( (unsigned)time( NULL ) );

    tsum = 0;


    // 60 Sek @ 25 FPS simulieren!
    for(i=0;i<150000;i++) {
        if((i % 50) == 0) printf(".");
        t1 = GetTickCount(); 

       GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
       if(pIUnknown!=NULL) {
           pIUnknown->QueryInterface(IID_IAtmoLiveViewControl,(void **)&pAtmoLiveViewControl);  

           if(pAtmoLiveViewControl != NULL) {
                
                SAFEARRAY *pixelDataArr = SafeArrayCreateVector(VT_UI1,0,64*48*3);
                SAFEARRAY *bitmapInfoArr = SafeArrayCreateVector(VT_UI1,0,sizeof(BITMAPINFOHEADER));

                SafeArrayAccessData(bitmapInfoArr,(void **)&header);
                SafeArrayAccessData(pixelDataArr,(void **)&pixelData);
                
                header->biSize = sizeof(BITMAPINFOHEADER);
                header->biWidth = 64;
                header->biHeight = 48;
                header->biBitCount = 24;
                header->biCompression = BI_RGB;

                unsigned char r = (rand() & 31) * 8;
                unsigned char g = (rand() & 31) * 8;
                unsigned char b = (rand() & 31) * 8;

                for(int y=0;y<48;y++) {
                   for(int x=0;x<64;x++) {
                       pixelData[y*64*3 + (x*3)]   = r; 
                       pixelData[y*64*3 + (x*3)+1] = g; 
                       pixelData[y*64*3 + (x*3)+2] = b; 
                   }
                }

                SafeArrayUnaccessData(bitmapInfoArr);
                SafeArrayUnaccessData(pixelDataArr);

                pAtmoLiveViewControl->setPixelData(bitmapInfoArr,pixelDataArr);

                SafeArrayDestroy(bitmapInfoArr);
                SafeArrayDestroy(pixelDataArr);
            }
        }
        t1 = GetTickCount() - t1; 
        tsum = tsum + t1;

        if(t1 < 5) 
           Sleep(5-t1);
    }

    printf(" \n\n");

    /* dieser Code Block sollte dann ausgeführt werden - wenn der Filter entladen / zerstört wird...
       damit im Atmo Win der GDI Capture Treiber wieder die Kontrolle übernimmt!
    */
    if((oldEffect != cemLivePicture) && (oldEffect!=cemUndefined)) {
        // Effek nur ändern woll - wenn es vorher ein andere Effekt war 
        // und auch nur wenn der alte effekt wirklich ermittelt wurde!
       GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
       if(pIUnknown!=NULL) {
            pIUnknown->QueryInterface(IID_IAtmoRemoteControl,(void **)&pAtmoRemoteControl);  
            if(pAtmoRemoteControl!=NULL) {
                pAtmoRemoteControl->setEffect(oldEffect,NULL); 
             }
       }
    }

    if(oldEffect == cemLivePicture) {
        // wenn alter Effekt LiveView war - dann jetzt auf GDI Modus zurückschalten...
        // andernfalls - spielt das eh keine Roll falls durch obigen Code ein andere Effekt
        // wie z.B. Static Color aktiviert wurde...
       GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
       if(pIUnknown!=NULL) {
          pIUnknown->QueryInterface(IID_IAtmoLiveViewControl,(void **)&pAtmoLiveViewControl);  
          if(pAtmoLiveViewControl!=NULL) {
             pAtmoLiveViewControl->setLiveViewSource(lvsGDI);
          }
       }
    }

    t1 = tsum / 1500;
    printf("T Average: %d ms Tsum: %d ms", t1, tsum);

    OleUninitialize();
	return 0;
}

