
// mfc_OpenGL_OpenCLDlg.cpp : 实现文件
//

#pragma once

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "mfc_OpenGL_OpenCLDlg.h"
#include "afxdialogex.h"
#include "Log.h"
#include <sstream>
#include <algorithm>

#include "cusLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

Cmfc_OpenGL_OpenCLDlg* Cmfc_OpenGL_OpenCLDlg::m_instance = NULL;



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cmfc_OpenGL_OpenCLDlg 对话框

CRenderDlg* Cmfc_OpenGL_OpenCLDlg::m_RenderDlg = NULL;


Cmfc_OpenGL_OpenCLDlg::Cmfc_OpenGL_OpenCLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmfc_OpenGL_OpenCLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Render = NULL;
}

Cmfc_OpenGL_OpenCLDlg::~Cmfc_OpenGL_OpenCLDlg()
{
	if (m_Render)
	{
		delete m_Render;
		m_Render = NULL;
	}


	
}
void Cmfc_OpenGL_OpenCLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HARDWAREINFO_Tree, m_hardwareInfoTreeCtl);
	DDX_Control(pDX, IDC_LOADMODEL_BUTTON, m_LoadModelBtn);
	DDX_Control(pDX, IDC_MODELNAME_EDIT, m_ModelNameEdt);
	DDX_Control(pDX, IDC_PLATFORMS_COMBO, m_PlatformComBox);
	DDX_Control(pDX, IDC_DEVICES_COMBO, m_DeviceComBox);
	DDX_Control(pDX, IDC_RUNTIMESTATUS_EDIT, m_StatusEdt);
	//  DDX_Control(pDX, IDC_COMBO1, m_DimChoose);
	DDX_Control(pDX, IDC_DIMCHOOSE_COMBO, m_DimChooseComBox);
	DDX_Control(pDX, IDC_DIMGX_EDIT, m_DimGXEdt);
	DDX_Control(pDX, IDC_DIMGY_EDIT, m_DimGYEdt);
	DDX_Control(pDX, IDC_DIMGZ_EDIT, m_DimGZEdt);
	DDX_Control(pDX, IDC_REFLECTCOUNT_EDIT, m_ReflectCount);
	DDX_Control(pDX, IDC_VIEWX_EDIT, m_ViewXEdt);
	//  DDX_Control(pDX, IDC_VIEWY_EDIT, m_ViewY);
	DDX_Control(pDX, IDC_VIEWY_EDIT, m_ViewYEdt);
	DDX_Control(pDX, IDC_VIEWZ_EDIT, m_ViewZEdt);
	DDX_Control(pDX, IDC_LIGHTX_EDIT, m_LightXEdt);
	DDX_Control(pDX, IDC_LIGHTY_EDIT, m_LightYEdt);
	DDX_Control(pDX, IDC_LIGHTZ_EDIT, m_LightZEdt);
	DDX_Control(pDX, IDC_DIMLX_EDIT, m_DimLXEdt);
	DDX_Control(pDX, IDC_DIMLY_EDIT2, m_DimLYEdt);
	DDX_Control(pDX, IDC_DIMLZ_EDIT2, m_DimLZEdt);
}

BEGIN_MESSAGE_MAP(Cmfc_OpenGL_OpenCLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_LOADMODEL_BUTTON, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedLoadmodelButton)
	ON_BN_CLICKED(IDC_GETMODELINFO_BUTTON, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedGetModeLinfoButton)
	ON_BN_CLICKED(IDC_INITIALCONTEXT_BUTTON, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedInitialContextButton)
	ON_CBN_SELCHANGE(IDC_PLATFORMS_COMBO, &Cmfc_OpenGL_OpenCLDlg::OnCbnSelchangePlatformsCombo)
	ON_CBN_SELCHANGE(IDC_DEVICES_COMBO, &Cmfc_OpenGL_OpenCLDlg::OnCbnSelchangeDevicesCombo)
	ON_CBN_SELCHANGE(IDC_DIMCHOOSE_COMBO, &Cmfc_OpenGL_OpenCLDlg::OnCbnSelchangeDimChooseCombo)
	ON_BN_CLICKED(IDC_CONFIGCONFIRM_BUTTON, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedConfigConfirmButton)
	ON_BN_CLICKED(IDC_COMMONSPLIT_RADIO, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedCommonSplitRadio)
	ON_BN_CLICKED(IDC_SAASAHSPLIT_RADIO, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedSAASAHSplitRadio)
	ON_BN_CLICKED(IDC_PSOSAHSPLIT_RADIO, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedPSOSAHSplitRadio)
	ON_BN_CLICKED(IDC_OFFLINERENDERING_BUTTON, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedOffLineRenderingButton)
	ON_BN_CLICKED(IDC_REALTIMERENDERING_BUTTON, &Cmfc_OpenGL_OpenCLDlg::OnBnClickedRealtimeRenderingButton)
