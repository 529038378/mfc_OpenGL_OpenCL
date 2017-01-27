#pragma once
#include "PrallalCompute.h"
#include "Log.h"

COpenCLCompute* COpenCLCompute::m_instance = NULL;
COpenCLCompute::COpenCLCompute()
{
	m_HardwareInfo = new HardwareInfo;
	m_GLCLShared = false;
	m_ConfigInfo = new ConfigInfo;
	m_GLCLShared = true;
	m_ConfigInfo->viewPos.resize(3);
	m_ConfigInfo->lightPos.resize(3);
	m_ContextReady = false;
	m_ParamReady = false;

}

COpenCLCompute::~COpenCLCompute()
{
	if (m_HardwareInfo)
	{	
		delete m_HardwareInfo;
		m_HardwareInfo = NULL;
	}

	if ( m_instance )
	{
		delete m_instance;
		m_instance = NULL;
	}

	if (m_ConfigInfo)
	{
		delete m_ConfigInfo;
		m_ConfigInfo = NULL;
	}
	
}

COpenCLCompute* COpenCLCompute::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new COpenCLCompute;
	}
	return m_instance;
}

HardwareInfo* COpenCLCompute::GetHardwareInfo()
{
	cl_int ciStatus;
	cl_uint platformNum;
	std::vector<cl_platform_id> platformIDs;
	ciStatus = clGetPlatformIDs(NULL, NULL, &platformNum);
	ASSERT(ciStatus == CL_SUCCESS);
	
	ASSERT(platformNum > 0);
	m_HardwareInfo->platformIDs.resize(platformNum);
	m_HardwareInfo->platformInfo.resize(platformNum);
	ciStatus = clGetPlatformIDs(platformNum, &m_HardwareInfo->platformIDs[0], 0);
	ASSERT(ciStatus == CL_SUCCESS);

	for (auto itr = m_HardwareInfo->platformIDs.begin(); itr != m_HardwareInfo->platformIDs.end(); itr++)
	{
		//获取平台信息
		//PlatformInfo info = GetPlatformInfo(itr);
		m_HardwareInfo->platformInfo[itr - m_HardwareInfo->platformIDs.begin()] = GetPlatformInfo(itr);
	}
	return m_HardwareInfo;
}

//获取平台信息
PlatformInfo COpenCLCompute::GetPlatformInfo(std::vector<cl_platform_id>::iterator& itr)
{
	PlatformInfo platformInfo;
	platformInfo.id = *itr;
	cl_int ciStatus;
	size_t size;

	ciStatus =clGetPlatformInfo(*itr, CL_PLATFORM_NAME, NULL, NULL, &size);
	ASSERT(ciStatus == CL_SUCCESS);
	platformInfo.name.resize(size);	
	ciStatus = clGetPlatformInfo(*itr, CL_PLATFORM_NAME, size, const_cast<char*>(platformInfo.name.c_str()), NULL);
	ASSERT(ciStatus == CL_SUCCESS);

	ciStatus = clGetPlatformInfo(*itr, CL_PLATFORM_VENDOR, NULL, NULL, &size);
	ASSERT(ciStatus == CL_SUCCESS);
	platformInfo.venderName.resize(size);
	ciStatus = clGetPlatformInfo(*itr, CL_PLATFORM_VENDOR, size, const_cast<char*>(platformInfo.venderName.c_str()), NULL);
	ASSERT(ciStatus == CL_SUCCESS);

	ciStatus = clGetPlatformInfo(*itr, CL_PLATFORM_PROFILE, NULL, NULL, &size);
	ASSERT(ciStatus == CL_SUCCESS);
	platformInfo.profile.resize(size);
	ciStatus = clGetPlatformInfo(*itr, CL_PLATFORM_PROFILE, size, const_cast<char*>(platformInfo.profile.c_str()), NULL);
	ASSERT(ciStatus == CL_SUCCESS);
	
	ciStatus = clGetDeviceIDs(*itr, CL_DEVICE_TYPE_ALL, NULL, NULL, &platformInfo.deviceNum);
	ASSERT( ciStatus == CL_SUCCESS && platformInfo.deviceNum >0);
	platformInfo.deviceIDs.resize(platformInfo.deviceNum);
	platformInfo.deviceInfo.resize(platformInfo.deviceNum);
	ciStatus = clGetDeviceIDs(*itr, CL_DEVICE_TYPE_ALL, platformInfo.deviceNum, &platformInfo.deviceIDs[0], NULL);
	ASSERT( ciStatus == CL_SUCCESS );

	//获取单个device的信息
	for ( auto deviceItr = platformInfo.deviceIDs.begin(); deviceItr != platformInfo.deviceIDs.end(); deviceItr++)
	{
		platformInfo.deviceInfo[deviceItr - platformInfo.deviceIDs.begin()] = GetDeviceInfo(deviceItr);
	}
	return platformInfo;
}

