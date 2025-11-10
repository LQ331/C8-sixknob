#include "Device.h"
#include "LClassLib.h"

Device::Device()
{
}


Device::~Device()
{
}


const DeviceParam & Device::GetParam(bool bIsCurrent)
{
	if(bIsCurrent)
		return m_dp;
	else
		return m_dpBackup;
}


void Device::SetParam(const DeviceParam & dp,bool bIsCurrent)
{
	if(bIsCurrent)
		m_dp=dp;
	else
		m_dpBackup=dp;
}


void Device::ParamBackup()
{
	m_dpBackup=m_dp;
}


void Device::ParamRestore()
{
	m_dp=m_dpBackup;
}

void Device::EraseWriteParam()
{
		g_flash.Erase();
		uint16_t F = g_emFrameCurrent;
		g_flash.write(EE_FrameCurrent,(uint16_t *)&F,sizeof(F)/2);
		g_flash.write(EE_Version,(uint16_t *)&g_u32Version,sizeof(g_u32Version)/2);
		g_device.WriteParam();
		g_coef.write(g_flash,EE_CoefStart);
		g_device.ParamBackup();
}

bool Device::WriteParam()
{
	return g_flash.write(EE_DeviceParam,(uint16_t *)&m_dp,sizeof(m_dp)/2);
}


bool Device::ReadParam()
{
	return g_flash.read(EE_DeviceParam,(uint16_t *)&m_dp,sizeof(m_dp)/2);
}
