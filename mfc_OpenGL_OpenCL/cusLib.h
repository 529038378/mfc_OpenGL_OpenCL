#pragma once
#include <map>
#include <vector>
#include <string>
#include <CL/cl.h>
#include <cl/cl_gl.h>

#include "afx.h"

struct DeviceInfo
{
	std::string		name;
	cl_device_id	id;
	cl_uint			bitWidth;
	cl_ulong		globalMemSize;
	cl_ulong		localMemSize;
	cl_uint			maxClockFreq;
	cl_uint			maxComputeUnits;
	size_t			maxWorkGroupSize;
	std::vector<size_t>	maxWorkItemSize;
	cl_device_type	type;
	cl_uint			maxDimension;
	std::string		version;
};

struct PlatformInfo
{
	cl_platform_id	id;
	std::string		name;
	std::string		venderName;
	std::string		profile;
	cl_uint			deviceNum;
	std::vector<cl_device_id> deviceIDs;
	std::vector<DeviceInfo> deviceInfo; 
};

struct HardwareInfo
{
	std::vector<PlatformInfo> platformInfo;
	std::vector<cl_platform_id> platformIDs;
	std::vector<DeviceInfo> deviceInfo;
};

template<typename T>
std::string TtoStr(T in)
{
	std::ostringstream os;
	os<<in;
	std::string result;
	std::istringstream is(os.str());
	is>>result;
	return result;
}
