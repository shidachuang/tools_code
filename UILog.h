#ifndef Trimiles_UILog_h
#define Trimiles_UILog_h

#include <iostream>
#include <fstream>
#include "shlwapi.h"
#include "time.h"
#include <string>
using namespace std;
#pragma comment(lib,"Shlwapi.lib")


static string UILog_GetFileDir(HMODULE aHMODULE = NULL)
{
	char buff[_MAX_PATH];
	string lstrRet ;

	char path_buffer[_MAX_PATH] = {0};
	char drive[_MAX_DRIVE] = {0};
	char dir[_MAX_DIR] = {0};
	char fname[_MAX_FNAME] = {0};
	char ext[_MAX_EXT] = {0};

//	lstrRet = GetCommandLineA();
	GetModuleFileNameA(aHMODULE, buff, _MAX_PATH);
	lstrRet = buff;
	int liFind = lstrRet.rfind('\\');
	if (liFind!=string::npos)
	{
		return lstrRet.substr(0, liFind);
	}
	
	return lstrRet;
}


class UIFileLog{
public:
    string mstrLogFile;
	string mstrPath;
	string mstrLogPath;
    bool mbOutLog;
	bool mbAutoAddTime;
    UIFileLog(){
		mbAutoAddTime = false;
		mbOutLog = false;

		ReNew();
	}
	void ReNew(HMODULE aHMODULE = NULL, const string & astrHead = "")
	{
		mstrPath = UILog_GetFileDir(aHMODULE);
		string lstrLogCfg = mstrPath + "\\" + "debuglog.cfg";
		if (PathFileExistsA(lstrLogCfg.c_str()))
		{
			mbOutLog = true;
		}
#ifdef _DEBUG
        mbOutLog = true;
#endif         
        if (mbOutLog){
			if (mstrLogPath.empty())
			{
				mstrLogPath = mstrPath;
			}
			mstrLogFile = mstrLogPath + "\\log";
			CreateDirectoryA(mstrLogFile.c_str(), NULL);
            char buf[512] = {0};
            time_t t;struct tm * pt;
            time(&t);
            pt = localtime(&t);
            pt->tm_year += 1900;
            pt->tm_mon++;
            
            sprintf(buf, "\\%s%04d%02d%02d_%02d%02d%02d.log", astrHead.c_str(), pt->tm_year, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
            
            mstrLogFile += buf;
        }            
    }
	void Logf(const char * szFormat, ...)
	{
		DWORD liLastError = GetLastError();

		char  _buffer[1025] = {0};
		va_list	argList;
		va_start(argList, szFormat);
		_vsnprintf(_buffer, 1024, szFormat, argList);
		va_end(argList);
#ifdef _DEBUG
#ifdef UILOG_PRINT
		printf(_buffer);
		printf("\n");
#else
		OutputDebugStringA(_buffer);
		OutputDebugStringA("\n");
#endif
#endif
		Log(_buffer);

		SetLastError(liLastError);
	}
	void LogBuff(const char * aszLog, int aiSize){
		if (mbOutLog){
			ofstream lodiskFile;	            
			lodiskFile.open(mstrLogFile.c_str(), ios::app|ios::binary);
			if (lodiskFile.is_open())
			{
				lodiskFile.write("\r\n", 2); 
				lodiskFile.write(aszLog, aiSize); 
				lodiskFile.write("\r\n", 2); 
				lodiskFile.close();  
			}
		}            
	}
	void LogEx(const char * aszLog){
		if (mbOutLog){
			ofstream lodiskFile;	            
			lodiskFile.open(mstrLogFile.c_str(), ios::app|ios::binary);

			if (lodiskFile.is_open())
			{
				lodiskFile.write(aszLog, strlen(aszLog)); 
				lodiskFile.close();  
			}

		}            
	}

    void Log(const char * aszLog){
        if (mbOutLog){
            ofstream lodiskFile;	            
            lodiskFile.open(mstrLogFile.c_str(), ios::app|ios::binary);

			if (lodiskFile.is_open())
			{
				if (mbAutoAddTime)
				{
					time_t t;
					struct tm * pt;
					char buftime[1025] = {0};

					time(&t);
					pt = localtime(&t);
					pt->tm_year += 1900;
					pt->tm_mon++;
					_snprintf(buftime, sizeof(buftime) - 1, "%04d-%02d-%02d %02d:%02d:%02d\t", pt->tm_year, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
					lodiskFile.write(buftime, strlen(buftime)); 
				}
	            lodiskFile.write(aszLog, strlen(aszLog)); 
		        lodiskFile.write("\r\n", 2); 
				lodiskFile.close();  
			}
            
        }            
    }
    bool OutLog(){
        return mbOutLog;
    }

    static UIFileLog & Single(){
        static UIFileLog g_log;
        return g_log;
    }
    
    
};

#include "windows.h"
class UILog{
public:
	static void dBig(int aiBuffLen, const char * asrTag, const char * szFormat, ...)
	{
		DWORD liLastError = GetLastError();
#ifndef _DEBUG
		if (!UIFileLog::Single().OutLog())
		{
			SetLastError(liLastError);
			return;
		}
#endif

		time_t t;
		struct tm * pt;
		char * buf = new char[aiBuffLen + 1];

		time(&t);
		pt = localtime(&t);
		pt->tm_year += 1900;
		pt->tm_mon++;

		_snprintf(buf, aiBuffLen, "%04d-%02d-%02d %02d:%02d:%02d\t%s", pt->tm_year, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec,szFormat);

		char  * _buffer = new char[aiBuffLen + 1];
		va_list	argList;
		va_start(argList, szFormat);
		_vsnprintf(_buffer, aiBuffLen, buf, argList);
		va_end(argList);
#ifdef _DEBUG
#ifdef UILOG_PRINT
		printf(_buffer);
		printf("\n");
		OutputDebugStringA(_buffer);
		OutputDebugStringA("\n");

#else
		OutputDebugStringA(_buffer);
		OutputDebugStringA("\n");
#endif
#endif
		if (!UIFileLog::Single().OutLog())
		{
			SetLastError(liLastError);
			return;
		}
		UIFileLog::Single().Log(_buffer);

		SetLastError(liLastError);

		delete [] buf; 
		delete [] _buffer;

	}

	static void d(const char * asrTag, const char * szFormat, ...)
	{
		DWORD liLastError = GetLastError();
#ifndef _DEBUG
		if (!UIFileLog::Single().OutLog())
		{
			SetLastError(liLastError);
			return;
		}
#endif
      
		time_t t;
		struct tm * pt;
		char buf[1024] = {0};
        
		time(&t);
		pt = localtime(&t);
		pt->tm_year += 1900;
		pt->tm_mon++;
		
		_snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d[%d_%s]\t%s", pt->tm_year, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec, GetCurrentThreadId(),asrTag, szFormat);
        
		char  _buffer[1025] = {0};
		va_list	argList;
		va_start(argList, szFormat);
		_vsnprintf(_buffer, 1024, buf, argList);
		va_end(argList);
#ifdef _DEBUG
#ifdef UILOG_PRINT
		printf(_buffer);
		printf("\n");
		OutputDebugStringA(_buffer);
		OutputDebugStringA("\n");

#else
		OutputDebugStringA(_buffer);
		OutputDebugStringA("\n");
#endif
#endif
		if (!UIFileLog::Single().OutLog())
		{
			SetLastError(liLastError);
			return;
		}
		UIFileLog::Single().Log(_buffer);

		SetLastError(liLastError);
	}
	static void e(const char * asrTag, const char * szFormat, ...)
	{
		DWORD liLastError = GetLastError();
#ifndef _DEBUG
		if (!UIFileLog::Single().OutLog())
		{
			SetLastError(liLastError);
			return;
		}
#endif
		time_t t;
		struct tm * pt;
		char buf[1024] = {0};
        
		time(&t);
		pt = localtime(&t);
		pt->tm_year += 1900;
		pt->tm_mon++;
        
		_snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d\t%s", pt->tm_year, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec,szFormat);
        
		char  _buffer[1025] = {0};
		va_list	argList;
		va_start(argList, szFormat);
		_vsnprintf(_buffer, 1024, buf, argList);
		va_end(argList);
#ifdef UILOG_PRINT
		printf(_buffer);
		printf("\n");
		OutputDebugStringA(_buffer);
		OutputDebugStringA("\n");

#else
		OutputDebugStringA(_buffer);
        OutputDebugStringA("\n");
#endif
		if (!UIFileLog::Single().OutLog())
		{
			SetLastError(liLastError);
			return;
		}
		UIFileLog::Single().Log(_buffer);

		SetLastError(liLastError);
	}
    
};


#endif
	