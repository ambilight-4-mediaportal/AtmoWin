#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include "atlstr.h"
#include "shlobj.h" 

using namespace std;

#define MAX_MENU_STRINGS 11
extern char *sTextMenu[MAX_MENU_STRINGS];

#define MAX_CHANNELASSIGNMENT_STRINGS 15
extern char *sTextChannelAssignment[MAX_CHANNELASSIGNMENT_STRINGS];

#define MAX_SETTINGSDIALOG_STRINGS 55
extern char *sTextSettingDialog[MAX_SETTINGSDIALOG_STRINGS];

#define MAX_COLORPICKER_STRINGS 5
extern char *sTextColorPicker[MAX_COLORPICKER_STRINGS];

#define MAX_GRADIENTSDIALOG_STRINGS 18
extern char *sTextAtmoGradients[MAX_GRADIENTSDIALOG_STRINGS];

#define MAX_WHITESETUP_STRINGS 26
extern char *sTextWhiteSetup[MAX_WHITESETUP_STRINGS];

#define MAX_WHITESETUPHW_STRINGS 11
extern char *sTextWhiteHWSetup[MAX_WHITESETUPHW_STRINGS];

#define MAX_ATMODLG_STRINGS 5
extern char *sTextAtmoDlg[MAX_ATMODLG_STRINGS];

#define MAX_DMXDLG_STRINGS 7
extern char *sTextDmxDlg[MAX_DMXDLG_STRINGS];

#define MAX_MULTIATMOLIGHTDLG_STRINGS 8
extern char *sTextMultiAtmoLightDlg[MAX_MULTIATMOLIGHTDLG_STRINGS];

#define MAX_MOMODLG_STRINGS 7
extern char *sTextMomoDlg[MAX_MOMODLG_STRINGS];

#define MAX_NORDLIGHTSETUP_STRINGS 7
extern char *sTextNordLightSetup[MAX_NORDLIGHTSETUP_STRINGS];

#define MAX_SECTION_STRINGS 12
extern char *sSection[MAX_SECTION_STRINGS];

class CLanguage
{
private:
	bool m_XmlOnInit;

public:

  static CString sMenuText[MAX_MENU_STRINGS];
	static CString sSettingText[MAX_SETTINGSDIALOG_STRINGS];
	static CString sChannelAssigmentText[MAX_CHANNELASSIGNMENT_STRINGS];
	static CString sTextCPicker[MAX_COLORPICKER_STRINGS];
	static CString sGradientsText[MAX_GRADIENTSDIALOG_STRINGS];
	static CString sWhiteSetupText[MAX_WHITESETUP_STRINGS];
	static CString sWhiteSetupHWText[MAX_WHITESETUPHW_STRINGS];
	static CString sAtmoDlgText[MAX_ATMODLG_STRINGS];
	static CString sDmxDlgText[MAX_DMXDLG_STRINGS];
	static CString sMultiAtmoLightDlgText[MAX_MULTIATMOLIGHTDLG_STRINGS];
  static CString sMomoDlgText[MAX_MOMODLG_STRINGS];
	static CString sNordLightSetupText[MAX_NORDLIGHTSETUP_STRINGS];

	static char szCurrentDir[MAX_PATH];
  static char szFileINI[MAX_PATH];
  static char szLang[MAX_PATH], szTemp[512], Buffer[1024], szParam[32];	

	char GetSpecialFolder(int CLSID);
	bool DirectoryExists(const char* dirName);
	void XMLParse(const char* FileName, CString* sInText, const char* Section);
	void CreateDefaultXML(const char* FileName, char* Section[]);

	CLanguage(void);
	virtual ~CLanguage(void);


};

#endif // __LANGUAGE_H__