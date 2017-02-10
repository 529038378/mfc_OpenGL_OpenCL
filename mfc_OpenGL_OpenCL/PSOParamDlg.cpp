// PSOParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "PSOParamDlg.h"
#include "afxdialogex.h"
#include "Log.h"

// CPSOParamDlg 对话框

IMPLEMENT_DYNAMIC(CPSOParamDlg, CDialogEx)

CPSOParamDlg::CPSOParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPSOParamDlg::IDD, pParent)
{

}

CPSOParamDlg::~CPSOParamDlg()
{
}

void CPSOParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPSOParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPSOParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()

CString CPSOParamDlg::GetEdtContent(int idc, wchar_t* errInfo)
{
	auto edt = dynamic_cast<CEdit*>(GetDlgItem(idc));
	wchar_t* content = new wchar_t[edt->LineLength()];
	int len = edt->GetLine(0, content, edt->LineLength());
	if (len <= 0)
	{
		CString csErrInfo(errInfo);
		csErrInfo = _T("Error:") + csErrInfo;
		systemLog->PrintStatus(csErrInfo.GetBuffer());
		return CString(_T(""));
	}

	return CString(content, len);
}

void CPSOParamDlg::SetOpenCLComPtr(COpenCLCompute* ptr)
{
	m_OpenCLCom = ptr;
}
// CPSOParamDlg 消息处理程序


void CPSOParamDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString paticleNum = GetEdtContent(IDC_PATICLENUM_EDIT, _T("粒子数参数设置"));
	CString sampleConut = GetEdtContent(IDC_SAMPLECOUNT_EDIT, _T("采样次数设置"));
	CString inertiaWeight = GetEdtContent(IDC_INERTIAWEIGHT_EDIT, _T("惯性权重设置"));
	CString c1Weight = GetEdtContent(IDC_C1WEIGHT_EDIT, _T("c1权重设置"));
	CString c2Weight = GetEdtContent(IDC_C2WEIGHT_EDIT, _T("c2权重设置"));
	CString singleMaxShift = GetEdtContent(IDC_SINGLEMAXSHIFT_EDIT, _T("单次位移最大值设置"));

	if ( paticleNum == CString(_T("")) | sampleConut == CString(_T("")) | inertiaWeight == CString(_T("")) | c1Weight == CString(_T("")) | c2Weight == CString(_T("")) | singleMaxShift == CString(_T(""))) return;

	m_OpenCLCom->SetPSOSAHParam(StrToInt(paticleNum.GetBuffer()),
								StrToInt(sampleConut.GetBuffer()),
								StrToFloat(CStrToStr(inertiaWeight)),
								StrToFloat(CStrToStr(c1Weight)),
								StrToFloat(CStrToStr(c2Weight)),
								StrToFloat(CStrToStr(singleMaxShift)));
	CDialogEx::OnOK();
}
