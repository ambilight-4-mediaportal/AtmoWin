#include "StdAfx.h"
#include "atmoregistry.h"

CAtmoRegistry::CAtmoRegistry(HKEY rootKey)
{
    this->m_RootKey = rootKey;
}

CAtmoRegistry::~CAtmoRegistry(void)
{
}

int CAtmoRegistry::RegistryKeyExists(char *path) {
	HKEY keyHandle;
	if( RegOpenKeyEx(m_RootKey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
	    RegCloseKey(keyHandle);	
		return 1;
	}
    return 0;
}

int CAtmoRegistry::RegistryValueExists(char *path, char *valueName) {
	HKEY keyHandle;
	int temp = 0;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(m_RootKey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   temp = 1;
		}
	    RegCloseKey(keyHandle);	
	}
    return temp;
}

long CAtmoRegistry::DeleteValue(char *path, char *valueName)
{
	HKEY keyHandle;
    long result;
	if( (result = RegOpenKeyEx(m_RootKey, path, 0, KEY_WRITE, &keyHandle)) == ERROR_SUCCESS) {
        result = RegDeleteValue(keyHandle, valueName);
	    RegCloseKey(keyHandle);	
	}
    //ERROR_SUCCESS = ok everything else you have a problem*g*,
    return result;
}

long CAtmoRegistry::DeleteKey(char *path, char *keyName)
{
	HKEY keyHandle;
    long result;
	if( (result = RegOpenKeyEx(m_RootKey, path, 0, KEY_WRITE, &keyHandle)) == ERROR_SUCCESS) {
         // be warned the key "keyName" will not be deleted if there are subkeys below him, values
        // I think are ok and will be recusively deleted, but not keys...
        // for this case we have to do a little bit more work!
        result = RegDeleteKey(keyHandle, keyName);
	    RegCloseKey(keyHandle);	
	}
    //ERROR_SUCCESS = ok everything else you have a problem*g*,
    return result;
}

void CAtmoRegistry::WriteRegistryInt(char *path, char *valueName, int value) {
    HKEY keyHandle;

	if( RegCreateKeyEx(m_RootKey, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &keyHandle, NULL)  == ERROR_SUCCESS) {
		RegSetValueEx(keyHandle,valueName,0,REG_DWORD,(LPBYTE)&value,sizeof(int));
	    RegCloseKey(keyHandle);	
	}

}

void CAtmoRegistry::WriteRegistryString(char *path, char *valueName, char *value) {
    HKEY keyHandle;

	if( RegCreateKeyEx(m_RootKey, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &keyHandle, NULL)  == ERROR_SUCCESS) {
		RegSetValueEx(keyHandle,valueName,0,REG_SZ,(LPBYTE)value,(DWORD)(strlen(value)+1));
	    RegCloseKey(keyHandle);	
	}
}

void CAtmoRegistry::WriteRegistryDouble(char *path, char *valueName, double value) {
    HKEY keyHandle;
	if( RegCreateKeyEx(m_RootKey, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &keyHandle, NULL)  == ERROR_SUCCESS) {
		RegSetValueEx(keyHandle,valueName,0,REG_BINARY,(LPBYTE)&value,sizeof(double));
	    RegCloseKey(keyHandle);	
	}
}

int CAtmoRegistry::ReadRegistryInt(char *path, char *valueName, int default_value) {
	HKEY keyHandle;
	int tempValue;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(m_RootKey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   if(valueType == REG_DWORD) {
			   if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, (LPBYTE)&tempValue, &size1) == ERROR_SUCCESS) {
                  default_value = tempValue;
			   };
		   }
		}
	    RegCloseKey(keyHandle);	
	}
    return default_value;
}

char * CAtmoRegistry::ReadRegistryString(char *path, char *valueName, char *default_value) {
	HKEY keyHandle;
	char *tempValue = NULL;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(m_RootKey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   if((valueType == REG_SZ) && (size1>1)) {
			   // free
			   tempValue = (char *)malloc(size1+1); // +1 für NullByte`?
			   if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, (LPBYTE)tempValue, &size1) == ERROR_SUCCESS) {
                  default_value = tempValue;
			   };
		   }
		}
	    RegCloseKey(keyHandle);	
	}
	if(tempValue == NULL)  {
		// wenn tempValue nicht aus registry gelesen wurde dafür sorgen das ein neuer String mit der Kopie von DefaultValue
		// geliefert wird - das macht das Handling des Rückgabewertes der Funktion einfacher - immer schön mit free freigeben!
		default_value = strdup(default_value);
	}

    return default_value;
}

double CAtmoRegistry::ReadRegistryDouble(char *path, char *valueName, double default_value) {
	HKEY keyHandle;
	double tempValue;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(m_RootKey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   if((valueType == REG_BINARY) && (size1 == sizeof(double))) {
			   if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, (LPBYTE)&tempValue, &size1) == ERROR_SUCCESS) {
                  default_value = tempValue;
			   };
		   }
		}
	    RegCloseKey(keyHandle);	
	}
    return default_value;
}
