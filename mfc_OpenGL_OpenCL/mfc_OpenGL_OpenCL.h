
// mfc_OpenGL_OpenCL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmfc_OpenGL_OpenCLApp:
// �йش����ʵ�֣������ mfc_OpenGL_OpenCL.cpp
//

class Cmfc_OpenGL_OpenCLApp : public CWinApp
{
public:
	Cmfc_OpenGL_OpenCLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmfc_OpenGL_OpenCLApp theApp;