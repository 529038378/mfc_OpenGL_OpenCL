#pragma once

#include "Render.h"
#include "afxwin.h"
// CRenderDlg 对话框

class CRenderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRenderDlg)

public:
	CRenderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRenderDlg();

// 对话框数据
	enum { IDD = IDD_RENDER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	static CRenderDlg*	m_instance;
	static CRender*			m_Render;
public:
	CRenderDlg* GetInstance();
	CRender*	GetRenderObj();
	HWND		GetHandle();
	//void		Display();
//	CButton m_RenderDisplayBtn;
	afx_msg void OnBnClickedRenderDisplayButton();
};
