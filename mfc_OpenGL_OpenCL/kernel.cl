__kernel void BitonicSort()
{

}

__kernel void SAHSplit()
{

}

__kernel void PSOSAHSplit()
{

}

__kernel void SAASAHSplit()
{

}

__kernel void RayTrace(__global uchar* pRes)
{
	int idxX = get_global_id(0);
	int idxY = get_global_id(1);
	pRes[(idxY*1024+idxX)*4] = 255;
	pRes[(idxY*1024+idxX)*4+1] = 255;
	pRes[(idxY*1024+idxX)*4+2] = 255;
	pRes[(idxY*1024+idxX)*4+3] = 255;
}