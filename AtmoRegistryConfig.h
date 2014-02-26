#ifndef _AtmoRegistryConfig_h_
#define _AtmoRegistryConfig_h_

#include "atmoconfig.h"
#include <iostream>


class CAtmoRegistryConfig :  public CAtmoConfig
{
private:
       void WriteRegistryInt(HKEY mykey, char *path, char *valueName, int value);
       void WriteRegistryString(HKEY mykey, char *path, char *valueName, char *value);
       void WriteRegistryDouble(HKEY mykey, char *path, char *valueName, double value);
   
	   

       int ReadRegistryInt(HKEY mykey, char *path, char *valueName, int default_value);
       char * ReadRegistryString(HKEY mykey, char *path, char *valueName, char *default_value);
       double ReadRegistryDouble(HKEY mykey, char *path, char *valueName, double default_value);

	   int RegistryKeyExists(HKEY mykey, char *path);
	   int RegistryValueExists(HKEY mykey, char *path, char *valueName);

	   int CheckByteValue(int value);
	   int Check8BitValue(int value);

	   
 
    private:
       char *psz_language; 

    private:
       // zwei Möglichkeiten alle Variablen Private und nur über Methoden darauf zugreifen
       // wäre zwar sauberer find ich - aber weniger performant wenn man bedenkt wie häufig
       // darauf zugegriffen wird - daher lieber alle Variablen Public lassen...?
       char configRootKey[80]; /* \HKLM\Software\AtmoWin\ */
	   char newconfigRootKey[100]; /* \HKLM\Software\AtmoWin\ */

public:
     CAtmoRegistryConfig(char *regKey);
     virtual ~CAtmoRegistryConfig(void);

	 void SaveSettings(HKEY mykey, std::string profile);
     void LoadSettings(HKEY mykey, std::string profile);
	 void fastLoadSettings(HKEY mykey, std::string profile);
	 void ReadRegistryStringList(HKEY mykey, char *path, char *valueName, char *default_value);
	 void WriteRegistryStringList(HKEY mykey, char *path, char *valueName, char *default_value);

     char *getLanguage() { return(psz_language); }

	 int trilinear(int x, int y, int z, int col);
	 //static BOOL CAtmoRegistryConfig::RegDelnodeRecurse (LPTSTR lpSubKey);
	 
	 
};

#endif

