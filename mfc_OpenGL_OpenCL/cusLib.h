#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdarg.h>

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
extern std::vector<size_t> pixelGlobalGroup;
extern std::vector<size_t> pixelLocalGroup;

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
	std::vector<size_t> workGlobalGroup;
	std::vector<size_t> workLocalGroup;
	int	reflectCount;
	std::vector<float> viewPos;
	std::vector<float> lightPos;

	int maxKDTreeDepth;

	int	SAHType;
	float originTmp;
	float SAASingleSampleCount;
	float descSpeed;
	float particleNum;
	float PSOSampleCount;
	float inertiaWeight;
	float c1Weight;
	float c2Weight;
	float singleMaxShift;

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


//用来记录三家片相关信息的结构体
struct TriangleInfo
{
	int triangleID;

	glm::vec3 vecInfo[3];

	glm::vec3 vecNormal;

	float GetXmin();
	float GetXmax();
	float GetYmin();
	float GetYmax();
	float GetZmin();
	float GetZmax();
	

};

//用来记录三角面片ID以及候选分隔平面的结构体
struct TriangleCandidateSplitPlane
{
	int triangleID;
	float xMin;
	float yMin;
	float zMin;
	float xMax;
	float yMax;
	float zMax;
	
};

//用来记录一个结点对应三角面片跟AABB等信息的结构体
struct DrawableInfo
{
	/*std::vector<osg::ref_ptr<osg::Geode>> res_TP;
	std::vector<osg::ref_ptr<osg::Geode>> res_AABB;*/
	std::vector<TriangleInfo> triangleInfoArray;
	std::vector<TriangleCandidateSplitPlane> triangleCandidateSplitPlaneArray;
	glm::vec3* vertexList;
};


//分割节点
struct SplitNode
{
	int beg;
	int end;
	int leftChild;
	int rightChild;

	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;

};

//初始化分割节点
void InitialSplitNode(struct SplitNode *mNode);

void GetTraingleInfo(glm::vec3* p1, glm::vec3* p2, glm::vec3* p3, glm::vec3* nor, int ID, TriangleInfo* res);

//将顶点信息划分为三角片信息
DrawableInfo* getTriangles(glm::vec3* v3Vertex, glm::vec3* v3Normal, int iVerLen);


//获取比m大的最小的2次幂数
int getMin2Power(int m);

//寻找最大值
TriangleCandidateSplitPlane findMax(std::vector<TriangleCandidateSplitPlane> input);

//最大值填充
void fillTo2PowerScale(std::vector<TriangleCandidateSplitPlane> &input);





//获取比childnNodeNum大的最小的以2为底的幂
int GetNodeArrayMaxLength(int childNodeNum);

//辅助函数
std::string CStrToStr(CString& str);
CString	StrToCStr(std::string str);
float	StrToFloat(std::string& str);

