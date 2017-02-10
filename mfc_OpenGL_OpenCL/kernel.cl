

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



//用于矩阵的运算，转换坐标系用
struct mat4
{
	float4 l0;
	float4 l1;
	float4 l2;
	float4 l3;

	
};


float4  mul(struct mat4 m4, float4 l)
{
	float4 res;
	res.x = m4.l0.x*l.x + m4.l0.y*l.y + m4.l0.z*l.z + m4.l0.w*l.w;
	res.y = m4.l1.x*l.x + m4.l1.y*l.y + m4.l1.z*l.z + m4.l1.w*l.w;
	res.z = m4.l2.x*l.x + m4.l2.y*l.y + m4.l2.z*l.z + m4.l2.w*l.w;
	res.w = m4.l3.x*l.x + m4.l3.y*l.y + m4.l3.z*l.z + m4.l3.w*l.w;

	return res;
		
};

__kernel void BitonicSort (__global struct TriangleCandidateSplitPlane* input,  
                         __global const int* group,  
                         __global const int* length,  
                         __global const int* dir,  
                         __global const int* flip)  
{  
    int bid = get_group_id(0);  
    int groupsInKernel = get_num_groups(0);  
    int wid = get_local_id(0);  
    int itemsInGroup = get_local_size(0);  
      
    for(int gpos = bid; gpos < (*group); gpos += groupsInKernel)
	{  
        for(int pos = wid; pos < (*length)/2; pos += itemsInGroup)
		{  
            int begin = gpos * (*length);  
            int delta;  
            if ((*flip) == 1)   delta = (*length) - 1;  
            else                delta = (*length)/2;  
              
            int a = begin + pos;  
            int b = begin + delta - (*flip) * pos;  
              
            if ( (*dir) == (input[a].xMin > input[b].xMin) )
			{  
                struct TriangleCandidateSplitPlane temp = input[a];  
                input[a] = input[b];  
                input[b] = temp;  
            }  
        }  
    }  
}



__kernel void SAHSplit(__global uchar* pRes)
{
	int idxX = get_global_id(0);
	int idxY = get_global_id(1);
	pRes[(idxY*1024+idxX)*4] = 255;
	pRes[(idxY*1024+idxX)*4+1] = 255;
	pRes[(idxY*1024+idxX)*4+2] = 255;
	pRes[(idxY*1024+idxX)*4+3] = 255;
}

__kernel void PSOSAHSplit()
{

}

__kernel void SAASAHSplit()
{

}

/*__kernel void RayTrace(__global struct SplitNode* nodeArray, __global int* width, __global int* height, __global unsigned char* pRes, __global struct TriangleInfo* tinfo, __global struct TriangleCandidateSplitPlane* tcsArray, __global int* len)
{
	int idxX = get_global_id(0);
	int idxY = get_global_id(1);
	pRes[(idxY*768+idxX)*4] = 0;
	pRes[(idxY*768+idxX)*4+1] = 255;
	pRes[(idxY*768+idxX)*4+2] = 0;
	pRes[(idxY*768+idxX)*4+3] = 255;
}*/

__kernel void RayTrace(__global unsigned char* pRes)
{
	int idxX = get_global_id(0);
	int idxY = get_global_id(1);
	pRes[(idxY*768+idxX)*4] = 0;
	pRes[(idxY*768+idxX)*4+1] = 255;
	pRes[(idxY*768+idxX)*4+2] = 0;
	pRes[(idxY*768+idxX)*4+3] = 255;
}