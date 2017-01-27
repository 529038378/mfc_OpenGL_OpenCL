#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"

#include "Log.h"

std::ofstream CLog::m_SystemLog("SystemLog.txt", std::ios_base::binary);
CString		CLog::m_StatusInfo = _T("");

void CLog::PrintStatus(wchar_t* str)
{
	ASSERT(m_SystemLog.is_open());
	if (m_StatusInfo.GetLength()>maxInfoLength) m_StatusInfo = CString(_T(""));
	if(m_StatusInfo.GetLength())m_StatusInfo = m_StatusInfo + "\r\n" + CString(str);
	else m_StatusInfo = CString(str);
	SetDlgItemText(AfxGetMainWnd()->m_hWnd ,IDC_RUNTIMESTATUS_EDIT, m_StatusInfo);
	CEdit* edit = dynamic_cast<CEdit*>(AfxGetMainWnd()->GetDlgItem(IDC_RUNTIMESTATUS_EDIT));
	edit->LineScroll(edit->GetLineCount());
	m_SystemLog.clear();
	m_SystemLog<<CStrToStr(CString(str))<<"\r\n"<<std::endl;
}

CLog* systemLog = new CLog;

BOOL CheckError(cl_int status, wchar_t* wchr)
{
	CString str(wchr);
	if ( status != CL_SUCCESS)
	{
		str += _T("--Ê§°Ü£¡£¡£¡");
		str = _T("Error:	") + str;
		systemLog->PrintStatus(str.GetBuffer());
		return FALSE;
	}
	else
	{
		str += _T("--³É¹¦£¡");
		systemLog->PrintStatus(str.GetBuffer());
		return TRUE;
	}
}