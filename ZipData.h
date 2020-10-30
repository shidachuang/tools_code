#ifndef _ZIP_DATA_H_
#define _ZIP_DATA_H_
#pragma once

extern "C"
{
	#include "../../Include/zlib/zlib.h"
}


#ifdef _DEBUG
	#pragma comment(lib,"../../lib/debug/zlibd.lib")
#else
	#pragma comment(lib,"../../lib/release/zlib.lib")
#endif

/*
	ZIP数据解压缩
	[in]		bf_src				源字符流
	[in]		sz_srclen			源字符流长度
	[in_out]	sz_unziplen			解压后字符流(必须输入)
	[return]	解压字节流指针

	注意,该函数将分配内存,失败后自行释放;如果解压成功需要自行释放
*/

static char* UnZipData(_In_ char* bf_src, _In_ unsigned long sz_srclen, _Inout_ size_t& sz_unziplen)
{
    int iErr = 0;
	Bytef* unZipBuf = NULL;
	uLong unTmpLen = 0;

	if (bf_src == NULL || sz_srclen <= 0)
		assert(0);

	if (sz_unziplen <= 0)
		assert(0);

	unZipBuf = new Bytef[sz_unziplen + 1];
	memset(unZipBuf, 0, sz_unziplen + 1);
	unTmpLen = sz_unziplen;

    iErr = uncompress(unZipBuf, &unTmpLen, (Bytef*)bf_src, (uLong)sz_srclen);
	if (iErr == Z_OK && unTmpLen == sz_unziplen)
	{
		sz_unziplen = unTmpLen;
	}
	else
	{
		sz_unziplen = 0;
        delete [] unZipBuf;
		unZipBuf = NULL;

        UILog::d("UZIP", "uzip buffer code %d", iErr);
	}

	return (char*)unZipBuf;
}


#endif
