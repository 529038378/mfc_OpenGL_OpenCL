#pragma once


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
	int		CusSetPixelFormat(HDC hDC);
	void	RenderScene();
	COpenCLCompute*	GetCompObj();
	BOOL	LoadObjInfo(std::string fileName);
	void	InitContext();
	void	SetSelHardware(int platformIndex = 0, int deviceIndex = 0);	
	void	SetRenderDlg(CDialog* dlg);
	void	SetPBO();
	void	SetDC();
private:
	HDC		m_HDC;
	HGLRC	m_HGLRC;
	static COpenCLCompute*		m_CompObj;
	ModelInfo*		m_ModelInfo;
	std::string		m_FileName;
	CDialog*		m_RenderDlg;
	GLuint			m_RenderTexture;
	GLuint			m_PBO;
	GLuint			m_VertArrayID;
	GLuint			m_QuadVert;
	GLuint			m_UV;
	GLuint			m_Program;
	GLuint			m_TexID;
};


