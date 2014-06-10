#ifndef __OBJECTMODEL_H__
#define __OBJECTMODEL_H__

#include "atlstr.h"
#include "shlobj.h" 
#include <string>

using namespace std; 

typedef struct profil 
{
 char *name;
} Profil;

class CUtils
{
private:
	TCHAR* GetThisPath(TCHAR* dest, size_t destSize);

public:
  static char szCurrentDir[MAX_PATH];
	static char szTemp[512];
	static bool firststart;

	GString strConfigFromFile;
	static GStringList profiles;
	
  int DirectoryExists(const char* dirName);
	CString GetFileVersionString(CString FileName);
	char SetSettingsPath();

	CUtils(void);
	virtual ~CUtils(void);
};

class MySection : public XMLObject
{
public:
  GString m_strSectionName;
  GList m_lstMySettings;
	
	virtual void MapXMLTagsToMembers();

	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY(MySection, section) 

	MySection(){} // keep one constructor with no arguments
	~MySection(){};
};

class MyConfiguration : public XMLObject
{
public:
	// This list will hold instances of type MyCustomObject
	GList m_lstMyObjects;
	
	virtual void MapXMLTagsToMembers();

	DECLARE_FACTORY(MyConfiguration, configuration);

	MyConfiguration(){};
	~MyConfiguration(){};
};

class MySetting : public XMLObject
{
public:
	GString m_strName;
  GString m_strValue;
	
	virtual void MapXMLTagsToMembers();

	DECLARE_FACTORY(MySetting, setting);

	MySetting(){}
	~MySetting(){}
};

#endif // __OBJECTMODEL_H__