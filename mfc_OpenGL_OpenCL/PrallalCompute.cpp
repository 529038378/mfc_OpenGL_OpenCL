#include "PrallalCompute.h"


COpenCLCompute* COpenCLCompute::m_instance = NULL;
COpenCLCompute::COpenCLCompute()
{
	m_HardwareInfo = new HardwareInfo;
	m_GLCLShared = false;
	m_context = NULL;
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

}

void COpenCLCompute::SetCalDevice(int platformID  , int deviceID  )
{

}

void COpenCLCompute::SetGLCLShared(bool flag /* = false */)
{
	m_GLCLShared = flag;
}