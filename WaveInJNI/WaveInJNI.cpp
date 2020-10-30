// WaveInJNI.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "com_wavein_WaveInDev.h"
#include "jni_public.h"
#include "WaveInDev.h"

#include <MMSYSTEM.H>
#pragma comment(lib, "WINMM.LIB")


void JniThrowMMRESULT(JNIEnv * env, MMRESULT mRet)
{
	LPCTSTR msg = L"unknow wavein error";
	switch(mRet)
	{
	case MMSYSERR_ERROR:
		msg = L"unspecified error";
		break;

	case MMSYSERR_BADDEVICEID:
		msg = L"device ID out of range";
		break;

	case MMSYSERR_NOTENABLED:
		msg = L"driver failed enable";
		break;
		
	case MMSYSERR_ALLOCATED:
		msg = L"device already allocated";
		break;
		
	case MMSYSERR_INVALHANDLE:
		msg = L"device handle is invalid";
		break;
		
	case MMSYSERR_NODRIVER:
		msg = L"no device driver present";
		break;
		
	case MMSYSERR_NOMEM:
		msg = L"memory allocation error";
		break;
		
	case MMSYSERR_NOTSUPPORTED:
		msg = L"function isn't supported";
		break;
		
	case MMSYSERR_BADERRNUM:
		msg = L"error value out of range";
		break;
		
	case MMSYSERR_INVALFLAG:
		msg = L"invalid flag passed";
		break;
		
	case MMSYSERR_INVALPARAM:
		msg = L"invalid parameter passed";
		break;
		
	case MMSYSERR_HANDLEBUSY:
		msg = L"handle being used";
		break;
		
	case MMSYSERR_INVALIDALIAS:
		msg = L"specified alias not found";
		break;
		
	case MMSYSERR_BADDB:
		msg = L"bad registry database";
		break;
		
	case MMSYSERR_KEYNOTFOUND:
		msg = L"registry key not found";
		break;
		
	case MMSYSERR_READERROR:
		msg = L"registry read error";
		break;
		
	case MMSYSERR_WRITEERROR:
		msg = L"registry write error";
		break;
		
	case MMSYSERR_DELETEERROR:
		msg = L"registry delete error";
		break;
		
	case MMSYSERR_VALNOTFOUND:
		msg = L"registry value not found";
		break;
		
	case MMSYSERR_NODRIVERCB:
		msg = L"driver does not call DriverCallback";
		break;

	case MMSYSERR_MOREDATA:
		msg = L"more data to be returned";
		break;
	}
}

jlong JNICALL Java_com_wavein_WaveInDev_rawNewWaveInDev(JNIEnv *, jclass)
{
	CWaveInDev* dev = new CWaveInDev();
	return (jlong)dev;
}

void JNICALL Java_com_wavein_WaveInDev_rawDeleteWaveInDev(JNIEnv *, jclass, jlong ptrWaveInDev)
{
	CWaveInDev* dev = (CWaveInDev*)ptrWaveInDev;
	delete dev;
}

void JNICALL Java_com_wavein_WaveInDev_rawStartCapture(JNIEnv * env, jclass, jlong ptrWaveInDev, jboolean micMode, jboolean debugLog)
{
	CWaveInDev* dev = (CWaveInDev*)ptrWaveInDev;
	if(!dev->StartCapture(micMode == JNI_TRUE, debugLog == JNI_TRUE))
	{
		JniThrowException(env, dev->GetErrMsg());
		return;
	}
}

void JNICALL Java_com_wavein_WaveInDev_rawStopCapture(JNIEnv * env, jclass, jlong ptrWaveInDev, jint timeout)
{
	CWaveInDev* dev = (CWaveInDev*)ptrWaveInDev;
	if(!dev->StopCapture(timeout) || dev->GetErrMsg()[0] != 0)
	{
		JniThrowException(env, dev->GetErrMsg());
		return;
	}
}

jint JNICALL Java_com_wavein_WaveInDev_rawReadBytes(JNIEnv * env, jclass, jlong ptrWaveInDev, jbyteArray b, jint off, jint len)
{
	CWaveInDev* dev = (CWaveInDev*)ptrWaveInDev;
	while(true)
	{
		int recvd = dev->PopDataBufToByteArray(env, b, off, len);
		switch(recvd)
		{
		case 0:
			//{
			//	char buf[8000 * 2 / 10];
			//	int size = min(len, sizeof buf);
			//	memset(buf, 0, sizeof buf);
			//	env->SetByteArrayRegion(b, off, size, (jbyte*)buf);
			//	Sleep(100);
			//	return size;
			//}
			Sleep(100);
			continue;

		case -1:
			if(dev->GetErrMsg()[0] != 0)
			{
				JniThrowException(env, dev->GetErrMsg());
			}
			return -1;

		default:
			return recvd;
		}
	}
}