END_MESSAGE_MAP()



// Cmfc_OpenGL_OpenCLDlg 消息处理程序

BOOL Cmfc_OpenGL_OpenCLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//CRender类对象初始化
	//CRect dlgRect;
	//GetClientRect(&dlgRect);
	//CRect rect(15, dlgRect.Height()/2 - 5, dlgRect.Width() - 15, dlgRect.Height()-5);
	/*CRect rect(0, 100, 200, 300);
	m_Render = new CRender;
	m_Render->Create(NULL, NULL, WS_CHILDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, rect, this, 0);*/
	if (!m_RenderDlg)
	{
		m_RenderDlg = new CRenderDlg;
	}
	m_RenderDlg->GetInstance()->DoModal();
	m_Render = m_RenderDlg->GetInstance()->GetRenderObj();

	//硬件信息显示CTreeCtrl和硬件选择ComboBox相关
	ASSERT(m_Render);
	m_HardwareInfo = m_Render->GetCompObj()->GetHardwareInfo();
	ASSERT(m_HardwareInfo);
	
	m_hardwarePlatformNode.resize(m_HardwareInfo->platformIDs.size());
	m_root = m_hardwareInfoTreeCtl.InsertItem(_T("本机可供GPGPU硬件信息"), 0, 0);

	static int deviceInfoIndex = 0;
	for (auto platformItr = m_HardwareInfo->platformInfo.begin(); platformItr != m_HardwareInfo->platformInfo.end(); platformItr++)
	{
		std::string platformInfo("");
		//platformInfo = platformItr->name.substr(0, platformItr->name.length()-1) + " ( Vendor Name:" + platformItr->venderName.substr(0, platformItr->venderName.length()-1) + " device num: " + TtoStr(platformItr->deviceNum) +")";
		platformInfo = platformItr->name;

		std::wstringstream wsPlatformInfo;
		wsPlatformInfo<<platformInfo.c_str();
		int platformIndex = platformItr - m_HardwareInfo->platformInfo.begin();
		m_hardwarePlatformNode[platformIndex] = m_hardwareInfoTreeCtl.InsertItem(wsPlatformInfo.str().c_str(), m_root, TVI_LAST);
		m_hardwareInfoTreeCtl.SetItemData(m_hardwarePlatformNode[platformIndex], platformIndex);

		//平台选择ComboBox相关
		m_PlatformComBox.InsertString(platformIndex, StrToCStr(platformItr->name));

		m_harewareDeviceNode.resize(m_harewareDeviceNode.size()+platformItr->deviceInfo.size());
		m_HardwareInfo->deviceInfo.resize(m_HardwareInfo->deviceInfo.size() + platformItr->deviceInfo.size());
		for (auto deviceItr = platformItr->deviceInfo.begin(); deviceItr != platformItr->deviceInfo.end(); deviceItr++, deviceInfoIndex++)
		{
			int deviceIndex = deviceItr - platformItr->deviceInfo.begin();
			std::wstringstream wsDeviceInfo;
			wsDeviceInfo<<deviceItr->name.c_str();
			HTREEITEM LeafNodeItem = m_hardwareInfoTreeCtl.InsertItem(wsDeviceInfo.str().c_str(), m_hardwarePlatformNode[platformIndex], TVI_LAST);
			m_harewareDeviceNode[deviceInfoIndex]=LeafNodeItem;
			m_hardwareInfoTreeCtl.SetItemData(LeafNodeItem, deviceIndex);
			m_HardwareInfo->deviceInfo[deviceInfoIndex] = *deviceItr;
		}

	}
	m_toolTip.CreateEx(this, TTS_ALWAYSTIP);
	CRect treeClientRect;
	m_hardwareInfoTreeCtl.GetClientRect(&treeClientRect);
	m_toolTip.AddTool(GetDlgItem(IDC_HARDWAREINFO_Tree), _T("本机硬件信息"), &treeClientRect, IDC_HARDWAREINFO_Tree);

	//参数设置区相关
	m_SelDimNum = 0;
	m_DimGXEdt.EnableWindow(FALSE);
	m_DimGYEdt.EnableWindow(FALSE);
	m_DimGZEdt.EnableWindow(FALSE);
	m_DimLXEdt.EnableWindow(FALSE);
	m_DimLYEdt.EnableWindow(FALSE);
	m_DimLZEdt.EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_COMMONSPLIT_RADIO))->SetCheck(TRUE);
	m_SelRadio = SAHSPLIT_COMM;
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cmfc_OpenGL_OpenCLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmfc_OpenGL_OpenCLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cmfc_OpenGL_OpenCLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int Cmfc_OpenGL_OpenCLDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

