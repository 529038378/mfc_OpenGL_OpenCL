#pragma once

#include "Render.h"
// CRenderDlg �Ի���

class CRenderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRenderDlg)

public:
	CRenderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRenderDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
