#pragma once
#include "PrallalCompute.h"
#include "Log.h"

COpenCLCompute* COpenCLCompute::m_instance = NULL;
extern std::vector<size_t> pixelGlobalGroup;
extern std::vector<size_t> pixelLocalGroup;

COpenCLCompute::COpenCLCompute()
{
	m_HardwareInfo = new HardwareInfo;
	m_GLCLShared = false;
	m_ConfigInfo = new ConfigInfo;
	m_GLCLShared = true;
	m_ConfigInfo->viewPos.resize(3);
	m_ConfigInfo->lightPos.resize(3);
	m_ConfigInfo->SAHType = SAHSPLIT_COMM;
	m_ConfigInfo->maxKDTreeDepth = 20;
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

BOOL COpenCLCompute::InitContext()
{
	//设置context
	
	cl_int iStatus;
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
	if(!CheckError(iStatus, _T("建立OpenGL-OpenCL共享context"))) return FALSE;

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
		return FALSE;
	}
	systemLog->PrintStatus(_T("编译device端程序——成功！"));

	m_BitonicSortKernel = clCreateKernel(m_Program, "BitonicSort", &iStatus);
	if(!CheckError(iStatus, _T("获取BitonicSortKernel"))) return FALSE;
	m_CommSAHSplitKernel = clCreateKernel(m_Program, "SAHSplit", &iStatus);
	if (!CheckError(iStatus, _T("获取SAHSplitKernel")))return FALSE;
	m_SAASAHSplitKernel = clCreateKernel(m_Program, "SAASAHSplit", &iStatus);
	if (!CheckError(iStatus, _T("获取SAASAHSplitKernel"))) return FALSE;
	m_PSOSAHSplitKernel = clCreateKernel(m_Program, "PSOSAHSplit", &iStatus);
	if (!CheckError(iStatus, _T("获取PSOSAHSplitKernel"))) return FALSE;
	m_RayTraceKernel = clCreateKernel(m_Program, "RayTrace", &iStatus);
	if (!CheckError(iStatus, _T("获取RayTraceKernel"))) return FALSE;
	
	//默认设置
	m_SAHSplitKernel = m_CommSAHSplitKernel;

	m_Queue = clCreateCommandQueue(m_Context, m_HardwareInfo->platformInfo[m_ConfigInfo->selPlaformIndex].deviceIDs[m_ConfigInfo->selDeviceIndex], CL_QUEUE_PROFILING_ENABLE, &iStatus);
	if (!CheckError(iStatus, _T("创建设备命令队列"))) return FALSE;


	systemLog->PrintStatus(_T("\r\n运算环境初始化——成功！\r\n"));
	m_ContextReady = true;
	return TRUE;
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

	m_DrawInfo = new DrawableInfo;
	m_DrawInfo = getTriangles(&m_ModleInfo->verts[0], &m_ModleInfo->normals[0], m_ModleInfo->verts.size());

	m_RenderType = OFFLINE_RENDERING;
	BitonicSort();
	BuildKDTree();
	CalPBO();

	systemLog->PrintStatus(_T("\r\n离线渲染成功！\r\n"));
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

	m_RenderType = REALTIME_RENDERING;
	BitonicSort();
	BuildKDTree();
	CalPBO();

	systemLog->PrintStatus(_T("\r\n实时渲染成功！\r\n"));
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

void COpenCLCompute::BitonicSort()
{
	cl_int iStatus;
	DWORD dBegTime = GetTickCount();

	int len = m_DrawInfo->triangleCandidateSplitPlaneArray.size();
	m_InputInfo = m_DrawInfo->triangleCandidateSplitPlaneArray;
	fillTo2PowerScale(m_InputInfo);

	m_InputInfoMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE, sizeof(TriangleCandidateSplitPlane)*m_InputInfo.size(), &m_InputInfo[0], &iStatus);
	CheckError(iStatus, _T("clCreateBuffer of InputInfoMem"));

	int dir = 1;
	cl_mem dirMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &dir, &iStatus);
	CheckError(iStatus, _T("clCreateBuffer of dirMem"));

	clSetKernelArg(m_BitonicSortKernel, 0, sizeof(cl_mem), &m_InputInfoMem);
	clSetKernelArg(m_BitonicSortKernel, 3, sizeof(cl_mem), &dirMem);

	int ceil = m_InputInfo.size();
	for (int i= 2; i<= ceil; i<<=1)
	{
		for (int j = i; j > 1; j>>=1)
		{
			int groupSize = ceil/j;
			int flip = (j==i?1:-1);
			const size_t global = 128;
			const size_t local = 32;

			cl_mem groupSizeMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &groupSize, &iStatus);
			cl_mem lengthMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &j, &iStatus);
			cl_mem flipMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &flip, &iStatus);
			clSetKernelArg(m_BitonicSortKernel, 1, sizeof(cl_mem), &groupSizeMem);
			clSetKernelArg(m_BitonicSortKernel, 2, sizeof(cl_mem), &lengthMem);
			clSetKernelArg(m_BitonicSortKernel, 4, sizeof(cl_mem), &flipMem);

			iStatus = clEnqueueNDRangeKernel(m_Queue, m_BitonicSortKernel, 1, 0, &global, &local, 0, 0, 0);
			//if (!CheckError(iStatus, _T("clEnqueueNDRangeKernel"))) return;
			if ( CL_SUCCESS != iStatus) return;
			clReleaseMemObject(groupSizeMem);
			clReleaseMemObject(flipMem);
			clReleaseMemObject(lengthMem);
		}
	}
	CheckError(iStatus, _T("clEnqueueNDRangeKernel of BitonicSort"));
	clFinish(m_Queue);

	clReleaseMemObject(dirMem);
	DWORD dEndTime = GetTickCount();
	DWORD dSortTime = dEndTime - dBegTime;
	auto sSortTime = TToStr(dSortTime);

	//测试代码
	/*std::vector<TriangleCandidateSplitPlane> test(m_InputInfo.size());
	iStatus = clEnqueueReadBuffer(m_Queue, m_InputInfoMem, CL_TRUE, 0, m_InputInfo.size()*sizeof(TriangleCandidateSplitPlane), &test[0], 0, NULL, NULL);
	CheckError(iStatus, _T("read buffer of inputInfoMem"));
	clFinish(m_Queue);*/
	CString info = _T("\r\n排序完成！	--花费时间:") + StrToCStr(sSortTime) + _T("(ms)\r\n");
	systemLog->PrintStatus(info.GetBuffer());
}

