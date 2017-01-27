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

	if ( glewInit() != GLEW_OK)
	{
		MessageBox(_T("glew��ʼ��ʧ�ܣ�"));
		exit(0);
	}

	SetDC();
	//��ʼ����Ⱦ����������
	glGenTextures(1, &m_RenderTexture);
	glBindTexture(GL_TEXTURE_2D, m_RenderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ciRenderWinWidth, ciRenderWinHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ciRenderWinWidth, ciRenderWinHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(0, m_RenderTexture);



	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_RenderTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenBuffers(1, &m_VertArrayID);
	glBindVertexArray(m_VertArrayID);

	glGenBuffers(1, &m_QuadVert);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfQuadVertBuffer), cfQuadVertBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_PBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_PBO);
	glBufferData(GL_ARRAY_BUFFER, ciRenderWinWidth*ciRenderWinHeight*sizeof(unsigned char), 0, GL_STREAM_DRAW);

	glGenBuffers(1, &m_UV);
	glBindBuffer(GL_ARRAY_BUFFER, m_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UV), UV, GL_STATIC_DRAW);
	m_Program = LoadShaders("vert.vs","frag.fs");
	m_TexID = glGetUniformLocation(m_Program, "renderedTexture");
	glClearColor(153.0f, 51.0f, 250.0f, 0.0f);

	SetPBO();

	return 0;
}


void CRender::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderScene();
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

void CRender::SetPBO()
{
	GetCompObj()->SetPBO(m_PBO);
}

void CRender::RenderScene()
{
	/*glBegin(GL_LINES);
	glVertex2f(-0.5f, -0.3f);
	glVertex2f(0.4f, 0.6f);
	glEnd();*/
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO);
	glBindTexture(GL_TEXTURE_2D, m_RenderTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ciRenderWinWidth, ciRenderWinHeight, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, ciRenderWinWidth, ciRenderWinHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_Program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RenderTexture);
	glUniform1i(m_TexID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVert);
	glVertexAttribPointer(
	0,
	3,
	GL_FLOAT,
	GL_FALSE,
	0,
	(void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_UV);
	glVertexAttribPointer(
	1,
	2,
	GL_FLOAT,
	GL_FALSE,
	0,
	(void*)0
	);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
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
	m_CompObj->InitContext();
}

void CRender::SetDC()
{
	m_CompObj->SetDC(m_HDC, m_HGLRC);
}

void CRender::SetSelHardware(int platformIndex /* = 0 */, int deviceIndex /* = 0 */)
{
	GetCompObj()->SetSelHardware(platformIndex, deviceIndex);
}

void CRender::SetRenderDlg(CDialog* dlg)
{
	m_RenderDlg = dlg;
}
