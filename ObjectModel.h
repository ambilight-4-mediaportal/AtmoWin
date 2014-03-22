#pragma once

#ifndef _OBJECT_MODEL_H__
#define _OBJECT_MODEL_H__

#include "xmlObject.h"
#include "GString.h"
#include "GProfile.h"
#include "shlobj.h" 

extern int g_verify0ref;

using namespace std;

class CUtils
{

public:
  static char szCurrentDir[MAX_PATH];
  static char szTemp[512];
	static char szName[MAX_PATH];
	
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


#endif //_OBJECT_MODEL_H__

