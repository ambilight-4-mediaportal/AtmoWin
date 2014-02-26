#include <windows.h>
#include "AtmoRes.h"


HINSTANCE hAtmo_ResInstance;
HINSTANCE hAtmo_Instance;


char *getResStr(UINT uID,char *psz_default)
{
   char *buffer = new char[1026];
   char *temp;
   if(hAtmo_ResInstance) {
      if(LoadString(hAtmo_ResInstance,uID,buffer,sizeof(char)*1024)) {
         temp = strdup(buffer);
         delete []buffer;
         return(temp);
       }
   }
   if(LoadString(hAtmo_Instance,uID,buffer,sizeof(char)*1024)) {
      temp = strdup(buffer);
      delete []buffer;
      return(temp);
   }
   delete []buffer;
   return strdup(psz_default);
}

