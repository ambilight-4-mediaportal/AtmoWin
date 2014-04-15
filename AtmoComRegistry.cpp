#include "StdAfx.h"
#include "atmocomregistry.h"
#include <oleauto.h>
#include "Language.h"

CAtmoComRegistry::CAtmoComRegistry(void)
{
}

CAtmoComRegistry::~CAtmoComRegistry(void)
{
}


void CAtmoComRegistry::SaveComSettings(ATMO_BOOL force) 
{
	CAtmoRegistry *reg = new CAtmoRegistry(HKEY_CLASSES_ROOT);
	CLanguage *Lng = new CLanguage;

	char buffer[MAX_PATH];
	char exe[MAX_PATH];
	wchar_t widePath[MAX_PATH];

	Lng->szCurrentDir[Lng->GetSpecialFolder(CSIDL_COMMON_APPDATA)];	
	sprintf(Lng->szFileINI, "%s\\Language.ini\0", Lng->szCurrentDir);

	GetPrivateProfileString("Common", "Language", "English", Lng->szLang, 256, Lng->szFileINI);

	// Read Buffer from IniFile
	sprintf(Lng->szTemp, "%s\\%s.xml\0", Lng->szCurrentDir, Lng->szLang);


	Lng->XMLParse(Lng->szTemp, Lng->sMessagesText, "Messages");

	GetModuleFileName(GetModuleHandle(NULL),exe,MAX_PATH);

	LPOLESTR aguid = NULL;

	if((reg->RegistryKeyExists("AtmoRemoteControl.1") == ATMO_FALSE) || force) 
	{
		reg->WriteRegistryString("AtmoRemoteControl.1\\",NULL,"AtmoRemoteControl Basic Functions");
		StringFromCLSID(CLSID_AtmoRemoteControl,&aguid);
		reg->WriteRegistryString("AtmoRemoteControl.1\\CLSID\\",NULL,(char *)(_bstr_t)aguid);
		sprintf(buffer,"CLSID\\%s",(char *)(_bstr_t)aguid);
		reg->WriteRegistryString(buffer,NULL,"AtmoRemoteControl Basic Functions");
		sprintf(buffer,"CLSID\\%s\\LocalServer32",(char *)(_bstr_t)aguid);
		reg->WriteRegistryString(buffer,NULL,exe);
		sprintf(buffer,"CLSID\\%s\\ProgID",(char *)(_bstr_t)aguid);
		reg->WriteRegistryString(buffer,NULL,"AtmoRemoteControl.1");
		CoTaskMemFree(aguid);
	}

	if((reg->RegistryKeyExists("AtmoLiveViewControl.1") == ATMO_FALSE) || force) 
	{
		reg->WriteRegistryString("AtmoLiveViewControl.1\\",NULL,"Atmo Live View Control Object");

		StringFromCLSID(CLSID_AtmoLiveViewControl,&aguid);
		reg->WriteRegistryString("AtmoLiveViewControl.1\\CLSID\\",NULL,(char *)(_bstr_t)aguid);

		sprintf(buffer,"CLSID\\%s",(char *)(_bstr_t)aguid);
		reg->WriteRegistryString(buffer,NULL,"Atmo Live View Control Object");

		sprintf(buffer,"CLSID\\%s\\LocalServer32",(char *)(_bstr_t)aguid);
		reg->WriteRegistryString(buffer,NULL,exe);

		sprintf(buffer,"CLSID\\%s\\ProgID",(char *)(_bstr_t)aguid);
		reg->WriteRegistryString(buffer,NULL,"AtmoLiveViewControl.1");

		CoTaskMemFree(aguid);
	}


	//IAtmoRemoteControl
	StringFromIID(IID_IAtmoRemoteControl,&aguid);
	sprintf(buffer,"Interface\\%s",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"IAtmoRemoteControl");

	sprintf(buffer,"Interface\\%s\\ProxyStubClsid32",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"{00020424-0000-0000-C000-000000000046}");

	sprintf(buffer,"Interface\\%s\\TypeLib",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"{e01e2041-5afc-11d3-8e80-00805f91cdd9}"); // e01e2044-5bfc-11d4-8e80-00805f91cdd9r,"Version","1.0");
	CoTaskMemFree(aguid);


	//IAtmoRemoteControl2
	StringFromIID(IID_IAtmoRemoteControl2,&aguid);
	sprintf(buffer,"Interface\\%s",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"IAtmoRemoteControl2");

	sprintf(buffer,"Interface\\%s\\ProxyStubClsid32",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"{00020424-0000-0000-C000-000000000046}");

	sprintf(buffer,"Interface\\%s\\TypeLib",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"{e01e2041-5afc-11d3-8e80-00805f91cdd9}");
	reg->WriteRegistryString(buffer,"Version","1.0");
	CoTaskMemFree(aguid);

	//IID_IAtmoLiveViewControl
	StringFromIID(IID_IAtmoLiveViewControl,&aguid);
	sprintf(buffer,"Interface\\%s",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"IAtmoLiveViewControl");

	sprintf(buffer,"Interface\\%s\\ProxyStubClsid32",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"{00020424-0000-0000-C000-000000000046}");

	// {e01e2041-5afc-11d3-8e80-00805f91cdd9}
	sprintf(buffer,"Interface\\%s\\TypeLib",(char *)(_bstr_t)aguid);
	reg->WriteRegistryString(buffer,NULL,"{e01e2041-5afc-11d3-8e80-00805f91cdd9}");
	reg->WriteRegistryString(buffer,"Version","1.0");
	CoTaskMemFree(aguid);

	MultiByteToWideChar(CP_ACP, 0, exe, lstrlen(exe) + 1, widePath, MAX_PATH);
	ITypeLib *typeLib = NULL;
	HRESULT res;
	res = LoadTypeLib( widePath, &typeLib);
	if(res != S_OK) 
	{
		char buf[100];
		sprintf(buf, Lng->sMessagesText[22] + "0x%.8x", res);
		MessageBox(0, buf, Lng->sMessagesText[3] ,0);
	}

	if(typeLib)
	{
		res = RegisterTypeLib(typeLib, widePath, 0);
		if(res == TYPE_E_REGISTRYACCESS)
		{
			HMODULE oleaut32 = GetModuleHandle("Oleaut32.dll");  
			if(oleaut32 != 0)
			{
				{
					OaEnablePerUserTLibRegistration();
					res = RegisterTypeLib(typeLib, widePath, 0);
				}
			}
		}
		if( res != S_OK ) 
		{
			if(force)
			{
				char buf[100];
				sprintf(buf, Lng->sMessagesText[23] + "0x%.8x", res);
				MessageBox(0, buf, Lng->sMessagesText[3] ,0);
			}
		} 
		else 
		{
			if(force)
				MessageBox(0,Lng->sMessagesText[24],Lng->sMessagesText[1],0);
		}
		typeLib->Release();
	}

	delete reg;
};

void CAtmoComRegistry::DeleteComSettings() 
{
};
