
// mfc_OpenGL_OpenCLDlg.h : 头文件
//

#pragma once
#include "Render.h"
#include "afxcmn.h"

// Cmfc_OpenGL_OpenCLDlg 对话框
class Cmfc_OpenGL_OpenCLDlg : public CDialogEx
{
// 构造
public:
	Cmfc_OpenGL_OpenCLDlg(CWnd* pParent = NULL);	// 标准构造函数
	~Cmfc_OpenGL_OpenCLDlg();
// 对话框数据
	enum { IDD = IDD_MFC_OPENGL_OPENCL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CRender* m_Render;
	enum {PRINT_INFO  = 0x00001000,};
	HTREEITEM m_root;
	std::vector<HTREEITEM> m_hardwarePlatformNode;
	std::vector<HTREEITEM> m_harewareDeviceNode;
	HardwareInfo* m_hardwareInfo;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CTreeCtrl m_hardwareInfoTreeCtl;
	CToolTipCtrl m_toolTip;
	BOOL PreTranslateMessage(MSG* pMsg);
};
