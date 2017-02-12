#pragma once

#include "cusLib.h"
#include "PrallalCompute.h"
#include "afxwin.h"
// CSAAParamDlg 对话框

class CSAAParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSAAParamDlg)

public:
	CSAAParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSAAParamDlg();

// 对话框数据
	enum { IDD = IDD_SAA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString GetEdtContent(CEdit* edt, wchar_t* errInfo);
	void	SetOpenCLComPtr(COpenCLCompute* ptr);

private:
	COpenCLCompute* m_OpenCLCom;
public:
	afx_msg void OnBnClickedSaaenterButton();
	CEdit m_OriginTmpEdt;
	CEdit m_ThresholdTmpEdt;
	CEdit m_SingleSampleCount;
	CEdit m_DecSpeed;
};
