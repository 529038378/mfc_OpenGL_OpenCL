#pragma once

#include "Render.h"
#include "afxwin.h"
// CRenderDlg �Ի���

class CRenderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRenderDlg)

public:
	CRenderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRenderDlg();

// �Ի�������
	enum { IDD = IDD_RENDER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