void COpenCLCompute::BuildKDTree()
{
	cl_int iStatus;
	DWORD buildTreeBeg = GetTickCount();

	clSetKernelArg(m_SAHSplitKernel, 0, sizeof(cl_mem), &m_InputInfoMem);
	int maxSplitNodeArrayLength = GetNodeArrayMaxLength(m_InputInfo.size());
	SplitNode originSplitNode;
	InitialSplitNode(&originSplitNode);
	std::vector<SplitNode> splitNodeArray(maxSplitNodeArrayLength, originSplitNode);
	SplitNode firstSplitNode;
	firstSplitNode.beg = 0;
	firstSplitNode.end = m_DrawInfo->triangleInfoArray.size()-1;
	firstSplitNode.leftChild = -1;
	firstSplitNode.rightChild = -1;
	firstSplitNode.xMax = m_InputInfo[0].xMax;
	firstSplitNode.xMin = m_InputInfo[0].xMin;
	firstSplitNode.yMax = m_InputInfo[0].yMax;
	firstSplitNode.yMin = m_InputInfo[0].yMin;
	firstSplitNode.zMax = m_InputInfo[0].zMax;
	firstSplitNode.zMin = m_InputInfo[0].zMin;
	splitNodeArray[0] = firstSplitNode;

	cl_mem maxSizeMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &maxSplitNodeArrayLength, &iStatus);
	CheckError(iStatus, _T("create buffer of MaxSize"));
	//iStatus = clSetKernelArg(m_SAHSplitKernel, 6, sizeof(cl_mem), &maxSizeMem);

	if ( SAHSPLIT_COMM == m_ConfigInfo->SAHType )
	{
		iStatus = clSetKernelArg(m_SAHSplitKernel, 4, sizeof(cl_mem), &maxSizeMem);
		m_SplitNodeArrayMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE, sizeof(SplitNode)*maxSplitNodeArrayLength, &splitNodeArray[0], &iStatus);
		CheckError(iStatus, _T("create buffer of CommSplitNodeArrayMem"));

		clSetKernelArg(m_SAHSplitKernel, 1, sizeof(cl_mem), &m_SplitNodeArrayMem);
		clSetKernelArg(m_SAHSplitKernel, 4, sizeof(cl_mem), &maxSizeMem);

		int depth = 0;
		for (int i= 1; (i < log((float) maxSplitNodeArrayLength)/log(2.0)) && (depth < m_ConfigInfo->maxKDTreeDepth); i++)
		{
			int splitNodeArrayBeg = pow(2.0, i-1) - 1;
			int splitNodeArrayEnd = pow(2.0, i) - 2;
			size_t layerLength = splitNodeArrayEnd - splitNodeArrayBeg + 1;
			const size_t globalSize = layerLength;
			const size_t localSize = (globalSize <= 64)? globalSize : 64;
			cl_mem splitNodeArrayBegMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &splitNodeArrayBeg, &iStatus);
			CheckError(iStatus, _T("create buffer of SplitNodeArrayBegMem"));
			cl_mem splitNodeArrayEndMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &splitNodeArrayEnd, &iStatus);
			CheckError(iStatus, _T("create buffer of SplitNodeArrayEndMem"));
			iStatus = clSetKernelArg(m_SAHSplitKernel, 2, sizeof(cl_mem), &splitNodeArrayBegMem);
			iStatus = clSetKernelArg(m_SAHSplitKernel, 3, sizeof(cl_mem), &splitNodeArrayEndMem);

			iStatus = clEnqueueNDRangeKernel(m_Queue, m_SAHSplitKernel, 1, 0, &globalSize, &localSize, 0, NULL, NULL);
			CheckError(iStatus, _T("clEnqueueNDRangeKernel of SAHSplitKernel_Common"));

			clReleaseMemObject(splitNodeArrayBegMem);
			clReleaseMemObject(splitNodeArrayEndMem);
			depth++;
		}
		clFinish(m_Queue);

		std::vector<SplitNode> test(splitNodeArray.size());
		iStatus = clEnqueueReadBuffer(m_Queue, m_SplitNodeArrayMem, CL_TRUE, 0, maxSplitNodeArrayLength*sizeof(SplitNode), &test[0], 0, NULL, NULL);
		CheckError(iStatus, _T("read buffer of inputInfoMem"));
		clFinish(m_Queue);
	}
	else if ( SAHSPLIT_SAA == m_ConfigInfo->SAHType | SAHSPLIT_PSO == m_ConfigInfo->SAHType)
	{
		m_SplitNodeArrayMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(SplitNode)*maxSplitNodeArrayLength, &splitNodeArray[0], &iStatus);
		CheckError(iStatus, _T("clCreateBuffer of SplitNodeArray"));
		
		clSetKernelArg(m_SAHSplitKernel, 1, sizeof(cl_mem), &m_SplitNodeArrayMem);

		int maxLayerLenght = getMin2Power(m_InputInfo.size());