DeviceInfo COpenCLCompute::GetDeviceInfo(std::vector<cl_device_id>::iterator& itr)
{
	DeviceInfo deviceInfo;
	deviceInfo.id = *itr;
	cl_int ciStatus;
	size_t size;

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_NAME, NULL, NULL, &size);
	ASSERT( ciStatus == CL_SUCCESS);
	deviceInfo.name.resize(size);
	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_NAME, size, const_cast<char*>(deviceInfo.name.c_str()), NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint), (void*) &deviceInfo.bitWidth, NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), (void*) &deviceInfo.globalMemSize, NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), (void*) &deviceInfo.localMemSize, NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), (void*) &deviceInfo.maxClockFreq, NULL);
	ASSERT( ciStatus == CL_SUCCESS);
	
	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), (void*) &deviceInfo.maxComputeUnits, NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), (void*) &deviceInfo.maxWorkGroupSize, NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), (void*) &deviceInfo.maxDimension, NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	deviceInfo.maxWorkItemSize.resize(deviceInfo.maxDimension);
	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_MAX_WORK_ITEM_SIZES, deviceInfo.maxDimension*sizeof(size_t), (void*) &deviceInfo.maxWorkItemSize[0], NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_TYPE, sizeof(cl_device_type), (void*) &deviceInfo.type, NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_VERSION, NULL, NULL, &size);
	ASSERT( ciStatus == CL_SUCCESS);
	deviceInfo.version.resize(size);
	ciStatus = clGetDeviceInfo(*itr, CL_DEVICE_VERSION, size, (void*) const_cast<char*>(deviceInfo.version.c_str()), NULL);
	ASSERT( ciStatus == CL_SUCCESS);

	return deviceInfo;
}

