#pragma once

#include "stdafx.h"
#include "mfc_OpenGL_OpenCL.h"
#include "cusLib.h"


class CPraCompute
{
public:
	virtual HardwareInfo* GetHardwareInfo() = 0;
	virtual BOOL InitContext()= 0;
	virtual void SetGLCLShared(bool flag = false) = 0;
	virtual void SetNDRange(std::vector<int>&, std::vector<int>&, int dim = 0) = 0;
	virtual BOOL OffLineRendering() = 0;
	virtual BOOL RealTimeRendering() = 0;
	virtual void BitonicSort() = 0;
	virtual void BuildKDTree() = 0;
	virtual BOOL CalPBO() = 0;
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

	virtual BOOL InitContext();
	virtual void SetGLCLShared(bool flag = false);
	virtual void SetSelHardware(int platformIndex = 0, int deviceIndex = 0);
	virtual void SetNDRange(std::vector<int>& workGlobalGroup, std::vector<int>& workLocalGroup, int dim = 0);
	virtual	void SetSAHSplitMethod(int method);
	virtual void SetRenderParam(int count = 0, int viewX = 0, int viewY = 0, int viewZ = 0, int lightX = 0, int lightY = 0, int lightZ = 0);
	virtual BOOL SetSAASAHParam(int oriTem, int thresholdTem,int singleSampleCount, float desSpeed);
	virtual BOOL SetPSOSAHParam(int particleNum, int sampleCount, float inertiaWeight, float c1Weight, float c2Weight, float singleMaxShift);
	virtual void SetParamReady(bool flagParam = true);
	virtual BOOL OffLineRendering();
	virtual BOOL RealTimeRendering();
	virtual void SetRenderDlg(CDialog* dlg);
	virtual void SetPBO(GLuint& pbo);
	virtual void SetDC(HDC& hdc, HGLRC& hglrc);
	virtual void BitonicSort();
	virtual void BuildKDTree();
	virtual BOOL CalPBO();
	virtual void SetModelInfo(ModelInfo*);
	virtual ConfigInfo* GetConfigInfo();
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
	GLuint			m_PBO;
	cl_mem			m_PBOMem;
	HDC				m_HDC;
	HGLRC			m_HGLRC;
	enum{ OFFLINE_RENDERING = 0x1000, REALTIME_RENDERING = 0x1001,};
	int				m_RenderType;

	DrawableInfo*	m_DrawInfo;
	ModelInfo*		m_ModleInfo;
	std::vector<TriangleCandidateSplitPlane> m_InputInfo;
	cl_mem			m_InputInfoMem;
	cl_mem			m_SplitNodeArrayMem;
};


