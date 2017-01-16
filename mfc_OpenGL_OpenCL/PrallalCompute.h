#pragma once

#include "cusLib.h"


class CPraCompute
{
public:
	virtual HardwareInfo* GetHardwareInfo() = 0;
	virtual void SetCalDevice(int platformID = 0, int deviceID = 0) = 0;
	virtual void InitContext()= 0;
	virtual void SetGLCLShared(bool flag = false) = 0;
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

	virtual void SetCalDevice(int platformID = 0 , int deviceID  = 0 );
	virtual void InitContext();
	virtual void SetGLCLShared(bool flag = false);
	
	
private:
	
	HardwareInfo*	m_HardwareInfo;
	cl_context		m_context;
	bool			m_GLCLShared;
	static COpenCLCompute* m_instance;

};


