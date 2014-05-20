#include "stdafx.h"
#include "Language.h"
#include "shlobj.h" 
#include "tinyxml.h"
#include <string>
#include <sstream>
#include <fstream>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

char *sSection[MAX_SECTION_STRINGS] =
{"Menu",               //0
"ChannelAssignment",   //1
"SettingsDialog",      //2
"Gradients",           //3
"ColorPicker",         //4 
"WhiteSetup",          //5
"WhiteSetupHW",        //6
"Atmoduinosetup",      //7
"Dmxsetup",            //8
"MultiAtmoLightsetup", //9
"Momosetup",           //10
"FNordLightsetup",     //11
"Messages",            //12
};

char *sMessages[MAX_MESSAGE_STRINGS] =
{"This device doesn't have a special config dialog",  //0
"Info",                                               //1
"connected.",                                         //2
"Error",                                              //3
"Selected display no longer in the list",             //4 
"Delay should be more than 10ms for ColorChanger",    //5
"Steps should be more than zero for ColorChanger",    //6
"Delay should be more than 10ms for LR ColorChanger", //7
"Steps should be more than zero for LR ColorChanger"  //8
"Sorry maximal 10 Settings möglich.",                 //9
"Name already exist",                                 //10
"valid range is 1..256",                              //11
"Would you really want to delete the profile?",       //12
"All configured channel will be lost",                //13   
"timeGetDevCaps failed. Terminate.",                  //14
"CoInitialize failed. Terminate.",                    //15
"Common Controls Initialization failed.",             //16
"Programm will be terminated.",                       //17
"Check the display settings.",                        //18
"Open Device LastError",                              //19
"Failed to open device",                              //20
"RegisterActiveObject failed.",                       //21
"LoadTypeLib failed. Code:",                          //22
"RegisterTypeLib failed. Code:",                      //23
"COM Server registered Ok!",                          //24
"Failed to load, Check Format, Check Size.",          //25
"Failed to open serial port",                         //26
"with errorcode:",                                    //27 
"Display Settings changed. check settings.",          //28
};

char *sTextChannelAssignment[MAX_CHANNELASSIGNMENT_STRINGS] = 
{"Off",                   //0
"Top",                    //1
"Right",                  //2
"Bottom",                 //3
"Left",                   //4
"Combined",               //5
"delete mapping",         //6
"edit mapping",           //7
"add Mapping",            //8
"cancel",                 //9
"Mappings",               //10
"Hardware Channel",       //11
"Zone number",            //12
"Name of mapping",        //13
"Channel assignment",     //14
};

char *sTextSettingDialog[MAX_SETTINGSDIALOG_STRINGS] = 
{"no effect",                    //0
"static color",                  //1
"life",                          //2
"color changer",                 //3
"color changer LR",              //4
"Connected successfully",        //5
"Configure device",              //6
"center zones",                  //7
"Change channel assignments",    //8
"Configure software adjustment", //9
"Configure hardware adjustment", //10
"Select",                        //11
"Select",                        //12
"Use...",                        //13
"Widescreenmodus",               //14
"Cancel",                        //15
"Effect active:",                //16
"Static color",                  //17
"Color change",                  //18
"Color change Left/Right",       //19
"Shutdown color",                //20
"Steps:",                        //21
"Steps:",                        //22
"Device type:",                  //23
"Zones top:",                    //24
"Zones bottom:",                 //25
"Zones Left/Right:",             //26
"Live image parameters",         //27
"No Filter",                     //28
"Combined",                      //29
"Percent",                       //30
"Edge weighting",                //31
"Brightness",                    //32
"Darkness Limit",                //33
"Hue windowing",                 //34
"Sat windowing",                 //35
"Zone overlap",                  //36
"Saturation",                    //37
"Sensitivity",                   //38
"Filter length",                 //39
"Filter threshold",              //40
"Filter smoothness",             //41
"GDI capure framerate",          //42
"Filtermode:",                   //43
"source:",                       //44
"zone/ LED setup",               //45
"White-/ Color Adjustmet",       //46
"Gradienten...",                 //47
"Interlace rows",                //48
"name",                          //49
"save",                          //50
"delete",                        //51
"profiles ",                     //52
"load",                          //53
"default",                       //54
"Enhanced Live view (Windows 8 or higher)", //55
};

