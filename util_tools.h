#ifndef _INCLUDE_UTIL_TOOLS_H_
#define _INCLUDE_UTIL_TOOLS_H_
#pragma once

#include <tchar.h>
#include <regex>
#include <vector>
using namespace std;

#pragma comment(lib,"Version.lib")


#ifdef _UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string tstring;
#endif


//沪深股票匹配
static string FindShareMarketType(string astrCode)
{
	match_results<string::const_iterator> regResult;
	regex reg_sz("^(((600|601|603|900|700|730|580)[\\d]{3})|60[\\d]{4})$");
	if (regex_match(astrCode, regResult, reg_sz))
	{
		return "sh";
	}

	regex reg_sh("^(((300|000|001|002|200|080|031)[\\d]{3})|00[\\d]{4})$");
	if (regex_match(astrCode, regResult,reg_sh))
	{
		return "sz";
	}

	return "";
}


static void TencentDataChange(string astr_src, _share_rt_data_* data)
{
	string astr_des;
	int i_index = astr_src.rfind('=');
	if (i_index <= 0)
		return;

	string szType = astr_src.substr(i_index - 8, 2);
	string szCode = astr_src.substr(i_index - 6, 6);

	regex reg_sz("\"(.*)\"");
	sregex_iterator reg_itor(astr_src.begin(), astr_src.end(), reg_sz);
	sregex_iterator reg_end;
	for (; reg_itor != reg_end; reg_itor++)
	{
		astr_des = reg_itor->str(1);
	}

	vector<string> _vcr;
	char buf[1024] = { 0 };
	char* p_token = nullptr;
	char  seps[] = "~";
	char *szTemp = nullptr;

	p_token = strtok_s((char*)astr_des.c_str(), seps, &szTemp);
	while (p_token != nullptr)
	{
		p_token = strtok_s(NULL, seps, &szTemp);
		if (p_token != nullptr)
		{
			string tmp = p_token;
			_vcr.push_back(p_token);
		}
	}

	data->astrMarketType = szType;
	if (_vcr.size() > 0)
	{

		data->astrName = _vcr[0];
		data->astrCode = _vcr[1];
		data->m_fNowPrice = strtod(_vcr[2].c_str(), &szTemp);
		data->m_fLastDayPrice = strtod(_vcr[3].c_str(), &szTemp);
		data->m_fLimitUp = strtod(_vcr[45].c_str(), &szTemp);
		data->m_fLimitDown = strtod(_vcr[46].c_str(), &szTemp);

		data->m_fBuy1 = strtod(_vcr[8].c_str(), &szTemp);
		data->m_lBVolume1 = atol(_vcr[9].c_str());
		data->m_fBuy2 = strtod(_vcr[10].c_str(), &szTemp);
		data->m_lBVolume2 = atol(_vcr[11].c_str());
		data->m_fBuy3 = strtod(_vcr[12].c_str(), &szTemp);
		data->m_lBVolume3 = atol(_vcr[13].c_str());
		data->m_fBuy4 = strtod(_vcr[14].c_str(), &szTemp);
		data->m_lBVolume4 = atol(_vcr[15].c_str());
		data->m_fBuy5 = strtod(_vcr[16].c_str(), &szTemp);
		data->m_lBVolume5 = atol(_vcr[17].c_str());

		data->m_fSell1 = strtod(_vcr[18].c_str(), &szTemp);
		data->m_lSVolume1 = atol(_vcr[19].c_str());
		data->m_fSell2 = strtod(_vcr[20].c_str(), &szTemp);
		data->m_lSVolume2 = atol(_vcr[21].c_str());
		data->m_fSell3 = strtod(_vcr[22].c_str(), &szTemp);
		data->m_lSVolume3 = atol(_vcr[23].c_str());
		data->m_fSell4 = strtod(_vcr[24].c_str(), &szTemp);
		data->m_lSVolume4 = atol(_vcr[25].c_str());
		data->m_fSell5 = strtod(_vcr[26].c_str(), &szTemp);
		data->m_lSVolume5 = atol(_vcr[27].c_str());
	}
	return;
}

static void TencentDataChangeList(string astr_src, list<_share_rt_data_>& list_data)
{
	char* ptr_arry;
	char* buf_tmp = strtok_s((char*)astr_src.c_str(), ";", &ptr_arry);
	while (buf_tmp != NULL)
	{
		_share_rt_data_ data;
		string _astr_item = buf_tmp;
		buf_tmp = strtok_s(NULL, ";",&ptr_arry);
		if (_astr_item.compare("\n") != 0)
		{
			TencentDataChange(_astr_item, &data);
			list_data.push_back(data);
		}
	}
	
}


//本地文件版本号获取
static string ExeLocalProgramVersion()
{
	TCHAR FileName[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, FileName, sizeof(FileName) - 1);

	string asVer = "";
	VS_FIXEDFILEINFO *pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);

	int iVerInfoSize = GetFileVersionInfoSize(FileName, NULL);
	if (iVerInfoSize != 0) {
		char* pBuf = new char[iVerInfoSize];
		memset(pBuf, 0, iVerInfoSize);

		if (GetFileVersionInfo(FileName, 0, iVerInfoSize, pBuf)) 
		{
			if (VerQueryValue(pBuf, L"\\", (void **)&pVsInfo, &iFileInfoSize)) 
			{
				sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
				asVer = pBuf;
			}
		}
		delete pBuf;
	}
	return asVer;
}


//根据条件分解字串
static int AnalyzeStr(tstring strResource, LPCTSTR strDelimit, vector<tstring>& arrayString)
{
    arrayString.clear();

    tstring tmp;
    LPCTSTR token = _tcstok((TCHAR*)strResource.c_str(),strDelimit);
    while (token != NULL)
    {
        tmp = token;
        arrayString.push_back(tmp);
        token = _tcstok(NULL, strDelimit);
    }
    return arrayString.size();
}

#endif