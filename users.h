#ifndef __USERS_H__
#define __USERS_H__

typedef struct authInfo {
	LPTSTR		userName;
	LPTSTR		password;
	int			uid;
}authInfo;

#define MAX_ITEMS_IN_DB 10

authInfo store[MAX_ITEMS_IN_DB] = {
	{TEXT("liviu"),		TEXT("testpass"),			-1169970172},
	{TEXT("test"),		TEXT("test"),				-1213}};

#endif /* __USERS_H__ */