std::vector<int> randArray(maxLayerLenght);
		for (int i = 0; i < maxLayerLenght; i++)
		{
			randArray[i] = rand() % maxLayerLenght;
		}
		cl_mem randArrayMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int)*maxLayerLenght, &randArray[0], &iStatus);
		clSetKernelArg(m_SAHSplitKernel, 4, sizeof(cl_mem), &randArrayMem);

		std::vector<float> randPro((m_ConfigInfo->originTmp)*(m_ConfigInfo->PSOSampleCount));
		for (int i = 0; i < randPro.size(); i++)
		{
			randPro[i] = (rand()%10)/10.0;
		}

		cl_mem randProMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float)*randPro.size(), &randPro[0], &iStatus);
		clSetKernelArg(m_SAHSplitKernel, 5, sizeof(cl_mem), &randProMem);

		int depth = 0;
		for (int i= 1; (i < log((float) maxSplitNodeArrayLength)/log(2.0)) && (depth < m_ConfigInfo->maxKDTreeDepth); i++)
		{
			int splitNodeArrayBeg = pow(2.0, i-1) - 1;
			int splitNodeArrayEnd = pow(2.0, i) - 2;
			size_t layerLength = splitNodeArrayEnd - splitNodeArrayBeg + 1;
			const size_t globalSize = layerLength;
			const size_t localSize = 64;
			cl_mem splitNodeArrayBegMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &splitNodeArrayBeg, &iStatus);
			CheckError(iStatus, _T("create buffer of SplitNodeArrayBegMem"));
			cl_mem splitNodeArrayEndMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), &splitNodeArrayEnd, &iStatus);
			CheckError(iStatus, _T("create buffer of SplitNodeArrayEndMem"));
			clSetKernelArg(m_SAHSplitKernel, 2, sizeof(cl_mem), &splitNodeArrayBegMem);
			clSetKernelArg(m_SAHSplitKernel, 3, sizeof(cl_mem), &splitNodeArrayEndMem);

			iStatus = clEnqueueNDRangeKernel(m_Queue, m_SAHSplitKernel, 1, 0, &globalSize, &localSize, 0, NULL, NULL);
			CheckError(iStatus, _T("EnqueueNDRangeKernel of SAHSplit_NONCOMMON"));
			clReleaseMemObject(splitNodeArrayBegMem);
			clReleaseMemObject(splitNodeArrayEndMem);
			depth++;
		}
		clFinish(m_Queue);
		clReleaseMemObject(randProMem);
		clReleaseMemObject(randArrayMem);
	}

	DWORD buildTreeEnd = GetTickCount();
	DWORD buildTime = buildTreeEnd - buildTreeBeg;
	auto sBuildTime = TToStr(buildTime);
	CString info = _T("\r\n构建KD-Tree完成！	--花费时间:") + StrToCStr(sBuildTime) + _T("(ms)\r\n"); 
	systemLog->PrintStatus(info.GetBuffer());
}

