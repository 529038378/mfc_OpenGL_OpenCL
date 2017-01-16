#pragma once
#include <gl/glut.h>

#include "PrallalCompute.h"
// CRender

class CRender : public CWnd
{
	DECLARE_DYNAMIC(CRender)

public:
	CRender();
	virtual ~CRender();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

public:
	int CusSetPixelFormat(HDC hDC);
	void RenderScene();
	COpenCLCompute* GetCompObj();
private:
	HDC m_hdc;
	HGLRC m_hglrc;
	static COpenCLCompute* m_CompObj;
};


