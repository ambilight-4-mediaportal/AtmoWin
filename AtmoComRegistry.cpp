#include "StdAfx.h"
#include "atmocomregistry.h"
#include <oleauto.h>

// #include "AtmoWin_h.h"


CAtmoComRegistry::CAtmoComRegistry(void)
{
}

CAtmoComRegistry::~CAtmoComRegistry(void)
{
}

//typedef void (__stdcall *OaEnablePerUserTLibRegistration)(void); 

void CAtmoComRegistry::SaveComSettings(ATMO_BOOL force) 
{
	CAtmoRegistry *reg = new CAtmoRegistry(HKEY_CLASSES_ROOT);
	char buffer[MAX_PATH];
	char exe[MAX_PATH];
	//    char tlb[MAX_PATH];
	wchar_t widePath[MAX_PATH];

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
		sprintf(buf, "LoadTypeLib failed. Code: 0x%.8x", res);
		MessageBox(0, buf, "Error" ,0);
	}

	/*
	S_OK
	E_OUTOFMEMORY
	E_INVALIDARG
	TYPE_E_IOERROR
	TYPE_E_REGISTRYACCESS --> Vista Problem mit Registry und dem UAC krams!
	TYPE_E_INVALIDSTATE 
	*/
	if(typeLib)
	{
		res = RegisterTypeLib(typeLib, widePath, 0);
		if(res == TYPE_E_REGISTRYACCESS)
		{
			HMODULE oleaut32 = GetModuleHandle("Oleaut32.dll");  
			if(oleaut32 != 0)
			{
				//OaEnablePerUserTLibRegistration oaEnablePerUserTLibRegistration = (OaEnablePerUserTLibRegistration )GetProcAddress(oleaut32, "OaEnablePerUserTLibRegistration");
				//if(oaEnablePerUserTLibRegistration)
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
				sprintf(buf, "RegisterTypeLib failed. Code: 0x%.8x", res);
				MessageBox(0, buf, "Error" ,0);
			}
		} 
		else 
		{
			if(force)
				MessageBox(0,"COM Server registered Ok!","Info",0);
		}
		typeLib->Release();
	}

	delete reg;
};

void CAtmoComRegistry::DeleteComSettings() 
{
};
