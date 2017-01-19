#pragma once

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "cusLib.h"


class CPraCompute
{
public:
	virtual HardwareInfo* GetHardwareInfo() = 0;
	virtual void InitContext(HDC&, HGLRC&)= 0;
	virtual void SetGLCLShared(bool flag = false) = 0;
	virtual void SetNDRange(std::vector<int>&, int dim = 0) = 0;
	virtual void OffLineRendering() = 0;
	virtual void RealTimeRendering() = 0;
};

class COpenCLCompute : public CPraCompute
{
public:
	COpenCLCompute();
	~COpenCLCompute();
	virtual COpenCLCompute* GetInstance();
	
	virtual HardwareInfo* GetHardwareInfo();
	virtual PlatformInfo GetPlatformInfo(std::vector<cl_platform_id>::iterator& itr);
	virtual DeviceInfo GetDeviceInfo(std::vector<cl_device_id>::iterator& itr);

	virtual void InitContext(HDC& hDC, HGLRC& hglrc);
	virtual void SetGLCLShared(bool flag = false);
	virtual void SetSelHardware(int platformIndex = 0, int deviceIndex = 0);
	virtual void SetNDRange(std::vector<int>& workGroup, int dim = 0);
	virtual	void SetSAHSplitMethod(int method);
	virtual void SetRenderParam(int count = 0, int viewX = 0, int viewY = 0, int viewZ = 0, int lightX = 0, int lightY = 0, int lightZ = 0);
	virtual void SetParamReady(bool flagParam = true);
	virtual void OffLineRendering();
	virtual void RealTimeRendering();
	virtual void SetRenderDlg(CDialog* dlg);
private:
	HardwareInfo*	m_HardwareInfo;
	cl_context		m_Context;
	bool			m_GLCLShared;
	static COpenCLCompute* m_instance;
	ConfigInfo*		m_ConfigInfo;
	cl_program		m_Program;
	cl_kernel		m_BitonicSortKernel;
	cl_kernel		m_SAHSplitKernel;
	cl_kernel		m_CommSAHSplitKernel;
	cl_kernel		m_SAASAHSplitKernel;
	cl_kernel		m_PSOSAHSplitKernel;
	cl_kernel		m_RayTraceKernel;
	cl_command_queue	m_Queue;
	int				m_ReflectCount;
	bool			m_ContextReady;
	bool			m_ParamReady;
	CDialog*		m_RenderDlg;
};


