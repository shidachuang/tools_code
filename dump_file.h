#pragma once

#include <time.h>
#include <Windows.h>
#include <Dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")

long __stdcall crush_callback(_EXCEPTION_POINTERS* ep)
{
	char fname[256] = { 0 };
	time_t time_seconds = time(0);
	struct tm now_time;
	localtime_s(&now_time, &time_seconds);
	sprintf_s(fname, "dump_%d-%d-%d_%d_%d_%d.DMP", 1900 + now_time.tm_year, 1 + now_time.tm_mon, now_time.tm_mday, (now_time.tm_hour) % 24, now_time.tm_min, now_time.tm_sec);

	//TCHAR *pStr = A2T(fname);
	HANDLE hFile = CreateFileA(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	MINIDUMP_EXCEPTION_INFORMATION    exceptioninfo;
	exceptioninfo.ExceptionPointers = ep;
	exceptioninfo.ThreadId = GetCurrentThreadId();
	exceptioninfo.ClientPointers = FALSE;

	if (!MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hFile,
		MiniDumpWithFullMemory, //这里会将程序运行时的内存写入磁盘，文件较大，如果不需要这些及时的内存信息，用：MiniDumpNormal
		&exceptioninfo,NULL,NULL))
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	CloseHandle(hFile);
	return EXCEPTION_EXECUTE_HANDLER;
}



#define _DUMP_FILE_FUN_		SetUnhandledExceptionFilter(crush_callback);