/*
Adapted from pGina Dummy plugin skeleton example code 
*/

#include "stdafx.h"

#include <wchar.h>
#include <tchar.h>
#include <cstdio>

#include "rfidLoginPlugin.h"
#include "users.h"

static int getUIDfromNFC();
static void printCStr(char * cString);
static void printCStr(char * cString, char *title);

// This isn't strictly necessary, but can be handy if you are going to add Dialog's etc
BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/* This is the function that pGina calls to find out whether a user is authorized or not
it is also the place to put changes in per-user configurations etc. */
RFIDPLUGIN_API  BOOL UserLogin(LPTSTR Username, LPTSTR Password, pGinaInfo *settingsInfo)
{
	// Validate user bsed on RFID tag	

	// Launch process to read UID from  rfid card
	int uid = getUIDfromNFC();
	for (int i=0; i<MAX_ITEMS_IN_DB; ++i)
	{
		if( (uid==store[i].uid) /*&& (!_tcscmp(store[i].userName, Username))*/)
		{
			// found match in db
			settingsInfo->Username = _tcsdup(store[i].userName);
			settingsInfo->Password = _tcsdup(store[i].password);

			return true;
		}
	}	

	// Nobody else gets in, unless they have a local account
	settingsInfo->errorString = _tcsdup(TEXT("The username was not recognized."));
	return false;
}


RFIDPLUGIN_API  BOOL ChangeUserPassword(LPTSTR Username,LPTSTR OldPassword,LPTSTR NewPassword)
{
	return true;
}
RFIDPLUGIN_API  LPCTSTR AboutPlugin(void)
{
	// Return a sample descriptions string
	return TEXT("Authenticates users based on RFID. \
				Fall back to local machine username/password authentication.");
}
RFIDPLUGIN_API  void ChangePluginSettings(void)
{
	// Describe in a messagebox what this would do
	MessageBox(NULL,TEXT("This would bring up settings... but this is a simple plugin, so no settings."),TEXT("ChangePluginSettings()"),MB_OK);
}
RFIDPLUGIN_API  void LoginHook(pGinaInfo *settingsInfo)
{

}
RFIDPLUGIN_API  void LogoutHook(pGinaInfo *settingsInfo)
{

}

RFIDPLUGIN_API  BOOL IsRequired(void)
{
	return false;
}

static int getUIDfromNFC(){
	int ret = -1;

	/* CreateProcess API initialization */ 
	STARTUPINFOW siStartupInfo; 
	PROCESS_INFORMATION piProcessInfo; 

	memset(&siStartupInfo, 0, sizeof(siStartupInfo)); 
	memset(&piProcessInfo, 0, sizeof(piProcessInfo)); 
	siStartupInfo.cb = sizeof(siStartupInfo); 

#define baseDir		TEXT("d:\\infosec\\rfid\\nfcLoginPlugin\\other resources\\")
#define procName	TEXT("nfc-mfultralight-getid.exe")
#define proc baseDir procName

	if (!CreateProcessW(proc, 
		proc,
		0, 0, false, 
		CREATE_NEW_CONSOLE, 0, 0, 
		&siStartupInfo, &piProcessInfo) != false) 
	{ 
		/* CreateProcess failed */ 			
		MessageBox(NULL, TEXT("Create process failed"), TEXT("Error"),MB_OK);
	}
	else 
	{
		/* Watch the process. */ 
		DWORD dwExitCode = 0; 
		DWORD SecondsToWait = 5;
		char code[100];
		

		// Wait for the external process to finish.
		WaitForSingleObject(piProcessInfo.hProcess, (SecondsToWait * 1000)); 			
		if (!GetExitCodeProcess(piProcessInfo.hProcess, &dwExitCode))
		{
			// Handle error
		}
		else 
		{
			sprintf_s(code, "%d", dwExitCode);
			//printCStr(code, "Read tag UID:");
			ret = dwExitCode;
		}

		/* Release handles */ 
		CloseHandle(piProcessInfo.hProcess); 
		CloseHandle(piProcessInfo.hThread); 
	}

	return ret;
}

static void printCStr(char * cString, char *title){
	// Get the size of the string by setting the 4th parameter to -1:
	DWORD textLen = MultiByteToWideChar (CP_ACP, 0, cString, -1, NULL, 0);
	DWORD titleLen = MultiByteToWideChar (CP_ACP, 0, title, -1, NULL, 0);

	wchar_t *wText = new wchar_t[textLen];
	wchar_t *wTitle = new wchar_t[titleLen];

	MultiByteToWideChar(  CP_ACP, NULL, cString, -1, wText, textLen );	
	MultiByteToWideChar(  CP_ACP, NULL, title, -1, wTitle, titleLen );	

	MessageBox(NULL, wText, wTitle, MB_OK);

	delete[] wText;
	delete[] wTitle;
}