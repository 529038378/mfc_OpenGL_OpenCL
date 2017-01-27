#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <gl/glew.h>
#include <gl/glu.h>

#include <CL/cl.h>
#include <cl/cl_gl.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/objloader.hpp>
#include <common/shader.hpp>

#include "afx.h"

const int ciRenderWinWidth = 1024;
const int ciRenderWinHeight = 768;

enum {
	SAHSPLIT_COMM	=	0x2000000,
	SAHSPLIT_SAA	=	0x2000001,
	SAHSPLIT_PSO	=	0x2000002,
};

static const GLfloat cfQuadVertBuffer[] = 
{
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};

static const GLfloat UV[] = 
{ 
	0.0f, 0.0f, 
	1.0f, 0.0f, 
	0.0f,  1.0f, 
	0.0f,  1.0f, 
	1.0f, 0.0f, 
	1.0f,  1.0f, 
};

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

struct ModelInfo
{
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
};

struct ConfigInfo
{
	int	selPlaformIndex;
	int	selDeviceIndex;
	std::vector<int> workGroup;
	int	reflectCount;
	std::vector<float> viewPos;
	std::vector<float> lightPos;
};

template<typename T>
std::string TToStr(T in)
{
	std::ostringstream os;
	os<<in;
	std::string result;
	std::istringstream is(os.str());
	is>>result;
	return result;
}

std::string CStrToStr(CString& str);
CString	StrToCStr(std::string str);
float	StrToFloat(std::string& str);