BOOL Cmfc_OpenGL_OpenCLDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_MOUSEMOVE)
	{
		CPoint pt = pMsg->pt;
		m_hardwareInfoTreeCtl.ScreenToClient(&pt);
		UINT uFlags;
		HTREEITEM item;
		item = m_hardwareInfoTreeCtl.HitTest(pt, &uFlags);
		if ( uFlags & TVHT_ONITEMLABEL)
		{
			if (item == m_root)
			{
				m_toolTip.UpdateTipText(_T("本机硬件信息"), GetDlgItem(IDC_HARDWAREINFO_Tree), IDC_HARDWAREINFO_Tree);
				m_toolTip.RelayEvent(pMsg);
			}
			else
			{
				auto itrPlatform = std::find(m_hardwarePlatformNode.begin(), m_hardwarePlatformNode.end(), item);
				if (itrPlatform != m_hardwarePlatformNode.end())
				{
					std::string tipInfo("");
					int platformIndex = itrPlatform - m_hardwarePlatformNode.begin();
					tipInfo = "VenderName: " + m_HardwareInfo->platformInfo[platformIndex].venderName.substr(0, m_HardwareInfo->platformInfo[platformIndex].venderName.length()-1) + ",	 Device Num: " + TToStr(m_HardwareInfo->platformInfo[platformIndex].deviceNum) + ".";

					std::wstringstream wTipInfo;
					wTipInfo<<tipInfo.c_str();
					m_toolTip.UpdateTipText(wTipInfo.str().c_str(), GetDlgItem(IDC_HARDWAREINFO_Tree), IDC_HARDWAREINFO_Tree);
					m_toolTip.RelayEvent(pMsg);
				}
				auto itrDevice = std::find(m_harewareDeviceNode.begin(), m_harewareDeviceNode.end(), item);
				if (itrDevice != m_harewareDeviceNode.end())
				{
					std::string tipInfo("");
					int deviceIndex = itrDevice - m_harewareDeviceNode.begin();
					std::string workItemSize("");
					for (auto itr = m_HardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize.begin(); itr != m_HardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize.end(); itr++)
					{
						int tmp = m_HardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize[0];
						tmp = m_HardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize[1];
						tmp = m_HardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize[2];
						workItemSize += TToStr(*itr) + ",";
					}
					workItemSize = "(" + workItemSize.substr(0, workItemSize.length()-1) +")";
					tipInfo = "Type: " + TToStr(m_HardwareInfo->deviceInfo[deviceIndex].type) 
						+",	Max Clock Freq: " + TToStr(m_HardwareInfo->deviceInfo[deviceIndex].maxClockFreq) 
						+ ",	BitWidth: " + TToStr(m_HardwareInfo->deviceInfo[deviceIndex].bitWidth) 
						+ ",	Global Mem Size: " + TToStr(m_HardwareInfo->deviceInfo[deviceIndex].globalMemSize)
						+ ",	Local Mem Size: " + TToStr(m_HardwareInfo->deviceInfo[deviceIndex].localMemSize)
						+ ",	Max Compute Unit: " + TToStr(m_HardwareInfo->deviceInfo[deviceIndex].maxComputeUnits)
						+ ",	Max WorkItemSize: " + workItemSize + ".";

					std::wstringstream wTipInfo;
					wTipInfo<<tipInfo.c_str();
					m_toolTip.UpdateTipText(wTipInfo.str().c_str(), GetDlgItem(IDC_HARDWAREINFO_Tree), IDC_HARDWAREINFO_Tree);
					m_toolTip.RelayEvent(pMsg);
				}
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void Cmfc_OpenGL_OpenCLDlg::OnBnClickedLoadmodelButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置过滤器   
	TCHAR szFilter[] = _T("模型文件(*.obj)|*.obj|所有文件(*.*)|*.*||");   
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("obj"), NULL, 0, szFilter, this);   
	CString strFilePath;   

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())   
	{   
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();   
		m_ModelName = CStrToStr(strFilePath);
		SetDlgItemText(IDC_MODELNAME_EDIT, strFilePath.Mid(strFilePath.ReverseFind(_T('\\'))+1));   
		CString str = _T("读取模型文件：") + strFilePath.Mid(strFilePath.ReverseFind(_T('\\'))+1);
		systemLog->PrintStatus(str.GetBuffer());
	}   
	
}


