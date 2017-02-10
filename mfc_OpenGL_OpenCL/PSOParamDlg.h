#pragma once

#include "cusLib.h"
#include "PrallalCompute.h"

// CPSOParamDlg �Ի���

class CPSOParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPSOParamDlg)

public:
	CPSOParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPSOParamDlg();

// �Ի�������
	enum { IDD = IDD_PSO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString GetEdtContent(int idc, wchar_t* errInfo);
	void SetOpenCLComPtr(COpenCLCompute* ptr);
private:
	COpenCLCompute* m_OpenCLCom;
public:
	afx_msg void OnBnClickedOk();
};
