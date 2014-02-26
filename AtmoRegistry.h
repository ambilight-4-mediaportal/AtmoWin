#pragma once

class CAtmoRegistry
{
private:
    HKEY m_RootKey;
    char m_pathBuffer[256];
public:
    CAtmoRegistry(HKEY rootKey);
    ~CAtmoRegistry(void);

    void WriteRegistryInt(char *path, char *valueName, int value);
    void WriteRegistryString(char *path, char *valueName, char *value);
    void WriteRegistryDouble(char *path, char *valueName, double value);

    int ReadRegistryInt(char *path, char *valueName, int default_value);
    char * ReadRegistryString(char *path, char *valueName, char *default_value);
    double ReadRegistryDouble(char *path, char *valueName, double default_value);

    int RegistryKeyExists(char *path);
    int RegistryValueExists(char *path, char *valueName);

    long DeleteValue(char *path, char *valueName);
    long DeleteKey(char *path, char *keyName) ;

};