char *sTextMenu[MAX_MENU_STRINGS] = 
{"Setting",                //0
"Language",                //1
"Mode",                    //2
"no effect",               //3
"static color",            //4
"life",                    //5
"color changer",           //6
"color changer LR",        //7
"channel assignment",      //8
"Info",                    //9
"Exit",                    //10
"Profiles",                //11
"Nothing",                 //12
};

char *sTextColorPicker[MAX_COLORPICKER_STRINGS] = 
{"Red",                    //0
"Green",                   //1
"Blue",                    //2
"ColorPicker",             //3
"Cancel",                  //4
};

char *sTextAtmoGradients[MAX_GRADIENTSDIALOG_STRINGS] = 
{"Zone",                                                 //0
"Combined zone",                                         //1
"Edge weighting",                                        //2
"File",                                                  //3
"really overwrite?",                                     //4
"Question",                                              //5
"file cannot be overwritten.",                           //6 
"Error",                                                 //7
"Gradient loaded ok.",                                   //8
"Info",                                                  //9
"File not found",                                        //10
"File size / resolution is wrong!",                      //11
"File is probably no BMP file!",                         //12
"File has not the correct color depth (8-bit, 24-bit)?", //13
"Please only BMP drag files onto the surface!",          //14
"Gradient progressions",                                 //15
"Cancel",                                                //16
"Zones",                                                 //17
};

char *sTextWhiteSetup[MAX_WHITESETUP_STRINGS] = 
{"white",                                                 //0
"red",                                                    //1
"green",                                                  //2
"blue",                                                   //3
"yellow",                                                 //4
"magenta",                                                //5
"cyan",                                                   //6
"Cancel",                                                 //7
"color slider ",                                          //8  
"software gamma adjustment ",                             //9
"modus:",                                                 //10
"active",                                                 //11
"reset all",                                              //12
"reset to default value",                                 //13
"View Color",                                             //14
"greyscale adjustment ",                                  //15
"get value from LUT",                                     //16
"use 3D-Lut",                                             //17
"color/ white adjustment ",                               //18
"simple white adjustment",                                //19
"color adjustment",                                       //20
"!!! not yet implemented !!!",                            //21
"SEDU calibration mode",                                  //22
"invert colors",                                          //23
"disabled",                                               //24
"Each color individually",                                //25
};

char *sTextWhiteSetupHW[MAX_WHITESETUPHW_STRINGS] = 
{"Save to controller programming (EEPROM)...",            //0
"Cancel",                                                 //1
"Adjusting brightness...",                                //2
"contrast",                                               //3
"Red",                                                    //4
"Green",                                                  //5
"Blue",                                                   //6
"Gamma correction",                                       //7
"Global contrast",                                        //8
"The hardware white balance is possible only with an active connection to the hardware.", //9
"Error",                                                  //10
};

char *sTextAtmoDlg[MAX_ATMODLG_STRINGS] = 
{"cancel",                                                //0
"Interface:",                                             //1
"Connection...",                                          //2
"number of LED's",                                        //3
"Speed:",                                                 //4
};

char *sTextDmxDlg[MAX_DMXDLG_STRINGS] = 
{"cancel",                                                //0
"Interface:",                                             //1
"Connection...",                                          //2
"DMX start channels (separator "","" or "";"")",          //3
"Number of RGB channels (1..256):",                       //4
"Speed:",                                                 //5
"Configure simple DMX",                                   //6
};

char *sTextMultiAtmoLightDlg[MAX_MULTIATMOLIGHTDLG_STRINGS] = 
{"Configure Atmo light multi",                            //0
"Cancel",                                                 //1
"Interface 0",                                            //2
"Connection...",                                          //3
"Interface 1",                                            //4
"Interface 2",                                            //5
"Interface 3",                                            //6
"Disabled",                                               //7
};

