#ifndef __INCLUDE_STRINGTOOLS_H
#define __INCLUDE_STRINGTOOLS_H

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

#ifdef _UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string tstring;
#endif

#define  sizeofstr(s)  sizeof(s)/sizeof(TCHAR)
static BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode 
	// string. 
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);

	if(dwSize < dwMinSize)
	{
		return FALSE;
	}


	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
	return TRUE;
}


static BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
	return TRUE;
}

static wstring MBTOW(const string & astr)
{
	wstring lRet;
	int liLen = astr.size() * 2 + 10;
	WCHAR * buff = new WCHAR[liLen];
	if (buff == 0)
		return lRet;

	memset(buff, 0, liLen * sizeof(TCHAR));
	MByteToWChar(astr.c_str(), buff, liLen - 1);
	lRet  = buff;
	delete [] buff;
	return lRet;
}
static string WTOMB(const wstring & astr)
{
	string lRet;
	int liLen = astr.size() * 2 + 10;
	char * buff = new char[liLen];
	if (buff == 0)
		return "";
	memset(buff, 0, liLen);
	WCharToMByte(astr.c_str(), buff, liLen - 1);
	lRet  = buff;
	delete [] buff;
	return lRet;
}

static string WTOUTF8(const wstring & strUnicode)
{
	//MessageBox(strUnicode);
	int ilen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	char *szUtf8Temp=new char[ilen + 1];
	if (szUtf8Temp == 0)
		return "";

	memset(szUtf8Temp, 0, ilen +1);
	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)strUnicode.c_str(), -1, szUtf8Temp, ilen, NULL,NULL);

	string lstrRet = szUtf8Temp;
	delete [] szUtf8Temp;
	return lstrRet;
}
static wstring UTF8TOW(const string & astrUTF8)
{
	DWORD dwUnicodeLen;        //ת����Unicode�ĳ���
	WCHAR *pwText;            //����Unicode��ָ��
	wstring strUnicode;        //����ֵ

	//���ת����ĳ��ȣ��������ڴ�
	dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,astrUTF8.c_str(),-1,NULL,0);
	pwText = new WCHAR[dwUnicodeLen];
	if (pwText == 0)
		return strUnicode;

	if (!pwText)
	{
		return strUnicode;
	}
	//תΪUnicode
	MultiByteToWideChar(CP_UTF8,0,astrUTF8.c_str(),-1,pwText,dwUnicodeLen);

	strUnicode = pwText;
	//����ڴ�
	delete []pwText;
	//����ת���õ�Unicode�ִ�
	return strUnicode;
}

// ���ֽڱ���תΪUTF8���� 
static string MBTOUTF8(const string & astr) 
{ 
	return WTOUTF8(MBTOW(astr));
} 

// UTF8����תΪ���ֽڱ��� 
static string UTF8TOMB(const string & astr) 
{ 
	return WTOMB(UTF8TOW(astr));
} 

static int MBTOWCHAR(wchar_t * awCharBuff, const string & astr, unsigned int aiBuffLen)
{
	if (awCharBuff == NULL)
		return 0;
	wstring lstrTmp = MBTOW(astr);
	int liRet = lstrTmp.size();
	wcsncpy(awCharBuff, lstrTmp.c_str(), aiBuffLen);
	return liRet;
}
static int UTF8TOWCHAR(wchar_t * awCharBuff, const string & astr, unsigned int aiBuffLen)
{
	if (awCharBuff == NULL)
		return 0;
	wstring lstrTmp = UTF8TOW(astr);
	int liRet = lstrTmp.size();
	wcsncpy(awCharBuff, lstrTmp.c_str(), aiBuffLen);
	return liRet;
}

static void ReplaceSubString(string& str, const string& substr, const string& newVal)
{
	string::size_type   pos(0);

	pos = str.find(substr);
	if(pos != string::npos)     
		str.replace(pos, substr.length(), newVal); 
}

static std::string formatstring(const char * szFormat, ...)
{
#define FORMATTSTRINGMAC_MAX 2048
	char _buffer[FORMATTSTRINGMAC_MAX + 1] = {0};
	va_list	argList;
	va_start(argList, szFormat);
	int	ret	= _vsnprintf(_buffer, FORMATTSTRINGMAC_MAX, szFormat, argList);
	va_end(argList);
	return _buffer;
}	

static wstring formatwstring(const WCHAR * szFormat, ...)
{
#define FORMATTSTRINGMAC_MAX 2048
	WCHAR _buffer[FORMATTSTRINGMAC_MAX + 1] = {0};
	va_list	argList;
	va_start(argList, szFormat);
	int	ret	= _vsnwprintf(_buffer, FORMATTSTRINGMAC_MAX, szFormat, argList);
	va_end(argList);
	return _buffer;
}	


