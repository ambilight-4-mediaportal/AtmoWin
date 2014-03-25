#ifndef __ATMOXMLCONFIG_H__
#define __ATMOXMLCONFIG_H__

#include "AtmoConfig.h"

class CAtmoXMLConfig :  public CAtmoConfig
{
private:
	int CheckByteValue(int value);
	int Check8BitValue(int value);

private:
   char *psz_language; 
private:
	char configSection[80]; 
	char newconfigSection[100];

public:
	CAtmoXMLConfig(char *section);
	virtual ~CAtmoXMLConfig(void);

	void SaveSettings(std::string Profile1);
	void LoadSettings(std::string profile);
	void ReadXMLStringList(char *section, char *default_value);

	char *getLanguage() { return(psz_language); }

	int trilinear(int x, int y, int z, int col);

};

#endif // __ATMOXMLCONFIG_H__