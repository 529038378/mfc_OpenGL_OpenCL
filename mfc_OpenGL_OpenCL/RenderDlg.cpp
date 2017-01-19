// RenderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "RenderDlg.h"
#include "afxdialogex.h"

CRenderDlg* CRenderDlg::m_instance = NULL;
// CRenderDlg �Ի���

IMPLEMENT_DYNAMIC(CRenderDlg, CDialogEx)

CRenderDlg::CRenderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRenderDlg::IDD, pParent)
{
	m_instance = this;
}

CRenderDlg::~CRenderDlg()
{
}

void CRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRenderDlg, CDialogEx)
END_MESSAGE_MAP()


// CRenderDlg ��Ϣ�������


BOOL CRenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect dlgRect;
	GetClientRect(&dlgRect);
	m_Render = new CRender;
	m_Render->Create(NULL, NULL, WS_CHILDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, dlgRect, this, 0);

	//SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_HIDEWINDOW); 
	//ModifyStyle(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	static int flag = 1;
	if (flag--) CDialogEx::OnOK();
	//CDialogEx::ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

CRender* CRenderDlg::GetRenderObj()
{
	return m_Render;
}

CRenderDlg* CRenderDlg::GetInstance()
{
	return m_instance;
}