void Cmfc_OpenGL_OpenCLDlg::OnBnClickedGetModeLinfoButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Render->LoadObjInfo(m_ModelName))
	{
		//MessageBox(_T("模型数据读取完毕！"));
		systemLog->PrintStatus(_T("模型数据读取完成！\r\n"));
	}
	else
	{
		MessageBox(_T("模型数据读取失败！"));
		systemLog->PrintStatus(_T("模型数据读取失败！"));
	}
}


void Cmfc_OpenGL_OpenCLDlg::OnBnClickedInitialContextButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Render->SetDC();
	m_Render->InitContext();
}


void Cmfc_OpenGL_OpenCLDlg::OnCbnSelchangePlatformsCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelPlatformIndex = m_PlatformComBox.GetCurSel();
	m_Render->SetSelHardware(m_SelPlatformIndex);
	m_DeviceComBox.ResetContent();
	auto itr = m_Render->GetCompObj()->GetHardwareInfo()->platformInfo[m_SelPlatformIndex];
	for ( auto inItr = itr.deviceInfo.begin(); inItr != itr.deviceInfo.end(); inItr++)
	{
		m_DeviceComBox.InsertString(inItr-itr.deviceInfo.begin(),StrToCStr(inItr->name));
	}

	CString str = _T("Selected Platform:		") + StrToCStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].name) 
		+	_T("\r\n	--VenderName:	") + StrToCStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].venderName)
		+	_T("\r\n	--DeviceNum:	") + StrToCStr(TToStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceNum))
		+	_T("\r\n");
	systemLog->PrintStatus(str.GetBuffer());
}


