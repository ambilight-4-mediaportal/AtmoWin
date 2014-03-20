#include "stdafx.h"
#include "ObjectModel.h"
#include "MemberDescriptor.h"

IMPLEMENT_FACTORY(MyConfiguration, configuration)
IMPLEMENT_FACTORY(MySection, section)
IMPLEMENT_FACTORY(MySetting ,	setting)

int g_verify0ref = 0;

char CUtils::szCurrentDir[MAX_PATH];
char CUtils::szTemp[512];

GString strConfigFromFile;

void MyConfiguration::MapXMLTagsToMembers()
{
	// All of the <Section>'s under <Configuration> goes into this list
	MapMember(&m_lstMyObjects, MySection::GetStaticTag());
}

void MySetting::MapXMLTagsToMembers()
{
  MapAttribute(&m_strName,"name");
  MapAttribute(&m_strValue,"value");
}

void MySection::MapXMLTagsToMembers()
{
  MapAttribute(&m_strSectionName, "name");
  MapMember(&m_lstMySettings, MySetting::GetStaticTag());
}

CUtils::CUtils(void)
{
}

CUtils::~CUtils(void)
{
}

bool CUtils::DirectoryExists(const char* dirName) 
{
	DWORD attribs = ::GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES) 
	{
		return false;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

char CUtils::GetSpecialFolder(int CLSID) 
{
	LPITEMIDLIST  pidl ; 
	LPMALLOC      pShellMalloc; 

	// SHGetSpecialFolderLocation generates a PIDL. The memory for the PIDL 
	// is allocated by the shell, and should be freed using the shell 
	// mallocator COM object. Use SHGetMalloc to retrieve the malloc object 
	if(SUCCEEDED(SHGetMalloc(&pShellMalloc))) 
	{ 
		// if we were able to get the shell malloc object, then 
		// proceed by fetching the pidl for the windows desktop 
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, 
			CLSID, &pidl))) 
		{ 
			// return is true if success 
			if(SHGetPathFromIDList(pidl, (char*)szCurrentDir)) 
			{ 
				strcat(szCurrentDir, "\\Team MediaPortal\\MediaPortal\\AtmoWin");
				return *szCurrentDir;
			} 

			pShellMalloc->Free(pidl); 
		} 

		pShellMalloc->Release(); 
	} 
}