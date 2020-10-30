#ifndef __JNI_PUBLIC_H
#define __JNI_PUBLIC_H
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <mutex>
#include <chrono>
#include <condition_variable>

#pragma warning (disable: 4996) 
#pragma warning (disable: 4267)

#define JNI_STRING_BUF_SIZE	1024

inline void JniThrowException(JNIEnv * env, LPCTSTR wmsg)
{
	size_t size = wcslen(wmsg) + 1;
	char* umsg = new char[size * 3];

	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)wmsg, -1, umsg, (int)size * 3, NULL, NULL);

	env->ThrowNew(env->FindClass("java/lang/Exception"), umsg);

	delete umsg;
}

inline void JniThrowException(JNIEnv * env, const char* msg)
{
	size_t size = strlen(msg) + 1;
	wchar_t* wmsg = new wchar_t[size];
	char* umsg = new char[size * 3];

	MultiByteToWideChar(CP_ACP, 0, msg, -1, wmsg, (int)size);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)wmsg, -1, umsg, (int)size * 3, NULL, NULL);

	env->ThrowNew(env->FindClass("java/lang/Exception"), umsg);

	delete wmsg;
	delete umsg;
}

inline void JniThrowExceptionFmt(JNIEnv * env, const char* fmt, ...)
{
	char sMsg[10240];
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsprintf_s(sMsg, fmt, arg_ptr);
	va_end(arg_ptr);
	JniThrowException(env, sMsg);
}

inline jstring JniNewWString(JNIEnv * env, LPCTSTR pwstr)
{
	jstring str = env->NewString((jchar*)pwstr, wcslen(pwstr));
	return str;
}

inline jstring JniNewString(JNIEnv * env, const char* value)
{
	wchar_t wstr[1024];
	wchar_t* pwstr = wstr;
	size_t size = strlen(value) + 1;
	if (size > (sizeof wstr / sizeof wstr[0]))
		pwstr = new wchar_t[size];

	MultiByteToWideChar(CP_ACP, 0, value, -1, pwstr, (int)size);
	jstring str = env->NewString((jchar*)pwstr, wcslen(pwstr));

	if (pwstr != wstr)
		delete pwstr;

	return str;
}

inline bool JniSeekFilePos(JNIEnv * env, FILE* file, __int64 pos)
{
	_fseeki64(file, pos, SEEK_SET);
	__int64 rpos = _ftelli64(file);
	if (rpos != pos)
	{
		JniThrowExceptionFmt(env, "seek file error:%lu->%lu", pos, rpos);
		return false;
	}
	return true;
}

class JniWString
{
public:
	JniWString(JNIEnv * env, jstring jstr)
	{
		_env = env;
		_jstr = jstr;
		_pstr = env->GetStringChars(_jstr, NULL);
	}

	~JniWString()
	{
		_env->ReleaseStringChars(_jstr, _pstr);
	}

public:
	size_t		size()
	{
		return wcslen((wchar_t*)_pstr);
	}

	LPCTSTR	c_str()
	{
		return (LPCTSTR)_pstr;
	}

private:
	const jchar*	_pstr;
	jstring			_jstr;
	JNIEnv *		_env;
};


class JniString
{
public:
	JniString(JNIEnv * env, jstring jstr)
	{
		_env = env;
		_jstr = jstr;
		_pstr = env->GetStringChars(_jstr, NULL);
		size_t len = wcslen((wchar_t*)_pstr) * 2 + 1;
		if (len < JNI_STRING_BUF_SIZE)
			_pbuf = _buf;
		else
			_pbuf = new char[len];

		WideCharToMultiByte(CP_ACP, 0, (LPCWCH)_pstr, -1, _pbuf, (int)len, NULL, NULL);
		len = 0;
	}

	~JniString()
	{
		if (_pbuf != _buf)
			delete _pbuf;
		_env->ReleaseStringChars(_jstr, _pstr);
	}

public:
	size_t		size()
	{
		return wcslen((wchar_t*)_pstr);
	}

	const char*	c_str()
	{
		return _pbuf;
	}

private:
	const jchar*	_pstr;
	jstring			_jstr;
	JNIEnv *		_env;
	char*			_pbuf;
	char			_buf[JNI_STRING_BUF_SIZE];
};

class ReadWriteLock
{
public:
	void getReadLock()
	{
		std::unique_lock<std::mutex> lock(m);
		while (writerUsed == true)
		{
			cv.wait(lock);
		}
		readerCount++;
	}
	void getWriteLock()
	{
		std::unique_lock<std::mutex> lock(m);
		while (readerCount != 0 || writerUsed == true)
		{
			cv.wait(lock);
		}
		writerUsed = true;
	}
	void unlockReader()
	{
		std::unique_lock<std::mutex> lock(m);
		readerCount--;
		if (readerCount == 0)
		{
			cv.notify_all();
		}
	}
	void unlockWriter()
	{
		std::unique_lock<std::mutex> lock(m);
		writerUsed = false;
		cv.notify_all();
	}

private:
	int readerCount = 0;
	bool writerUsed = false;
	std::mutex m;
	std::condition_variable cv;
};
#endif