void Cmfc_OpenGL_OpenCLDlg::OnCbnSelchangeDevicesCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelDeviceIndex = m_DeviceComBox.GetCurSel();
	m_Render->SetSelHardware(-1, m_SelDeviceIndex);

	//信息打印相关
	/*std::string str = "Selected Device: "+ m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].name;	
	systemLog->PrintStatus(StrToCStr(str).GetBuffer());*/
	CString deviceType(_T(""));
	switch (m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].type)
	{
	case CL_DEVICE_TYPE_CPU:
		deviceType = _T("CPU");
		break;
	case CL_DEVICE_TYPE_GPU:
		deviceType = _T("GPU");
		break;
	default:
		break;
	}
	std::string workItemSize("");
	workItemSize += "(";
	for (auto itr = m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].maxWorkItemSize.begin(); itr != m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].maxWorkItemSize.end(); itr++)
	{
		workItemSize += TToStr(*itr) + ",";
	}
	workItemSize = workItemSize.substr(0, workItemSize.length()-1);
	workItemSize += ")";
	CString str = _T("Selected Device:			") + StrToCStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].name)
		+	_T("\r\n	--DeviceType:		") + deviceType
		+	_T("\r\n	--ClockFreq(MHz):		") + StrToCStr(TToStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].maxClockFreq))
		+	_T("\r\n	--BitWidth:		")	+ StrToCStr(TToStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].bitWidth))
		+	_T("\r\n	--GlobalMemSize(MB):	") + StrToCStr(TToStr((float)m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].globalMemSize/(1024*1024)))
		+	_T("\r\n	--LocalMemSize(KB):	") + StrToCStr(TToStr((float)m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].localMemSize/1024))
		+	_T("\r\n	--MaxComputeUnitNum:	") + StrToCStr(TToStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].maxComputeUnits))
		+	_T("\r\n	--MaxWorkItemSize:	") + StrToCStr(workItemSize)
		+	_T("\r\n	--OpenCLVersion:		") + StrToCStr(m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].version) 
		+	_T("\r\n");
		systemLog->PrintStatus(str.GetBuffer());

		//参数设置相关
		m_DimChooseComBox.ResetContent();
		for (auto i = 0; i<m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].maxDimension; i++)
		{
			m_DimChooseComBox.AddString(StrToCStr(TToStr(i+1)));
		}

		
}

CEdit* Cmfc_OpenGL_OpenCLDlg::GetStatusEdt()
{
	return &m_StatusEdt;
}



void Cmfc_OpenGL_OpenCLDlg::OnCbnSelchangeDimChooseCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_DimChooseComBox.GetCurSel();

	switch (index+1)
	{
	case 1:
		m_DimGXEdt.EnableWindow(TRUE);
		m_DimGYEdt.EnableWindow(FALSE);
		m_DimGZEdt.EnableWindow(FALSE);
		m_DimLXEdt.EnableWindow(TRUE);
		m_DimLYEdt.EnableWindow(FALSE);
		m_DimLZEdt.EnableWindow(FALSE);
		systemLog->PrintStatus(_T("设置NDRange维度为1！"));
		m_SelDimNum = 1;
		break;
	case 2:
		m_DimGXEdt.EnableWindow(TRUE);
		m_DimGYEdt.EnableWindow(TRUE);
		m_DimGZEdt.EnableWindow(FALSE);
		m_DimLXEdt.EnableWindow(TRUE);
		m_DimLYEdt.EnableWindow(TRUE);
		m_DimLZEdt.EnableWindow(FALSE);
		systemLog->PrintStatus(_T("设置NDRange维度为2！"));
		m_SelDimNum = 2;
		break;
	case 3:
		m_DimGXEdt.EnableWindow(TRUE);
		m_DimGYEdt.EnableWindow(TRUE);
		m_DimGZEdt.EnableWindow(TRUE);
		m_DimLXEdt.EnableWindow(TRUE);
		m_DimLYEdt.EnableWindow(TRUE);
		m_DimLZEdt.EnableWindow(TRUE);
		systemLog->PrintStatus(_T("设置NDRange维度为3！"));
		m_SelDimNum = 3;
		break;
	default:
		break;
	}
}

CString Cmfc_OpenGL_OpenCLDlg::GetEdtContent(CEdit* edit, wchar_t* info)
{
	wchar_t* res = new wchar_t[edit->LineLength()];
	int len = edit->GetLine(0, res, edit->LineLength());
	if (len<=0)
	{
		CString csInfo(info);
		csInfo = _T("Error:") + CString(info);
		MessageBox(csInfo.GetBuffer());
		csInfo = _T("Error:	") + CString(info);
		systemLog->PrintStatus(csInfo.GetBuffer());
		return CString(_T(""));
	}
	else
	return CString(res, len);
}