char *sTextMomoDlg[MAX_MOMODLG_STRINGS] = 
{"Configure MoMoLight device",                            //0
"Cancel",                                                 //1
"Serialport:",                                            //2
"Connection...",                                          //3
"Device type:",                                           //4
"3 - Channels",                                           //5
"4 - Channels",                                           //6
};

char *sTextNordLightSetup[MAX_NORDLIGHTSETUP_STRINGS] = 
{"Configure Fnordlight device",                           //0
"Cancel",                                                 //1
"Serialport:",                                            //2
"Connection...",                                          //3
"Num Channels:",                                          //4
"Number of channels out of range 1..255",                 //5
"Error",                                                  //6
};

CString CLanguage::sChannelAssigmentText[MAX_CHANNELASSIGNMENT_STRINGS];
CString CLanguage::sSettingText[MAX_SETTINGSDIALOG_STRINGS];
CString CLanguage::sMenuText[MAX_MENU_STRINGS];
CString CLanguage::sTextCPicker[MAX_COLORPICKER_STRINGS];
CString CLanguage::sGradientsText[MAX_GRADIENTSDIALOG_STRINGS];
CString CLanguage::sWhiteSetupText[MAX_WHITESETUP_STRINGS];
CString CLanguage::sWhiteSetupHWText[MAX_WHITESETUPHW_STRINGS];
CString CLanguage::sAtmoDlgText[MAX_ATMODLG_STRINGS];
CString CLanguage::sDmxDlgText[MAX_DMXDLG_STRINGS];
CString CLanguage::sMultiAtmoLightDlgText[MAX_MULTIATMOLIGHTDLG_STRINGS];
CString CLanguage::sMomoDlgText[MAX_MOMODLG_STRINGS];
CString CLanguage::sNordLightSetupText[MAX_NORDLIGHTSETUP_STRINGS];
CString CLanguage::sMessagesText[MAX_MESSAGE_STRINGS];

char CLanguage::szFileINI[MAX_PATH];
char CLanguage::szLang[MAX_PATH], CLanguage::szTemp[512], CLanguage::Buffer[1024], CLanguage::szParam[32]; 

CLanguage::CLanguage(void)
{
}

CLanguage::~CLanguage(void)
{
}

