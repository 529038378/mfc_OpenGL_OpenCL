
// mfc_OpenGL_OpenCLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "mfc_OpenGL_OpenCLDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <algorithm>

#include "cusLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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
}

BEGIN_MESSAGE_MAP(Cmfc_OpenGL_OpenCLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
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
	CRect rect(500, 0, 1000, 300);
	m_Render = new CRender;
	m_Render->Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, rect, this, 0);
	
	//硬件信息显示
	m_hardwareInfo = m_Render->GetCompObj()->GetHardwareInfo();
	ASSERT(m_hardwareInfo);
	
	m_hardwarePlatformNode.resize(m_hardwareInfo->platformIDs.size());
	m_root = m_hardwareInfoTreeCtl.InsertItem(_T("本机可供GPGPU硬件信息"), 0, 0);

	static int deviceInfoIndex = 0;
	for (auto platformItr = m_hardwareInfo->platformInfo.begin(); platformItr != m_hardwareInfo->platformInfo.end(); platformItr++)
	{
		std::string platformInfo("");
		//platformInfo = platformItr->name.substr(0, platformItr->name.length()-1) + " ( Vendor Name:" + platformItr->venderName.substr(0, platformItr->venderName.length()-1) + " device num: " + TtoStr(platformItr->deviceNum) +")";
		platformInfo = platformItr->name;

		std::wstringstream wsPlatformInfo;
		wsPlatformInfo<<platformInfo.c_str();
		int platformIndex = platformItr - m_hardwareInfo->platformInfo.begin();
		m_hardwarePlatformNode[platformIndex] = m_hardwareInfoTreeCtl.InsertItem(wsPlatformInfo.str().c_str(), m_root, TVI_LAST);
		m_hardwareInfoTreeCtl.SetItemData(m_hardwarePlatformNode[platformIndex], platformIndex);

		m_harewareDeviceNode.resize(m_harewareDeviceNode.size()+platformItr->deviceInfo.size());
		m_hardwareInfo->deviceInfo.resize(m_hardwareInfo->deviceInfo.size() + platformItr->deviceInfo.size());
		for (auto deviceItr = platformItr->deviceInfo.begin(); deviceItr != platformItr->deviceInfo.end(); deviceItr++, deviceInfoIndex++)
		{
			int deviceIndex = deviceItr - platformItr->deviceInfo.begin();
			std::wstringstream wsDeviceInfo;
			wsDeviceInfo<<deviceItr->name.c_str();
			HTREEITEM LeafNodeItem = m_hardwareInfoTreeCtl.InsertItem(wsDeviceInfo.str().c_str(), m_hardwarePlatformNode[platformIndex], TVI_LAST);
			m_harewareDeviceNode[deviceInfoIndex]=LeafNodeItem;
			m_hardwareInfoTreeCtl.SetItemData(LeafNodeItem, deviceIndex);
			m_hardwareInfo->deviceInfo[deviceInfoIndex] = *deviceItr;
		}

	}
	m_toolTip.CreateEx(this, TTS_ALWAYSTIP);
	CRect treeClientRect;
	m_hardwareInfoTreeCtl.GetClientRect(&treeClientRect);
	m_toolTip.AddTool(GetDlgItem(IDC_HARDWAREINFO_Tree), _T("本机硬件信息"), &treeClientRect, IDC_HARDWAREINFO_Tree);

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
					tipInfo = "VenderName: " + m_hardwareInfo->platformInfo[platformIndex].venderName.substr(0, m_hardwareInfo->platformInfo[platformIndex].venderName.length()-1) + ",	 Device Num: " + TtoStr(m_hardwareInfo->platformInfo[platformIndex].deviceNum) + ".";

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
					for (auto itr = m_hardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize.begin(); itr != m_hardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize.end(); itr++)
					{
						int tmp = m_hardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize[0];
						tmp = m_hardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize[1];
						tmp = m_hardwareInfo->deviceInfo[deviceIndex].maxWorkItemSize[2];
						workItemSize += TtoStr(*itr) + ",";
					}
					workItemSize = "(" + workItemSize.substr(0, workItemSize.length()-1) +")";
					tipInfo = "Type: " + TtoStr(m_hardwareInfo->deviceInfo[deviceIndex].type) 
						+",	Max Clock Freq: " + TtoStr(m_hardwareInfo->deviceInfo[deviceIndex].maxClockFreq) 
						+ ",	BitWidth: " + TtoStr(m_hardwareInfo->deviceInfo[deviceIndex].bitWidth) 
						+ ",	Global Mem Size: " + TtoStr(m_hardwareInfo->deviceInfo[deviceIndex].globalMemSize)
						+ ",	Local Mem Size: " + TtoStr(m_hardwareInfo->deviceInfo[deviceIndex].localMemSize)
						+ ",	Max Compute Unit: " + TtoStr(m_hardwareInfo->deviceInfo[deviceIndex].maxComputeUnits)
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