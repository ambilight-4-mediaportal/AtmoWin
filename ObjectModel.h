#pragma once

#include "shlobj.h" 

class CUtils
{

public:
  static char szCurrentDir[MAX_PATH];
  static char szTemp[512];
	
	GString strConfigFromFile;
		
	char GetSpecialFolder(int CLSID);
  bool DirectoryExists(const char* dirName);

	CUtils(void);
	virtual ~CUtils(void);
};

class MySection : public XMLObject
{
	virtual void MapXMLTagsToMembers();

public:
  GString m_strSectionName;
  GList m_lstMySettings;

	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY(MySection, section) 

	MySection(){} // keep one constructor with no arguments
	~MySection(){};
};

class MyConfiguration : public XMLObject
{

	virtual void MapXMLTagsToMembers();
public:
	// This list will hold instances of type MyCustomObject
	GList m_lstMyObjects;

	DECLARE_FACTORY(MyConfiguration, configuration);

	MyConfiguration(){};
	~MyConfiguration(){};

};

class MySetting : public XMLObject
{
	virtual void MapXMLTagsToMembers();
public:
	GString m_strName;
  GString m_strValue;

	DECLARE_FACTORY(MySetting, setting);

	MySetting(){}
	~MySetting(){}

};