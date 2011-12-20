// Define macro's for exporting methods
#ifdef RFIDLOGINPLUGIN_EXPORTS
#define RFIDPLUGIN_API extern "C" __declspec(dllexport)
#else
#define RFIDPLUGIN_API  extern "C" __declspec(dllimport)
#endif /* DUMMYPLUGIN_EXPORTS */

typedef struct pGinaInfo {
		LPTSTR pathMSGina;
		LPTSTR pathPlugin;
		LPTSTR pathProfile;
		LPTSTR mapPaths;
		LPTSTR Username;
		LPTSTR Password;
		LPTSTR homeDir;
		BOOL isAdmin;
		BOOL disabled;
		int authType;
		HANDLE hUser;
		LPTSTR userGroups;
		LPTSTR userDescription;
		LPTSTR userFullName;
		BOOL allowPassChange;
		LPTSTR errorString;
		LPTSTR defaultDomain;
		BOOL Reserved3;
		BOOL Reserved4;
} pGinaInfo ;



// Export these methods... - REQUIRED
RFIDPLUGIN_API  BOOL UserLogin(LPTSTR, LPTSTR,pGinaInfo *);			
RFIDPLUGIN_API  BOOL ChangeUserPassword(LPTSTR,LPTSTR,LPTSTR);
RFIDPLUGIN_API  LPCTSTR AboutPlugin(void);
RFIDPLUGIN_API  void ChangePluginSettings(void);
RFIDPLUGIN_API  void LoginHook(pGinaInfo *);
RFIDPLUGIN_API 	void LogoutHook(pGinaInfo *);

// Option method with the 1.6.x series and up of pGina
RFIDPLUGIN_API  BOOL IsRequired(void);
