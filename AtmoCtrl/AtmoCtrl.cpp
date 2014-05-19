/*

  AtmoCtrl.exe Befehlszeilentool um AtmoWin zu steuren ... ein ausschalten Farben wechseln...

*/

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <comdef.h>		
#include <time.h>
#include "..\AtmoWin_h.h"


void showhelp(){
     printf("AtmoCtrl.exe - parameter description\n");
     printf("\n\n");
     printf("-effect none|live|static|lrchanger|changer] \n");
     printf("-color RedValue GreenValue BlueValue \n");
     printf("-chcolor channel RedValue GreenValue BlueValue \n");
     printf("-chvalues channel value channel value... (until next parm or non integer on line) \n");
     printf("-sleep delayms {in Millisekunden!} \n");
     printf("\n\n");
     printf("the paramters could be repeated in the commandline in any order... have fun..\n\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc == 1) {
       showhelp();
       return 1;
    }

    OleInitialize(NULL);
	IUnknown *pIUnknown     = NULL;
	IAtmoRemoteControl *pAtmoRemoteControl = NULL;
    ComEffectMode oldEffect = cemUndefined;
    ComEffectMode newEffect;
    int red,green,blue,delayTime;

    GetActiveObject(CLSID_AtmoRemoteControl,NULL,&pIUnknown);
    if(pIUnknown!=NULL) {
       pIUnknown->QueryInterface(IID_IAtmoRemoteControl,(void **)&pAtmoRemoteControl);  
    }
    if(pAtmoRemoteControl == NULL) {
       printf("\n\nplease start first the AtmoWin software...\n\n");
       return 3;
    }


/*
	cemDisabled	= 0,
	cemStaticColor	= 1,
	cemLivePicture	= 2,
	cemColorChange	= 3,
	cemLrColorChange	= 4
*/

    int i=1;
    while(i<argc) {
        if((stricmp(argv[i],"-effect") == 0) || (stricmp(argv[i],"/effect") == 0)) {
           i++;
           if(i>=argc) {
              showhelp();
              return 1;
           }
           if(stricmp(argv[i],"none")==0) {
               newEffect = cemDisabled;
               printf("SetEffect cemDisabled\n");
           } else if(stricmp(argv[i],"live")==0) {
               newEffect = cemLivePicture;
               printf("SetEffect cemLivePicture\n");
           } else if(stricmp(argv[i],"static")==0) {
               newEffect = cemStaticColor;
               printf("SetEffect cemStaticColor\n");
           } else if(stricmp(argv[i],"lrchanger")==0) {
               newEffect = cemLrColorChange;
               printf("SetEffect cemLrColorChange\n");
           } else if(stricmp(argv[i],"changer")==0) {
               newEffect = cemColorChange;
               printf("SetEffect cemColorChange\n");
           } else {
               showhelp();
               return 2;
           }
           pAtmoRemoteControl->setEffect(newEffect,&oldEffect);
           i++;
           continue;
        }
        if((stricmp(argv[i],"-color") == 0) || (stricmp(argv[i],"/color") == 0)) {
           i++;
           if(i>=argc) {
              showhelp();
              return 1;
           }
           red = atoi(argv[i++]);
           if(i>=argc) {
              showhelp();
              return 1;
           }
           green = atoi(argv[i++]);
           if(i>=argc) {
              showhelp();
              return 1;
           }
           blue = atoi(argv[i++]);
           printf("SetColor %d, %d, %d \n",red & 255,green & 255,blue & 255);
           pAtmoRemoteControl->setStaticColor((BYTE)(red & 255),(BYTE)(green & 255),(BYTE)(blue & 255));
          continue;
        }

        if((stricmp(argv[i],"-chcolor") == 0) || (stricmp(argv[i],"/chcolor") == 0)) {
           i++;
           if(i>=argc) {
              showhelp();
              return 1;
           }

           int channel = atoi(argv[i++]);
           if(i>=argc) {
              showhelp();
              return 1;
           }

           red = atoi(argv[i++]);
           if(i>=argc) {
              showhelp();
              return 1;
           }
           green = atoi(argv[i++]);
           if(i>=argc) {
              showhelp();
              return 1;
           }
           blue = atoi(argv[i++]);

           printf("SetChColor %d -> %d, %d, %d \n",channel, red & 255,green & 255,blue & 255);
           pAtmoRemoteControl->setChannelStaticColor(channel,(BYTE)(red & 255),(BYTE)(green & 255),(BYTE)(blue & 255));
           continue;
        }

        if((stricmp(argv[i],"-chvalues") == 0) || (stricmp(argv[i],"/chvalues") == 0)) {
           i++;
           int old_i = i; 
           int values = 0;
           while(i < (argc-1)) {
                 if((argv[i]==NULL) || (argv[i][0] < 48) || (argv[i][0]>57)) {
                    break; 
                 }
                 if((argv[i+1]==NULL) || (argv[i+1][0] < 48) || (argv[i+1][0]>57)) {
                    break; 
                 }
                 values++;
                 i+=2;
           }
           printf("Found %d values\n",values);

           unsigned char *channel_values;
           SAFEARRAY *channelDataArr = SafeArrayCreateVector(VT_UI1, 0, values * 2);
           SafeArrayAccessData(channelDataArr,(void **)&channel_values);
           i = old_i;
           values = 0;
           while(i < (argc-1)) {
                 if((argv[i]==NULL) || (argv[i][0] < 48) || (argv[i][0]>57)) {
                    break; 
                 }
                 if((argv[i+1]==NULL) || (argv[i+1][0] < 48) || (argv[i+1][0]>57)) {
                    break; 
                 }
                 channel_values[values++] = (atoi(argv[i++]) & 255);   // DMX Kanal
                 channel_values[values++] = (atoi(argv[i++]) & 255); // Wert
           }
           SafeArrayUnaccessData(channelDataArr);

           printf("SetChannelValues Bytes %d\n",values);

           pAtmoRemoteControl->setChannelValues( channelDataArr );

           continue;
        }

        if((stricmp(argv[i],"-sleep") == 0) || (stricmp(argv[i],"/sleep") == 0)) {
           i++;
           if(i>=argc) {
              showhelp();
              return 1;
           }
           delayTime = atoi(argv[i++]);
           printf("delay %d ms\n",delayTime);
           Sleep(delayTime);
           
           continue;
        }
    }
   

    OleUninitialize();

	return 0;
}

