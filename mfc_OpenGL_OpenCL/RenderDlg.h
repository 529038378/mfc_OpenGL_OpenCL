#pragma once

#include "Render.h"
// CRenderDlg 对话框

class CRenderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRenderDlg)

public:
	CRenderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRenderDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	static CRenderDlg*	m_instance;
	CRender*			m_Render;
public:
	CRenderDlg* GetInstance();
	CRender*	GetRenderObj();
};
