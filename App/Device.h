#ifndef __DEVICE_H
#define __DEVICE_H

#include "Modbus.h"

#define DEVICE_LOGINPSWD_SIZE 4

enum PROTOCOL_TYPE
{
	PROTOCOL_MODBUS=0,
};

class DeviceParam
{
public:
	PROTOCOL_TYPE m_emProtocol;
	uint8_t m_u8Addr;
	uint32_t m_u32Baudrate;

public:
	DeviceParam()
		: m_emProtocol(PROTOCOL_MODBUS)
		, m_u8Addr(1)
		, m_u32Baudrate(9600)
	{
	};
};

class Device
{
private:
	DeviceParam m_dp;
	DeviceParam m_dpBackup;
public:
	Device();
	virtual ~Device();
	const DeviceParam & GetParam(bool bIsCurrent=true);
	void SetParam(const DeviceParam & dp,bool bIsCurrent=true);
	void ParamBackup();
	void ParamRestore();
	void EraseWriteParam();
	bool WriteParam();
	bool ReadParam();
};

#endif