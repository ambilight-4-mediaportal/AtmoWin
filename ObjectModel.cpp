#include "stdafx.h"
#include "ObjectModel.h"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

IMPLEMENT_FACTORY(MyConfiguration, configuration)
IMPLEMENT_FACTORY(MySection, section)
IMPLEMENT_FACTORY(MySetting ,	setting)

char CUtils::szCurrentDir[MAX_PATH];
char CUtils::szTemp[512];
bool CUtils::firststart;

GString strConfigFromFile;
GStringList CUtils::profiles;

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

int CUtils::DirectoryExists(const char* dirName) 
{
	DWORD attribs = ::GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES) 
	{
		return false;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

TCHAR* CUtils::GetThisPath(TCHAR* dest, size_t destSize)
{
    if (!dest) return NULL;
    if (MAX_PATH > destSize) return NULL;

    DWORD length = GetModuleFileName( NULL, dest, destSize );
    PathRemoveFileSpec(dest);
		return dest;
}

char CUtils::SetSettingsPath()
{
	memset(&szCurrentDir[0], 0, sizeof(szCurrentDir));

	GetThisPath(szCurrentDir , MAX_PATH);

	CString str = _T("\\Settings");
	strcat(szCurrentDir, str);

	return *szCurrentDir;
}

CString CUtils::GetFileVersionString(CString FileName)
 {

 char* fName = new char [strlen(FileName) +1];
 strcpy(fName, FileName);

 DWORD nSize = GetFileVersionInfoSize(fName, 0);
 CString Result = "";

 if (nSize)
 {
 char *pInfo = new char[nSize];

 GetFileVersionInfo(fName, 0, nSize, pInfo);

 VS_FIXEDFILEINFO *FileInfo;
 UINT uLen;

 VerQueryValue(pInfo, "\\", (void**) &FileInfo, (PUINT) &uLen);

 Result.Format("%d.%d.%d.%d",
 HIWORD(FileInfo->dwProductVersionMS),
 LOWORD(FileInfo->dwProductVersionMS),
 HIWORD(FileInfo->dwProductVersionLS),
 LOWORD(FileInfo->dwProductVersionLS));

 delete[] fName;
 }

 return Result;
 }

