#include "cusLib.h"

CString statusInfo;

std::vector<size_t> pixelGlobalGroup;
std::vector<size_t> pixelLocalGroup;


std::string CStrToStr(CString& str)
{
	wchar_t* wText = str.GetBuffer();
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);
	char *psText;
	psText = new char[dwNum];
	if(!psText)
	{
		delete []psText;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);
	return std::string(psText);
}

CString	StrToCStr(std::string str)
{
	std::wstringstream cs;
	cs<<str.c_str();
	return CString(cs.str().c_str());
}

float StrToFloat(std::string& str)
{
	float res;
	std::stringstream strm(str.c_str());
	strm.setf(std::ios::fixed);
	strm>>res;
	return res;
}

float TriangleInfo::GetXmin()
{
	float xTmp = vecInfo[0].x;
	if(vecInfo[1].x < xTmp) 
	{	
		xTmp = vecInfo[1].x;
	}
	if (vecInfo[2].x < xTmp)
	{
		xTmp = vecInfo[2].x;
	}

	return xTmp;
}

float TriangleInfo::GetXmax()
{
	float xTmp = vecInfo[0].x;
	if(vecInfo[1].x > xTmp) 
	{	
		xTmp = vecInfo[1].x;
	}
	if (vecInfo[2].x > xTmp)
	{
		xTmp = vecInfo[2].x;
	}

	return xTmp;
}

float TriangleInfo::GetYmin()
{
	float yTmp = vecInfo[0].y;
	if(vecInfo[1].y < yTmp) 
	{	
		yTmp = vecInfo[1].y;
	}
	if (vecInfo[2].y < yTmp)
	{
		yTmp = vecInfo[2].y;
	}

	return yTmp;
}

float TriangleInfo::GetYmax()
{
	float yTmp = vecInfo[0].y;
	if(vecInfo[1].y > yTmp) 
	{	
		yTmp = vecInfo[1].y;
	}
	if (vecInfo[2].y > yTmp)
	{
		yTmp = vecInfo[2].y;
	}

	return yTmp;
}

float TriangleInfo::GetZmin()
{
	float zTmp = vecInfo[0].z;
	if(vecInfo[1].z < zTmp) 
	{	
		zTmp = vecInfo[1].z;
	}
	if (vecInfo[2].z < zTmp)
	{
		zTmp = vecInfo[2].z;
	}

	return zTmp;
}

float TriangleInfo::GetZmax()
{
	float zTmp = vecInfo[0].z;
	if(vecInfo[1].z > zTmp) 
	{	
		zTmp = vecInfo[1].z;
	}
	if (vecInfo[2].z > zTmp)
	{
		zTmp = vecInfo[2].z;
	}

	return zTmp;
}

void GetTraingleInfo(glm::vec3* p1, glm::vec3* p2, glm::vec3* p3, glm::vec3* nor, int ID, TriangleInfo* res)
{
	res->vecInfo[0] = *p1;
	res->vecInfo[1] = *p2;
	res->vecInfo[2] = *p3;

	res->vecNormal = *nor;

	res->triangleID = ID;
}

DrawableInfo* getTriangles(glm::vec3* v3Vertex, glm::vec3* v3Normal, int iVerLen)
{
	int ID = 0;
	TriangleInfo* resTrianglesInfo = new TriangleInfo;
	TriangleCandidateSplitPlane* resTriangleCandidateSplitPlane = new TriangleCandidateSplitPlane;
	DrawableInfo* res = new DrawableInfo;

	glm::vec3* p1 = new glm::vec3;
	glm::vec3* p2 = new glm::vec3;
	glm::vec3* p3 = new glm::vec3;
	glm::vec3* nor = new glm::vec3;
	res->vertexList = v3Vertex;

	int i = 1;
	for(int j=0 ; j<iVerLen; j++)
	{
		switch(i)
		{
		case 1:
			p1 = new glm::vec3;
			p1 = &v3Vertex[j];
			break;
		case 2:
			p2 = new glm::vec3;
			p2 = &v3Vertex[j];

			break;
		case 3:
			p3 = new glm::vec3;
			p3 = &v3Vertex[j];

			nor = new glm::vec3;
			nor = &v3Normal[j];

			//	std::cout<<"================================"<<std::endl;
			resTrianglesInfo = new TriangleInfo;
			resTriangleCandidateSplitPlane = new TriangleCandidateSplitPlane;
			GetTraingleInfo(p1, p2, p3, nor, ID, resTrianglesInfo);
			resTriangleCandidateSplitPlane->triangleID = ID;
			ID++;
			resTriangleCandidateSplitPlane->xMin = resTrianglesInfo->GetXmin();
			resTriangleCandidateSplitPlane->yMin = resTrianglesInfo->GetYmin();
			resTriangleCandidateSplitPlane->zMin = resTrianglesInfo->GetZmin();
			resTriangleCandidateSplitPlane->xMax = resTrianglesInfo->GetXmax();
			resTriangleCandidateSplitPlane->yMax = resTrianglesInfo->GetYmax();
			resTriangleCandidateSplitPlane->zMax = resTrianglesInfo->GetZmax();
			res->triangleInfoArray.push_back(*resTrianglesInfo);
			res->triangleCandidateSplitPlaneArray.push_back(*resTriangleCandidateSplitPlane);
			//	std::cout<<std::endl;
			i = 0;
			break;
		default:
			break;
		}



		i++;
	}
	return res;
}

int getMin2Power(int m)
{
	int res = 1;
	while (res < m)
	{
		res<<=1;
	}
	return res;
}

TriangleCandidateSplitPlane findMax(std::vector<TriangleCandidateSplitPlane> input)
{
	TriangleCandidateSplitPlane res = input[0];
	for (auto it = input.begin(); it < input.end(); it++)
	{
		if ( it->xMin > res.xMin)
		{
			res = *it;
		}
	}
	return res;
}

void fillTo2PowerScale(std::vector<TriangleCandidateSplitPlane> &input)
{
	int length = getMin2Power(input.size());
	TriangleCandidateSplitPlane max = findMax(input);

	for (int i = input.size(); i < length; i++)
	{
		input.push_back(max);
	}

}

void InitialSplitNode(struct SplitNode *mNode)
{
	mNode->beg = -1;
	mNode->end = -1;
	mNode->leftChild = -1;
	mNode->rightChild = -1;
}

int GetNodeArrayMaxLength(int childNodeNum)
{
	int layerNum = log((float) childNodeNum) / log(2.0) + 1;
	int maxNodeNum = pow(2.0, layerNum) - 1;
	return maxNodeNum;
}