BOOL COpenCLCompute::CalPBO()
{
	cl_int iStatus;
	DWORD calBeg = GetTickCount();
	//计算PBO_Mem
	if ( OFFLINE_RENDERING == m_RenderType )
	{
		cl_mem cmWinWidthMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), (void*) &ciRenderWinWidth, &iStatus);
		CheckError(iStatus, _T("clCreateBuffer of cmWinWidthMem"));

		cl_mem cmWinHeightMem = clCreateBuffer(m_Context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(int), (void*) &ciRenderWinHeight, &iStatus);
		CheckError(iStatus, _T("clCreateBuffer of cmWinHeightMem"));

		iStatus = clSetKernelArg(m_RayTraceKernel, 0, sizeof(cl_mem), &m_SplitNodeArrayMem);
		iStatus = clSetKernelArg(m_RayTraceKernel, 1, sizeof(cl_mem), &cmWinWidthMem);
		iStatus = clSetKernelArg(m_RayTraceKernel, 2, sizeof(cl_mem), &cmWinHeightMem);

		m_PBOMem = clCreateFromGLBuffer(m_Context, CL_MEM_WRITE_ONLY, m_PBO, &iStatus);
		CheckError(iStatus, _T("clCreateFromGLBuffer of PBOMem"));
		clSetKernelArg(m_RayTraceKernel, 3, sizeof(cl_mem), &m_PBOMem);

		std::vector<TriangleInfo> svTriangleInfo = m_DrawInfo->triangleInfoArray;
		cl_mem clmTriangleInfoMem = clCreateBuffer(m_Context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(TriangleInfo)*m_DrawInfo->triangleInfoArray.size(), &svTriangleInfo[0], &iStatus);
		iStatus = clSetKernelArg(m_RayTraceKernel, 4, sizeof(cl_mem), &clmTriangleInfoMem);
		iStatus = clSetKernelArg(m_RayTraceKernel, 5, sizeof(cl_mem), &m_InputInfoMem);

		cl_int cliLength = GetNodeArrayMaxLength(m_InputInfo.size());
		cl_mem lengthMem = clCreateBuffer(m_Context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &cliLength, &iStatus);
		iStatus = clSetKernelArg(m_RayTraceKernel, 6, sizeof(cl_mem), &lengthMem);

		int posX = ciRenderWinWidth/2;
		int posY = ciRenderWinHeight/2;
		cl_mem posXMem = clCreateBuffer(m_Context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &posX, &iStatus);
		cl_mem posYMem = clCreateBuffer(m_Context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &posY, &iStatus);
		iStatus = clSetKernelArg(m_RayTraceKernel, 7, sizeof(cl_mem), &posXMem);
		iStatus = clSetKernelArg(m_RayTraceKernel, 8, sizeof(cl_mem), &posYMem);

		glFinish();
		iStatus = clEnqueueAcquireGLObjects(m_Queue, 1, &m_PBOMem, 0, NULL, NULL);
		CheckError(iStatus, _T("获取GL环境下的PBO控制权"));
		iStatus = clEnqueueNDRangeKernel(m_Queue, m_RayTraceKernel, 2, NULL, &pixelGlobalGroup[0], &pixelLocalGroup[0], 0, NULL, NULL);
		CheckError(iStatus, _T("计算PBO"));
		clFinish(m_Queue);
		iStatus = clEnqueueReleaseGLObjects(m_Queue, 1, &m_PBOMem, 0, NULL, NULL);
		CheckError(iStatus, _T("释放CL环境下的PBO控制权"));
		clFinish(m_Queue);

		//clReleaseMemObject(cmWinWidthMem);
		//clReleaseMemObject(cmWinHeightMem);
		//clReleaseMemObject(m_PBOMem);
		//clReleaseMemObject(clmTriangleInfoMem);
		//clReleaseMemObject(m_InputInfoMem);
		//clReleaseMemObject(lengthMem);
		
		/*m_PBOMem = clCreateFromGLBuffer(m_Context, CL_MEM_WRITE_ONLY, m_PBO, &iStatus);
		CheckError(iStatus, _T("clCreateFromBuffer of PBOMem "));
		iStatus = clSetKernelArg(m_RayTraceKernel, 0, sizeof(cl_mem), &m_PBOMem);

		glFinish();
		iStatus = clEnqueueAcquireGLObjects(m_Queue, 1, &m_PBOMem, 0, NULL, NULL);
		CheckError(iStatus, _T("获取GL环境下的PBO控制权"));
		iStatus = clEnqueueNDRangeKernel(m_Queue, m_RayTraceKernel, 2, NULL, &pixelGlobalGroup[0], &pixelLocalGroup[0], 0, NULL, NULL);
		CheckError(iStatus, _T("计算PBO"));
		iStatus = clFinish(m_Queue);
		iStatus = clEnqueueReleaseGLObjects(m_Queue, 1, &m_PBOMem, 0, NULL, NULL);
		CheckError(iStatus, _T("释放CL环境下的PBO控制权"));
		iStatus = clFinish(m_Queue);*/
		
	}
	else if ( REALTIME_RENDERING == m_RenderType)
	{

	}

	DWORD calEnd = GetTickCount();
	CString info = _T("\r\n渲染成功！	--花费时间为: ") + StrToCStr(TToStr(calEnd - calBeg)) + _T("(ms)\r\n");
	systemLog->PrintStatus(info.GetBuffer());
	return TRUE;
	//测试代码
	/*cl_mem m_testMem = clCreateBuffer(m_Context, CL_MEM_READ_WRITE, 4*ciRenderWinHeight*ciRenderWinWidth*sizeof(unsigned char), 0, &iStatus);
	CheckError(iStatus,_T("test pre！"));
	iStatus = clSetKernelArg(m_RayTraceKernel, 0, sizeof(cl_mem), &m_testMem);
	CheckError(iStatus, _T("test set!"));
	iStatus = clEnqueueNDRangeKernel(m_Queue, m_RayTraceKernel, 2, NULL, &pixelGlobalGroup[0], &pixelLocalGroup[0], 0, NULL, NULL);
	clFinish(m_Queue);
	CheckError(iStatus, _T("test cal !"));

	std::vector<unsigned char> res(4*ciRenderWinHeight*ciRenderWinWidth);
	iStatus = clEnqueueReadBuffer(m_Queue, m_testMem, CL_TRUE, 0, 4*ciRenderWinHeight*ciRenderWinWidth*sizeof(unsigned char), &res[0], 0, NULL, NULL);*/
	
	/*std::vector<unsigned char> color(4*ciRenderWinWidth*ciRenderWinHeight);
	for (auto itr = color.begin(); itr != color.end();)
	{
		*itr = 255;
		itr++;
		*itr = 0;
		itr++;
		*itr = 0;
		itr++;
		*itr = 255;
		itr++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_PBO);
	glBufferData(GL_ARRAY_BUFFER, 4*ciRenderWinHeight*ciRenderWinWidth*sizeof(unsigned char), &color[0], GL_STATIC_DRAW);*/
}

