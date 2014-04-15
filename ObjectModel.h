#ifndef __OBJECTMODEL_H__
#define __OBJECTMODEL_H__

#include "shlobj.h" 

class CUtils
{

public:
  static char szCurrentDir[MAX_PATH];
  static char szTemp[512];
	static bool firststart;

	GString strConfigFromFile;
	static GStringList profiles;
	
	char GetSpecialFolder(int CLSID);
  int DirectoryExists(const char* dirName);

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