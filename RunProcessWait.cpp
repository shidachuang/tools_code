#include "stdafx.h"
#include <Tlhelp32.h>

// 进程启动结束监控


int RunProcessScan(LPTSTR szExec)
{
	int						iRet = 0;
	STARTUPINFO				si;
	PROCESS_INFORMATION		pi;
	PROCESSENTRY32			pe32;
	//HANDLE					hProcessSnap;


	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL, szExec, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		return -1;

	bool bIsRuning = true;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


	while (true)
	{
		ZeroMemory(&pe32, sizeof(PROCESSENTRY32));

		HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32First(hProcessSnap, &pe32))
		{
			CloseHandle(hProcessSnap);
			return -2;
		}

		while (Process32Next(hProcessSnap, &pe32))
		{
			if (pe32.th32ProcessID == pi.dwProcessId)
			{
				bIsRuning = true;
				break;
			}
			else
				bIsRuning = false;
		}

		if (!bIsRuning)
		{
			iRet = 1;
			break;
		}

		CloseHandle(hProcessSnap);
	}
	return iRet;
}