BOOL Cmfc_OpenGL_OpenCLDlg::CheckDimEdt(CEdit* edt, wchar_t* dimName, int index, bool flag )
{
	CString dimInfo;
	wchar_t* dim = new wchar_t[edt->LineLength()];
	edt->GetLine(0, dim, edt->LineLength());
	dimInfo = CString(dim, edt->LineLength());
	if (dimInfo.GetLength()<=0)
	{
		CString info = _T("Error:") + CString(dimName) + _T("维度设置出错！");
		MessageBox(info.GetBuffer());
		info = _T("Error:	") + CString(dimName) + _T("维度设置出错！");
		systemLog->PrintStatus(info.GetBuffer());
		return FALSE;
	}
	delete[] dim;
	float fDimInfo = StrToFloat(CStrToStr(dimInfo));
	if (fDimInfo>m_HardwareInfo->platformInfo[m_SelPlatformIndex].deviceInfo[m_SelDeviceIndex].maxWorkItemSize[index] || fDimInfo<=0)
	{
		CString info;
		info = _T("Error:") + CString(dimName) + _T("维度设置出错！");
		MessageBox(info.GetBuffer());
		info = _T("Error:	") + CString(dimName) + _T("维度设定值超出设备阈值！");
		systemLog->PrintStatus(info.GetBuffer());
		return FALSE;
	}
	CString info = _T("设置") + CString(dimName) + _T("维度的工作组为--") + dimInfo;
	systemLog->PrintStatus(info.GetBuffer());
	if ( flag ) m_WorkGlobalGroup[index] = fDimInfo;
	else	m_WorkLocalGroup[index] = fDimInfo;

	return TRUE;

}

void Cmfc_OpenGL_OpenCLDlg::OnBnClickedConfigConfirmButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//NDRange参数查错
	m_WorkGlobalGroup.resize(m_SelDimNum);
	m_WorkLocalGroup.resize(m_SelDimNum);

	switch(m_SelDimNum)
	{
	case 1:
		if(!CheckDimEdt(&m_DimGXEdt, _T("GX"), 0)) 
		{
			MessageBox(_T("Error:参数初始化失败！"));
			systemLog->PrintStatus(_T("Error:	参数初始化失败！"));
			return;
		}

		if (!CheckDimEdt(&m_DimLXEdt, _T("GX"), 0, false))
		{
			MessageBox(_T("Error:参数初始化失败！"));
			systemLog->PrintStatus(_T("Error:	参数初始化失败！"));
			return;
		}
		break;
	case 2:
		{
			auto flagGX = CheckDimEdt(&m_DimGXEdt, _T("GX"), 0);
			auto flagGY = CheckDimEdt(&m_DimGYEdt, _T("GY"), 1);
			auto flagLX = CheckDimEdt(&m_DimLXEdt, _T("LX"), 0, false);
			auto flagLY = CheckDimEdt(&m_DimLYEdt, _T("LY"), 1, false);
			if( !flagGX || !flagGY || !flagLX || !flagLY)
			{
				MessageBox(_T("Error:参数初始化失败！"));
				systemLog->PrintStatus(_T("Error:	参数初始化失败！"));
				return;
			}
		}
		break;
	case 3:
		{
			auto flagGX = CheckDimEdt(&m_DimGXEdt, _T("GX"), 0);
			auto flagGY = CheckDimEdt(&m_DimGYEdt, _T("GY"), 1);
			auto flagGZ = CheckDimEdt(&m_DimGZEdt, _T("GZ"), 2);
			auto flagLX = CheckDimEdt(&m_DimLXEdt, _T("LX"), 0, false);
			auto flagLY = CheckDimEdt(&m_DimLYEdt, _T("LY"), 1, false);
			auto flagLZ = CheckDimEdt(&m_DimLZEdt, _T("LZ"), 2, false);
			if ( !flagGX || !flagGY || !flagGZ || !flagLX || !flagLY || !flagLZ)
			{
				MessageBox(_T("Error:参数初始化失败！"));
				systemLog->PrintStatus(_T("Error:	参数初始化失败！"));
				return;
			}
		}
		break;
	default:
		break;
	}

	if (m_SelDimNum<=0)
	{
		MessageBox(_T("Error:工作组和工作项维度设置错误！"));
		systemLog->PrintStatus(_T("Error:	工作组和工作项维度设置错误！"));
		return;
	}
	m_Render->GetCompObj()->SetNDRange( m_WorkGlobalGroup, m_WorkLocalGroup, m_SelDimNum);
	systemLog->PrintStatus(_T("设备工作组和工作项设置成功！——NDRange设置成功！\r\n"));

	//设置SAHSplit方法
	m_Render->GetCompObj()->SetSAHSplitMethod(m_SelRadio);

	//设置Render相关参数
	CString reflectCount = GetEdtContent(&m_ReflectCount, _T("迭代次数设置出错！"));
	//if(reflectCount == CString("")) return;
	CString viewX, viewY, viewZ;
	CString lightX, lightY, lightZ;

	viewX = GetEdtContent(&m_ViewXEdt, _T("视点位置X坐标设置出错！"));
	//if(viewX == CString("")) return;
	viewY = GetEdtContent(&m_ViewYEdt, _T("视点位置Y坐标设置出错！"));
	//if(viewY == CString("")) return;
	viewZ = GetEdtContent(&m_ViewZEdt, _T("视点位姿Z坐标设置出错！"));
	//if(viewZ == CString("")) return;
	lightX = GetEdtContent(&m_LightXEdt, _T("光源位置X坐标设置出错！"));
	//if(lightX == CString("")) return;
	lightY = GetEdtContent(&m_LightYEdt, _T("光源位置Y坐标设置出错！"));
	//if(lightY == CString("")) return;
	lightZ = GetEdtContent(&m_LightZEdt, _T("光源位置Z坐标设置出错！"));
	//if(lightZ == CString("")) return;

	if (reflectCount == CString("") || viewX == CString("") || viewY == CString("") || viewZ == CString("") || lightX == CString("") || lightY == CString("") || lightZ == CString("")) 
	{
		MessageBox(_T("Error:参数初始化失败！"));
		systemLog->PrintStatus(_T("Error:	参数初始化失败！"));
		return;
	}

	CString info = 
		_T("渲染参数设置：\r\n	--光线迭代次数：	") + reflectCount + _T("\r\n")
		+	_T("	--视点位置：	(") + viewX + _T(",") + viewY + _T(",") + viewZ +_T(")\r\n")
		+	_T("	--光源位置：	(") + lightX + _T(",") + lightY + _T(",") + lightZ + _T(")\r\n");

	systemLog->PrintStatus(info.GetBuffer());

	m_Render->GetCompObj()->SetRenderParam(StrToFloat(CStrToStr(reflectCount)), StrToFloat(CStrToStr(viewX)), StrToFloat(CStrToStr(viewY)), StrToFloat(CStrToStr(viewZ)), StrToFloat(CStrToStr(lightX)), StrToFloat(CStrToStr(lightY)), StrToFloat(CStrToStr(lightZ)));

	m_Render->GetCompObj()->SetParamReady();
	systemLog->PrintStatus(_T("参数设置完毕！\r\n"));	

	
}