void COpenCLCompute::InitContext()
{
	//设置context
	cl_int iStatus;
	if (m_GLCLShared)
	{
		cl_context_properties clProp[]= {
		CL_GL_CONTEXT_KHR,
		(cl_context_properties)m_HGLRC,
		CL_WGL_HDC_KHR,
		(cl_context_properties)m_HDC,
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)m_HardwareInfo->platformIDs[m_ConfigInfo->selPlaformIndex],
		0
		};
		m_Context = clCreateContext(clProp, 1, &m_HardwareInfo->platformInfo[m_ConfigInfo->selPlaformIndex].deviceIDs[m_ConfigInfo->selDeviceIndex], NULL, NULL, &iStatus);
		if(!CheckError(iStatus, _T("建立OpenGL-OpenCL共享context"))) return;
	}
	else
	{
		cl_context_properties clProp[] = {
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)m_HardwareInfo->platformIDs[m_ConfigInfo->selPlaformIndex],
			0
		};
		m_Context = clCreateContext(clProp, 1, &m_HardwareInfo->platformInfo[m_ConfigInfo->selPlaformIndex].deviceIDs[m_ConfigInfo->selDeviceIndex], NULL, NULL, &iStatus);
		if(!CheckError(iStatus, _T("建立context"))) return;
	}

	//读取kernel文件
	std::ifstream ifFile("kernel.cl", std::ios_base::binary);
	ASSERT(ifFile.is_open());
	
	size_t stFileLen;
	ifFile.seekg(0, std::ios_base::end);
	stFileLen = ifFile.tellg();
	ifFile.seekg(0, std::ios_base::beg);
	std::vector<char> svcData(stFileLen + 1);
	ifFile.read(&svcData[0], stFileLen);
	svcData[stFileLen] = 0;
	const char *ccSource = &svcData[0];
	systemLog->PrintStatus(_T("读取kernel文件——成功！"));

	//编译kernel程序
	m_Program = clCreateProgramWithSource(m_Context, 1,	&ccSource, &stFileLen, &iStatus);
	ASSERT(iStatus == CL_SUCCESS);
	const char ccOptions[] ="-cl-std=CL1.2 ";//-DT0  -DNMAX -DDEC_SPEED -DMAXITER -w ";
	iStatus = clBuildProgram(m_Program, 1, &m_HardwareInfo->platformInfo[m_ConfigInfo->selPlaformIndex].deviceIDs[m_ConfigInfo->selDeviceIndex], ccOptions, 0, 0);
	if ( CL_SUCCESS != iStatus )
	{
		char cInfo[0x10000];
		clGetProgramBuildInfo(m_Program, m_HardwareInfo->platformInfo[m_ConfigInfo->selPlaformIndex].deviceIDs[m_ConfigInfo->selDeviceIndex], CL_PROGRAM_BUILD_LOG, 0x10000, cInfo, NULL);
		CString str = _T("编译device端程序——失败！！！\r\n原因如下：\r\n") + StrToCStr(cInfo) ;
		systemLog->PrintStatus(str.GetBuffer());
		return;
	}
	systemLog->PrintStatus(_T("编译device端程序——成功！"));

	m_BitonicSortKernel = clCreateKernel(m_Program, "BitonicSort", &iStatus);
	if(!CheckError(iStatus, _T("获取BitonicSortKernel"))) return;
	m_CommSAHSplitKernel = clCreateKernel(m_Program, "SAHSplit", &iStatus);
	if (!CheckError(iStatus, _T("获取SAHSplitKernel")))return;
	m_SAASAHSplitKernel = clCreateKernel(m_Program, "SAASAHSplit", &iStatus);
	if (!CheckError(iStatus, _T("获取SAASAHSplitKernel"))) return;
	m_PSOSAHSplitKernel = clCreateKernel(m_Program, "PSOSAHSplit", &iStatus);
	if (!CheckError(iStatus, _T("获取PSOSAHSplitKernel"))) return;
	m_RayTraceKernel = clCreateKernel(m_Program, "RayTrace", &iStatus);
	if (!CheckError(iStatus, _T("获取RayTraceKernel"))) return;
	
	m_Queue = clCreateCommandQueue(m_Context, m_HardwareInfo->platformInfo[m_ConfigInfo->selPlaformIndex].deviceIDs[m_ConfigInfo->selDeviceIndex], NULL, &iStatus);
	if (!CheckError(iStatus, _T("创建设备命令队列"))) return;


	systemLog->PrintStatus(_T("运算环境初始化——成功！\r\n"));
	m_ContextReady = true;
}

void COpenCLCompute::SetSelHardware(int platformIndex /* = 0 */, int deviceIndex /* = 0 */)
{
	if ( platformIndex != -1 ) m_ConfigInfo->selPlaformIndex = platformIndex;
	if (deviceIndex != -1 ) m_ConfigInfo->selDeviceIndex = deviceIndex;
}

void COpenCLCompute::SetGLCLShared(bool flag /* = false */)
{
	m_GLCLShared = flag;
}

void COpenCLCompute::SetNDRange(std::vector<int>& workGlobalGroup, std::vector<int>& workLocalGroup, int dim /* = 0 */ )
{
	
	m_ConfigInfo->workGlobalGroup.resize(dim);
	m_ConfigInfo->workLocalGroup.resize(dim);
	for (auto itr = m_ConfigInfo->workGlobalGroup.begin(); itr != m_ConfigInfo->workGlobalGroup.end(); itr++)
	{
		int index = itr - m_ConfigInfo->workGlobalGroup.begin();
		m_ConfigInfo->workGlobalGroup[index] = workGlobalGroup[index];
		m_ConfigInfo->workLocalGroup[index] = workLocalGroup[index];
	}
	
}

void COpenCLCompute::SetSAHSplitMethod(int method)
{
	switch (method)
	{
	case SAHSPLIT_COMM:
		m_SAHSplitKernel = m_CommSAHSplitKernel;
		systemLog->PrintStatus(_T("使用常规SAH划分方法！"));
		break;
	case SAHSPLIT_SAA:
		m_SAHSplitKernel = m_SAASAHSplitKernel;
		systemLog->PrintStatus(_T("使用模拟退火方法进行SAH划分！"));
		break;
	case SAHSPLIT_PSO:
		m_SAHSplitKernel = m_PSOSAHSplitKernel;
		systemLog->PrintStatus(_T("使用粒子群方法进行SAH划分！"));
		break;
	default: 
		break;
	}
}

