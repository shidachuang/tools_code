#include "FileConfig.h"

BOOL APIENTRY InitConfigFile(LPCTSTR pFileName, LPTSTR pOutPath)
{
	BOOL  bRet = FALSE;
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };


	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	LPTSTR pos = StrRChr(szFilePath, NULL, '\\');
	if (pos != NULL)
	{
		pos++;
		*pos = 0;
	}

	lstrcpy(pos, pFileName);
	if (!PathFileExists(szFilePath))
	{
		HANDLE hFile = CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			GetLastError();
			bRet = FALSE;
		}
		else
			bRet = TRUE;
	}
	if (pOutPath != NULL && bRet == TRUE)
	{
		lstrcpy(pOutPath, szFilePath);
		bRet = TRUE;
	}

	return bRet;
}

tstring APIENTRY GetConfigPath()
{
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	LPTSTR pos = StrRChr(szFilePath, NULL, '\\');
	if (pos != NULL)
	{
		pos++;
		*pos = 0;
	}
	lstrcpy(pos, APP_CONFIG_FILE_NAME);

	return szFilePath;
}
