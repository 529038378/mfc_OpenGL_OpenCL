
// mfc_OpenGL_OpenCLDlg.h : 头文件
//

#pragma once
#include "Render.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "RenderDlg.h"

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
	static Cmfc_OpenGL_OpenCLDlg* m_instance;
	CRender*	m_Render;
	enum	{PRINT_INFO  = 0x00001000,};
	HTREEITEM	m_root;
	std::vector<HTREEITEM>	m_hardwarePlatformNode;
	std::vector<HTREEITEM>	m_harewareDeviceNode;
	HardwareInfo*	m_HardwareInfo;
	std::string		m_ModelName;
	int				m_SelPlatformIndex;
	int				m_SelDeviceIndex;
	int				m_SelDimNum;
	int				m_SelRadio;
	static CRenderDlg*		m_RenderDlg;
	std::vector<int> m_WorkGlobalGroup;
	std::vector<int> m_WorkLocalGroup;
public:
	CEdit*	GetStatusEdt();
	CString	GetEdtContent(CEdit* edit, wchar_t* info);
	CRenderDlg* GetRenderDlg();
	BOOL	CheckDimEdt(CEdit* edt, wchar_t* dimName, int index, bool flag = true);
public:
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	CTreeCtrl	m_hardwareInfoTreeCtl;
	CToolTipCtrl	m_toolTip;
	CButton	m_LoadModelBtn;
	CEdit	m_ModelNameEdt;
	CEdit	m_StatusEdt;	
	BOOL	PreTranslateMessage(MSG* pMsg);	
	afx_msg void	OnBnClickedLoadmodelButton();
//	afx_msg void OnBnClickedGemodelinfoButton();
	afx_msg void OnBnClickedGetModeLinfoButton();
	afx_msg void OnBnClickedInitialContextButton();
	CComboBox m_PlatformComBox;
	CComboBox m_DeviceComBox;
	afx_msg void OnCbnSelchangePlatformsCombo();
	afx_msg void OnCbnSelchangeDevicesCombo();
	
//	CComboBox m_DimChoose;
	CComboBox m_DimChooseComBox;
	CEdit m_DimGXEdt;
	CEdit m_DimGYEdt;
	CEdit m_DimGZEdt;
	afx_msg void OnCbnSelchangeDimChooseCombo();
	afx_msg void OnBnClickedConfigConfirmButton();
	afx_msg void OnBnClickedCommonSplitRadio();
	afx_msg void OnBnClickedSAASAHSplitRadio();
	afx_msg void OnBnClickedPSOSAHSplitRadio();
	CEdit m_ReflectCount;
	CEdit m_ViewXEdt;
//	CEdit m_ViewY;
	CEdit m_ViewYEdt;
	CEdit m_ViewZEdt;
	CEdit m_LightXEdt;
	CEdit m_LightYEdt;
	CEdit m_LightZEdt;
	afx_msg void OnBnClickedOffLineRenderingButton();
	afx_msg void OnBnClickedRealtimeRenderingButton();
	CEdit m_DimLXEdt;
	CEdit m_DimLYEdt;
	CEdit m_DimLZEdt;
};
