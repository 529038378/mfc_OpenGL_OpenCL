// Render.cpp : ʵ���ļ�
//
#pragma once

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "Render.h"
#include "Log.h"

// CRender

IMPLEMENT_DYNAMIC(CRender, CWnd)

COpenCLCompute* CRender::m_CompObj = NULL;

CRender::CRender()
{
	m_CompObj = new COpenCLCompute;
	m_ModelInfo = new ModelInfo;
}

CRender::~CRender()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_HGLRC);
	::ReleaseDC(m_hWnd, m_HDC);

	if ( m_CompObj )
	{
		delete m_CompObj;
		m_CompObj = NULL;
	}

	if ( m_ModelInfo )
	{
		delete m_ModelInfo;
		m_ModelInfo = NULL;
	}
}


BEGIN_MESSAGE_MAP(CRender, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CRender ��Ϣ�������




int CRender::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	if(CusSetPixelFormat(::GetDC(m_hWnd))==FALSE)
		return 0;

	// ��û�ͼ������
	m_HDC = ::GetDC(m_hWnd);
	// ������Ⱦ������
	m_HGLRC = wglCreateContext(m_HDC);
	// ʹ��ͼ������Ϊ��ǰ�����̵ֳĵ�ǰ��ͼ������ 
	wglMakeCurrent(m_HDC, m_HGLRC); 
	return 0;
}


void CRender::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderScene();
	glPopMatrix();
	glFlush();
	SwapBuffers(m_HDC);
}

int CRender::CusSetPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | 
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;	
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	int iPixelFormat; 

	// Ϊ�豸������õ���ƥ������ظ�ʽ 
	if((iPixelFormat = ChoosePixelFormat(hDC, &pixelDesc)) == 0)
	{
		MessageBox(_T("ChoosePixelFormat Failed"), NULL, MB_OK);
		return FALSE;
	}

	// ������ƥ������ظ�ʽΪ��ǰ�����ظ�ʽ 
	if(SetPixelFormat(hDC, iPixelFormat, &pixelDesc) == FALSE)
	{
		MessageBox(_T("SetPixelFormat Failed"), NULL, MB_OK);
		return FALSE;
	}
	return TRUE;
}

void CRender::RenderScene()
{
	glBegin(GL_LINES);
	glVertex2f(-0.5f, -0.3f);
	glVertex2f(0.4f, 0.6f);
	glEnd();
}

COpenCLCompute* CRender::GetCompObj()
{
	return m_CompObj;
}

BOOL CRender::LoadObjInfo(std::string fileName)
{
	m_FileName = fileName;
	return loadOBJ(fileName.c_str(), m_ModelInfo->verts, m_ModelInfo->UVs, m_ModelInfo->normals);
	
}

void CRender::InitContext()
{
	m_CompObj->InitContext(m_HDC, m_HGLRC);
}

void CRender::SetSelHardware(int platformIndex /* = 0 */, int deviceIndex /* = 0 */)
{
	GetCompObj()->SetSelHardware(platformIndex, deviceIndex);
}

void CRender::SetRenderDlg(CDialog* dlg)
{
	m_RenderDlg = dlg;
}