#pragma once
#include <iostream>
#include <fstream>
#include "cusLib.h"
class CLog
{
private:
	static std::ofstream m_SystemLog;
	static CString		m_StatusInfo;
	enum {maxInfoLength = 0x100000,};
	static CLog*	m_instance;
public:
	CLog(){m_SystemLog.open(_T("SystemLog.txt"), std::ios_base::binary);};
	~CLog(){m_SystemLog.close();};
	static void PrintStatus(wchar_t* str);
	CLog*	GetInstance(){return m_instance;};
};

extern CLog* systemLog;

BOOL CheckError(cl_int status, wchar_t* wchar);