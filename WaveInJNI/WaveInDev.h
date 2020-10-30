#pragma once
#include <MMSYSTEM.H>
#include <string>
#include <jni.h>

class GrowByteList
{
public:
	GrowByteList();
	~GrowByteList();

public:
	void			SetSize(int size){ m_dataSize = size; }
	unsigned char*	GetData(int offset){ return m_data + offset; }
	unsigned char*	GetData(){ return m_data; };
	unsigned int	GetSize(){ return m_dataSize; };
	unsigned char*	AddBytes(void* p, unsigned int size){ return AddBytes(p, size, -1); };
	unsigned char*	AddBytes(void* p, unsigned int size, int init);
	void			Clear(){ m_dataSize = 0; };
	void			Remove(int size);

protected:
	unsigned int	BYTE_BLOCK_SIZE;
	unsigned int	m_dataSize;
	unsigned int	m_bufSize;
	unsigned char*	m_data;
};

class CriticalSectionLock
{
public:
	CriticalSectionLock(CRITICAL_SECTION* cs)
	{
		m_cs = cs;
		EnterCriticalSection(m_cs);
	}

	~CriticalSectionLock()
	{
		LeaveCriticalSection(m_cs);
	}

	CRITICAL_SECTION* m_cs;

};



class CWaveInDev
{
public:
	CWaveInDev();
	~CWaveInDev();

public:
	BOOL				StartCapture(BOOL micMode, BOOL enableCOUT);
	BOOL				StopCapture(int timeOut);
	DWORD				WaveInDevThread();
	LPCTSTR				GetErrMsg(){return m_errMsg.c_str();};
	int					GetDataBufSize();
	int					PopDataBufToByteArray(JNIEnv * env, jbyteArray b, jint off, jint len);
	WCHAR				GetCurStatus();

private:
	void				SaveWaveDataToBuf(BYTE* pData, int size);
	void				SetErrMsg(LPCTSTR spErrMsg);

private:	
	int					m_sampleStep;
	int					m_blockAlign;
	BOOL				m_isMICMode;
	BOOL				m_enabelCOUT;
	volatile WCHAR		m_curStatus;
	CRITICAL_SECTION	m_csDataBuf;
	CRITICAL_SECTION	m_csThread;
	HANDLE				m_hStopEvent;
	HANDLE				m_hThread;
	std::wstring		m_errMsg;
	GrowByteList		m_recvBuf;
};