void COpenCLCompute::SetModelInfo(ModelInfo* info)
{
	m_ModleInfo = info;
}

BOOL COpenCLCompute::SetSAASAHParam(int originTem, int thresholdTem , int singleSampleCount, float desSpeed)
{
	if (originTem <= thresholdTem | singleSampleCount <=0 | desSpeed<=0 | originTem <= 0 | thresholdTem <= 0)
	{
		CheckError(!CL_SUCCESS, _T("SAA_SAH参数设置"));
		return FALSE;
	}
	m_ConfigInfo->SAHType = SAHSPLIT_PSO;
	m_ConfigInfo->originTmp = originTem;
	m_ConfigInfo->SAASingleSampleCount = singleSampleCount;
	m_ConfigInfo->descSpeed = desSpeed;
	return TRUE;
}

BOOL COpenCLCompute::SetPSOSAHParam(int particleNum, int sampleCount, float inertiaWeight, float c1Weight, float c2Weight, float singleMaxShift)
{
	if( particleNum <= 0 | sampleCount <= 0 | inertiaWeight < 0 | c1Weight < 0 | c2Weight < 0 | singleMaxShift <= 0)
	{
		CheckError(!CL_SUCCESS, _T("PSO_SAH参数设置"));
		return FALSE;
	}
	m_ConfigInfo->SAHType = SAHSPLIT_SAA;
	m_ConfigInfo->particleNum = particleNum;
	m_ConfigInfo->PSOSampleCount = sampleCount;
	m_ConfigInfo->inertiaWeight = inertiaWeight;
	m_ConfigInfo->c1Weight = c1Weight;
	m_ConfigInfo->c2Weight = c2Weight;
	m_ConfigInfo->singleMaxShift = singleMaxShift;
	return TRUE;
}

ConfigInfo* COpenCLCompute::GetConfigInfo()
{
	return m_ConfigInfo;
}