typedef basic_string<char>::size_type S_T;      
static const S_T npos = -1;      

////trimָʾ�Ƿ����մ���Ĭ��Ϊ������      
static int tokenize(const string& src, string tok, vector<string>& aResult, bool trim=false, string null_subst="")      
{      
	if( src.empty() || tok.empty() )    
		return 0;

	S_T pre_index = 0, index = 0, len = 0;      
	while( (index = src.find_first_of(tok, pre_index)) != npos )      
	{      
		if( (len = index-pre_index)!=0 )      
			aResult.push_back(src.substr(pre_index, len));      
		else if(trim==false)      
			aResult.push_back(null_subst);      
		pre_index = index+1;      
	}      
	string endstr = src.substr(pre_index);      
	if( trim==false )    
		aResult.push_back( endstr.empty()? null_subst:endstr );      
	else if( !endstr.empty() )    
		aResult.push_back(endstr);      
	return aResult.size();      
}   

static int split(const string& src, string delimit, vector<string>& aResult, string null_subst="")      
{      
	if( src.empty() || delimit.empty() )    
		return 0;

	S_T deli_len = delimit.size();      
	long index = npos, last_search_position = 0;      
	while( (index=src.find(delimit, last_search_position))!=npos )      
	{      
		if(index==last_search_position)      
			aResult.push_back(null_subst);      
		else     
			aResult.push_back( src.substr(last_search_position, index - last_search_position) );      
		last_search_position = index + deli_len;      
	}     

	string last_one = src.substr(last_search_position);      
	aResult.push_back( last_one.empty()? null_subst:last_one );      
	return aResult.size();      
}    
static int splitw(const wstring& src, wstring delimit, vector<wstring>& aResult, wstring null_subst=L"")      
{      
	if( src.empty() || delimit.empty() )    
		return 0;

	S_T deli_len = delimit.size();      
	long index = npos, last_search_position = 0;      
	while( (index=src.find(delimit, last_search_position))!=npos )      
	{      
		if(index==last_search_position)      
			aResult.push_back(null_subst);      
		else     
			aResult.push_back( src.substr(last_search_position, index - last_search_position) );      
		last_search_position = index + deli_len;      
	}     

	wstring last_one = src.substr(last_search_position);      
	aResult.push_back( last_one.empty()? null_subst:last_one );      
	return aResult.size();      
}  
/************************
˵���� �ַ����Ƚ� �����ִ�Сд ��ת����Сд��
���룺	string astr1 
		string astr2
*************************/
static bool StringCompareNoCase(string astr1,string astr2)
{
	transform(astr1.begin(),astr1.end(),astr1.begin(),tolower);
	transform(astr2.begin(),astr2.end(),astr2.begin(),tolower);
	return (astr1 == astr2);
}
static string DelSpecialChar(string astrScr,string astrSpecialCharBuff)
{
	string lstrRet;
	for (unsigned int i = 0;i < astrScr.size();++i)
	{
		bool lbSpecialChar = false;
		for (unsigned int j = 0;j < astrSpecialCharBuff.size();++j)
		{
			if (astrScr[i] == astrSpecialCharBuff[j])
			{
				lbSpecialChar = true;
				break;
			}
		}
		if (!lbSpecialChar)
		{
			lstrRet += astrScr[i];
		}
	}
	return lstrRet;
}
//�ļ����ƹ���
static string FileNameFilter(string astrScr)
{
	string lstrRet;
	for (unsigned int i = 0;i < astrScr.size();++i)
	{
		char lcChar = astrScr[i];
		bool lbSpecialChar = false;

		if (lcChar>=0&&lcChar<32)
		{
			lbSpecialChar = true;
		}
		if (lcChar>126)
		{
			lbSpecialChar = true;
		}
		if (lcChar == '\\')
		{
			lbSpecialChar = true;
		}
		if (lcChar == '/')
		{
			lbSpecialChar = true;
		}
		if (lcChar == ':')
		{
			lbSpecialChar = true;
		}
		if (lcChar == '*')
		{
			lbSpecialChar = true;
		}
		if (lcChar == '?')
		{
			lbSpecialChar = true;
		}
		if (lcChar == '\"')
		{
			lbSpecialChar = true;
		}
		if (lcChar == '<')
		{
			lbSpecialChar = true;
		}
		if (lcChar == '>')
		{
			lbSpecialChar = true;
		}
		if (lcChar == '|')
		{
			lbSpecialChar = true;
		}
		if (lcChar == ' ')
		{
			lbSpecialChar = true;
		}
		if (!lbSpecialChar)
		{
			lstrRet += astrScr[i];
		}
	}
	return lstrRet;
}

#endif//__INCLUDE_STRINGTOOLS_H