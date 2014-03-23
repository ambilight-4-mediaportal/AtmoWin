#pragma once

#include "AtmoConfig.h"

class CAtmoXMLConfig :  public CAtmoConfig
{
private:
	int CheckByteValue(int value);
	int Check8BitValue(int value);

	char configSection[100]; 
	char newconfigSection[100];

public:
	CAtmoXMLConfig(char *section);
	virtual ~CAtmoXMLConfig(void);

	void SaveSettings(std::string profile);
	void LoadSettings(std::string profile);
	void ReadXMLStringList(char *section, std::string, char *default_value);
	void WriteXMLStringList(char *section, char *default_value);

	int trilinear(int x, int y, int z, int col);

};