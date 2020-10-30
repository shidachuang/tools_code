#ifndef _FILE_CONFIG_H_
#define _FILE_CONFIG_H_
#pragma once

#include <windows.h>
#include <string>
#include <Shlwapi.h>
using namespace std;

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;

#endif

#define APP_CONFIG_FILE_NAME				TEXT("config.ini")

BOOL APIENTRY InitConfigFile(LPCTSTR pFileName,LPTSTR pOutPath);

tstring APIENTRY GetConfigPath();

#endif
