#ifndef __RANDOM_H__
#define __RANDOM_H__
#pragma once
#include <stdlib.h>
#include <string>

std::string GetRandomString(int ilen)
{
	std::string astrDes;
	const char buf[63] = {'1','2','3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'0' ,\
		'a' ,'b' ,'c' ,'d' ,'e' ,'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',\
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z','\0'};

	srand((unsigned int)time(NULL));
	int i = 0;
	while (i < ilen)
	{
		astrDes.append(buf + rand() % strlen(buf),1);
		i++;
	}
	return astrDes;
}

std::string GetRandomNumberINT(int ilen)
{
	std::string astrDes;
	const char buf[63] = { '1','2','3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'0'};
	srand((unsigned int)time(NULL));
	int i = 0;
	while (i < ilen)
	{
		astrDes.append(buf + rand() % strlen(buf), 1);
		i++;
	}

	return astrDes;
}
#endif	//__RANDOM_H__