int CLanguage::DirectoryExists(const char* dirName) 
{
	DWORD attribs = ::GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES) 
	{
		return false;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

void CLanguage::XMLParse(const char* FileName, CString *sInText, const char* Section)
{

	std::ostringstream ostrm;
	std::ifstream inFile(FileName);
	ostrm << inFile.rdbuf();

	TiXmlDocument doc;
	doc.Parse(ostrm.str().c_str()); 

	inFile.close();

	TiXmlElement *pRoot = doc.RootElement();
	if (pRoot)
	{
		TiXmlElement *element = pRoot->FirstChildElement();

		string attribute;
		while(attribute != Section)
		{
			string value = element->Value(); 
			attribute = element->Attribute("name");
			if (attribute == Section)
			{
				TiXmlElement *pChildNode = element->FirstChildElement();         
				while (pChildNode)
				{
					string name = pChildNode->Attribute("name");            
					string values = pChildNode->Attribute("value"); 

					int entry = atoi(name.c_str());

					strcpy(Buffer, values.c_str());

					sInText[entry] = Buffer;
					sInText[entry].Replace("\\t", "\t");
					sInText[entry].Replace("\\n", "\n");	

					pChildNode = pChildNode->NextSiblingElement();
				}
			}
			element = element->NextSiblingElement();
		}
	}
}

void CLanguage::CreateDefaultXML(const char* FileName, char* Section[])
{
	char *sInText[128];
	int Count;

	TiXmlDocument doc; 
	TiXmlDeclaration * xdeclaration = new TiXmlDeclaration( "1.0", "ISO-8859-1", "" ); 
	doc.LinkEndChild( xdeclaration ); 

	// first entry
	TiXmlElement * xEConfiguration = new TiXmlElement( "configuration" ); 
	doc.LinkEndChild( xEConfiguration ); 
	m_XmlOnInit = true;

	for(int j=0; j < MAX_SECTION_STRINGS; j++)
	{ 
		switch (j)
		{
		case 0:
			memcpy(sInText, sTextMenu, sizeof(sTextMenu));
			Count = MAX_MENU_STRINGS;
			break;
		case 1:
			memcpy(sInText, sTextChannelAssignment, sizeof(sTextChannelAssignment));
			Count = MAX_CHANNELASSIGNMENT_STRINGS;
			break;
		case 2:
			memcpy(sInText, sTextSettingDialog, sizeof(sTextSettingDialog));
			Count = MAX_SETTINGSDIALOG_STRINGS;
			break;
		case 3:
			memcpy(sInText, sTextAtmoGradients, sizeof(sTextAtmoGradients));
			Count = MAX_GRADIENTSDIALOG_STRINGS;
			break;
		case 4:
			memcpy(sInText, sTextColorPicker, sizeof(sTextColorPicker));
			Count = MAX_COLORPICKER_STRINGS;
			break;
		case 5:
			memcpy(sInText, sTextWhiteSetup, sizeof(sTextWhiteSetup));
			Count = MAX_WHITESETUP_STRINGS;
			break;
		case 6:
			memcpy(sInText, sTextWhiteSetupHW, sizeof(sTextWhiteSetupHW));
			Count = MAX_WHITESETUPHW_STRINGS;
			break;
		case 7:
			memcpy(sInText, sTextAtmoDlg, sizeof(sTextAtmoDlg));
			Count = MAX_ATMODLG_STRINGS;
			break;
		case 8:
			memcpy(sInText, sTextDmxDlg, sizeof(sTextDmxDlg));
			Count = MAX_DMXDLG_STRINGS;
			break;
		case 9:
			memcpy(sInText, sTextMultiAtmoLightDlg, sizeof(sTextMultiAtmoLightDlg));
			Count = MAX_MULTIATMOLIGHTDLG_STRINGS;
			break;
		case 10:
			memcpy(sInText, sTextMomoDlg, sizeof(sTextMomoDlg));
			Count = MAX_MOMODLG_STRINGS;
			break;
		case 11:
			memcpy(sInText, sTextNordLightSetup, sizeof(sTextNordLightSetup));
			Count = MAX_NORDLIGHTSETUP_STRINGS;
			break;
		case 12:
			memcpy(sInText, sMessages, sizeof(sMessages));
			Count = MAX_MESSAGE_STRINGS;
			break;
		}

		// Section Menu
		TiXmlElement * xESection = new TiXmlElement( "section" );
		xESection->SetAttribute("name", Section[j]);

		// Settings
		TiXmlElement * xESettings = new TiXmlElement( "setting" ); 


		// enumerate Line count from sTextMenu
		for(int i=0; i < Count; i++)
		{
			// convert integer(i) to string 
			std::string s = std::to_string(i);
			char const *pchar = s.c_str(); 

			// copy string buffer to *char
			char *buffer = new char[s.length()];
			strcpy(buffer,s.c_str());

			// First Attribut for Line(i)
			xESettings->SetAttribute("name", buffer);
			// Attribute Value for Line(i)

			xESettings->SetAttribute("value", sInText[i]);
			// End Line(i)
			xESection->InsertEndChild(*xESettings);
		}
		xESettings->LinkEndChild( xESection ); 
		xEConfiguration->LinkEndChild( xESection );
	}

	doc.SaveFile( FileName);

}

TCHAR* CLanguage::GetThisPath(TCHAR* dest, size_t destSize)
{
    if (!dest) return NULL;
    if (MAX_PATH > destSize) return NULL;

    DWORD length = GetModuleFileName( NULL, dest, destSize );
    PathRemoveFileSpec(dest);
		return dest;
}
