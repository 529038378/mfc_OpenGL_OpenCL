#include "cusLib.h"

CString statusInfo;



std::string CStrToStr(CString& str)
{
	wchar_t* wText = str.GetBuffer();
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);
	char *psText;
	psText = new char[dwNum];
	if(!psText)
	{
		delete []psText;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);
	return std::string(psText);
}

CString	StrToCStr(std::string str)
{
	std::wstringstream cs;
	cs<<str.c_str();
	return CString(cs.str().c_str());
}

float StrToFloat(std::string& str)
{
	float res;
	std::stringstream strm(str.c_str());
	strm.setf(std::ios::fixed);
	strm>>res;
	return res;
}

