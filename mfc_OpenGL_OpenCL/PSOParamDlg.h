#pragma once

#include "cusLib.h"
#include "PrallalCompute.h"
#include "afxwin.h"

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
	CString GetEdtContent(CEdit* edt, wchar_t* errInfo);
	void SetOpenCLComPtr(COpenCLCompute* ptr);
private:
	COpenCLCompute* m_OpenCLCom;
public:
	afx_msg void OnBnClickedOk();
	CEdit m_ParticlNumEdt;
	CEdit m_SampleCountEdt;
	CEdit m_IertiaWeightEdt;
	CEdit m_C1WeightEdt;
	CEdit m_C2WeigthEdt;
	CEdit m_SingleMaxShiftEdt;
};