void Cmfc_OpenGL_OpenCLDlg::OnBnClickedCommonSplitRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelRadio = SAHSPLIT_COMM;
}


void Cmfc_OpenGL_OpenCLDlg::OnBnClickedSAASAHSplitRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelRadio = SAHSPLIT_SAA;
}




void Cmfc_OpenGL_OpenCLDlg::OnBnClickedPSOSAHSplitRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelRadio = SAHSPLIT_PSO;
}


void Cmfc_OpenGL_OpenCLDlg::OnBnClickedOffLineRenderingButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Render->GetCompObj()->SetRenderDlg(dynamic_cast<CDialog*>(m_RenderDlg));
	m_Render->SetRenderDlg(dynamic_cast<CDialog*>(m_RenderDlg));
	if(!m_Render->GetCompObj()->OffLineRendering()) return;
	m_RenderDlg->DoModal();
}


void Cmfc_OpenGL_OpenCLDlg::OnBnClickedRealtimeRenderingButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Render->GetCompObj()->SetRenderDlg(dynamic_cast<CDialog*>(m_RenderDlg));
	m_Render->SetRenderDlg(dynamic_cast<CDialog*>(m_RenderDlg));
	if(!m_Render->GetCompObj()->RealTimeRendering()) return;
	m_RenderDlg->DoModal();
}

CRenderDlg* Cmfc_OpenGL_OpenCLDlg::GetRenderDlg()
{
	return m_RenderDlg;
}