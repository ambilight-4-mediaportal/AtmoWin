#ifndef _AtmoXMLConfig_h_
#define _AtmoXMLConfig_h_

#include "AtmoConfig.h"
#include <iostream>

class CAtmoXMLConfig :  public CAtmoConfig
{
private:
	int CheckByteValue(int value);
	int Check8BitValue(int value);

private:
	//char *psz_language; 

private:
	// zwei Möglichkeiten alle Variablen Private und nur über Methoden darauf zugreifen
	// wäre zwar sauberer find ich - aber weniger performant wenn man bedenkt wie häufig
	// darauf zugegriffen wird - daher lieber alle Variablen Public lassen...?
	char configSection[80]; /* "AtmoWinX" */
	char newconfigSection[100]; /* "AtmoWinX" */

public:
	CAtmoXMLConfig(char *section);
	virtual ~CAtmoXMLConfig(void);

	void SaveSettings(char *section, std::string profile);
	void LoadSettings(char *section, std::string profile);

	int trilinear(int x, int y, int z, int col);


};

#endif

