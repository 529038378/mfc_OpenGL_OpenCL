// RenderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "RenderDlg.h"
#include "afxdialogex.h"

CRenderDlg* CRenderDlg::m_instance = NULL;
CRender*	CRenderDlg::m_Render = NULL;
// CRenderDlg 对话框

IMPLEMENT_DYNAMIC(CRenderDlg, CDialogEx)

CRenderDlg::CRenderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRenderDlg::IDD, pParent)
{
	m_instance = this;
	if (!m_Render)
	{
		m_Render = new CRender;
	}
}

CRenderDlg::~CRenderDlg()
{
}

void CRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_RENDERDISPLAY_BUTTON, m_RenderDisplayBtn);
}


BEGIN_MESSAGE_MAP(CRenderDlg, CDialogEx)
	//ON_BN_CLICKED(IDC_RENDERDISPLAY_BUTTON, &CRenderDlg::OnBnClickedRenderDisplayButton)
END_MESSAGE_MAP()


// CRenderDlg 消息处理程序


BOOL CRenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//渲染窗口位置设置
	::SetWindowPos(this->m_hWnd, HWND_BOTTOM, (GetSystemMetrics(SM_CXSCREEN) - ciRenderWinWidth)/2, (GetSystemMetrics(SM_CYSCREEN) - ciRenderWinHeight)/2, ciRenderWinWidth, ciRenderWinHeight, SWP_NOZORDER);  
		
	//::SetWindowPos(this->m_hWnd, HWND, (cx - ciRenderWinWidth)/2, (cy- ciRenderWinHeight)/2, (cx+ciRenderWinWidth)/2, (cy+ciRenderWinHeight)/2, SWP_NOZORDER);
	//设置渲染区域
	CRect dlgRect;
	GetClientRect(&dlgRect);
	
	m_Render->Create(NULL, NULL, WS_CHILDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, dlgRect, this, 0);

	static int flag = 1;
	//if (flag--) CDialogEx::OnOK();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CRender* CRenderDlg::GetRenderObj()
{
	return m_Render;
}

CRenderDlg* CRenderDlg::GetInstance()
{
	return m_instance;
}

HWND CRenderDlg::GetHandle()
{
	return GetSafeHwnd();
}

//void CRenderDlg::Display()
//{
//	HWND hwnd = this->m_hWnd;
//	Invalidate(TRUE);
//	UpdateWindow();
//	DoModal();
//}

void CRenderDlg::OnBnClickedRenderDisplayButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//HWND hwnd = m_Render->GetSafeHwnd();
}
