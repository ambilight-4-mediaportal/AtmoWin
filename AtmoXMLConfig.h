#ifndef _AtmoXMLConfig_h_
#define _AtmoXMLConfig_h_

#include "AtmoConfig.h"
#include <iostream>

class CAtmoXMLConfig :  public CAtmoConfig
{
private:
	int CheckByteValue(int value);
	int Check8BitValue(int value);

	// zwei Möglichkeiten alle Variablen Private und nur über Methoden darauf zugreifen
	// wäre zwar sauberer find ich - aber weniger performant wenn man bedenkt wie häufig
	// darauf zugegriffen wird - daher lieber alle Variablen Public lassen...?
	char configSection[100]; /* "AtmoWinX" */
	char newconfigSection[100]; /* "AtmoWinX" */

public:
	CAtmoXMLConfig(char *section);
	virtual ~CAtmoXMLConfig(void);

	void SaveSettings(char *section, std::string profile);
	void LoadSettings(char *section, std::string profile);
	void ReadXMLStringList(std::string, char *default_value);
	void WriteXMLStringList(char *section, char *default_value);

	int trilinear(int x, int y, int z, int col);

};

#endif