void COpenCLCompute::SetRenderParam(int count /* = 0 */, int viewX /* = 0 */, int viewY /* = 0 */, int viewZ /* = 0 */, int lightX /* = 0 */, int lightY /* = 0 */, int lightZ /* = 0 */)
{
	m_ConfigInfo->reflectCount = count;

	m_ConfigInfo->lightPos[0] = lightX;
	m_ConfigInfo->lightPos[1] = lightY;
	m_ConfigInfo->lightPos[2] = lightZ;

	m_ConfigInfo->viewPos[0] = viewX;
	m_ConfigInfo->viewPos[1] = viewY;
	m_ConfigInfo->viewPos[2] = viewZ;
}

void COpenCLCompute::SetParamReady(bool flagParam /* = true */)
{
	m_ParamReady = flagParam;
	
}



BOOL COpenCLCompute::OffLineRendering()
{
	cl_int iStatus;
	if(!m_ContextReady)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,_T("Error:运算环境未初始化！"), NULL, 0);
		systemLog->PrintStatus(_T("Error:	运算环境未初始化！"));
		return FALSE;
	}
	if (!m_ParamReady)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,_T("Error:参数设置出错！"), NULL, 0);
		systemLog->PrintStatus(_T("Error:	参数设置出错！"));
		return FALSE;
	}

	//计算PBO_Mem
	m_PBOMem = clCreateFromGLBuffer(m_Context, CL_MEM_WRITE_ONLY, m_PBO, &iStatus);
	if(!CheckError(iStatus, _T("建立PBO"))) return FALSE;
	
	clSetKernelArg(m_SAHSplitKernel, 0, sizeof(cl_mem), &m_PBOMem);
	
	glFinish();
	iStatus = clEnqueueAcquireGLObjects(m_Queue, 1, &m_PBOMem, 0, NULL, NULL);
	CheckError(iStatus, _T("获取GL环境下的PBO控制权"));
	iStatus = clEnqueueNDRangeKernel(m_Queue, m_RayTraceKernel, m_ConfigInfo->workGlobalGroup.size(), NULL, &m_ConfigInfo->workGlobalGroup[0], &m_ConfigInfo->workLocalGroup[0], 0, 0, 0);
	CheckError(iStatus, _T("计算PBO"));
	clFinish(m_Queue);
	iStatus = clEnqueueReleaseGLObjects(m_Queue, 1, &m_PBOMem, 0, NULL, NULL);
	CheckError(iStatus, _T("释放CL环境下的PBO控制权"));

	std::vector<unsigned char> tmpData(ciRenderWinHeight*ciRenderWinWidth);
	clEnqueueReadBuffer(m_Queue, m_PBOMem, CL_TRUE, 0, ciRenderWinHeight*ciRenderWinWidth*sizeof(unsigned char), &tmpData[0], 0, 0, 0);
	systemLog->PrintStatus(_T("离线渲染成功！"));
	return TRUE;
}

BOOL COpenCLCompute::RealTimeRendering()
{
	cl_int iStatus;
	if(!m_ContextReady)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,_T("Error:运算环境未初始化！"), NULL, 0);
		systemLog->PrintStatus(_T("Error:	运算环境未初始化！"));
		return FALSE;
	}
	if (!m_ParamReady)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd,_T("Error:参数设置出错！"), NULL, 0);
		systemLog->PrintStatus(_T("Error:	参数设置出错！"));
		return FALSE;
	}

	//计算PBO_Mem
	m_PBOMem = clCreateFromGLBuffer(m_Context, CL_MEM_WRITE_ONLY, m_PBO, &iStatus);

	if(!CheckError(iStatus, _T("建立PBO"))) return FALSE;

	clSetKernelArg(m_SAHSplitKernel, 1, sizeof(cl_mem), &m_PBOMem);

	systemLog->PrintStatus(_T("实时渲染成功！"));
	return TRUE;
}

void COpenCLCompute::SetRenderDlg(CDialog* dlg)
{
	m_RenderDlg = dlg;
}

void COpenCLCompute::SetPBO(GLuint& pbo)
{
	m_PBO = pbo;
}

void COpenCLCompute::SetDC(HDC& hdc, HGLRC& hglrc)
{
	m_HDC = hdc;
	m_HGLRC = hglrc;
}