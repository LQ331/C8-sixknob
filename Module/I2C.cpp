#include "I2C.h"


I2C::I2C()
	: m_bIsLocked(false)
{
}


I2C::I2C(const LGpio & gpioSCL,const LGpio & gpioSDA)
	: m_gpioSCL(gpioSCL)
	, m_gpioSDA(gpioSDA)
	, m_bIsLocked(false)
{
}


I2C::~I2C()
{
}


void I2C::SetSdaMode(bool bIsOutput)
{
	m_gpioSDA.m_init.Mode=bIsOutput?GPIO_MODE_OUTPUT_OD:GPIO_MODE_INPUT;
	m_gpioSDA.Init();
}


void I2C::Delay()
{
	int i=1;
	while(i--);
}


bool I2C::IsLocked()
{
	return m_bIsLocked;
}


void I2C::Lock()
{
	m_bIsLocked=true;
}


void I2C::Unlock()
{
	m_bIsLocked=false;
}


void I2C::Start()
{
	SetSdaMode(true);
	m_gpioSCL.Set();
	m_gpioSDA.Set();
	Delay();
	m_gpioSDA.Reset();
	Delay();
	m_gpioSCL.Reset();
}


void I2C::Stop()
{
	SetSdaMode(true);
	m_gpioSCL.Reset();
	m_gpioSDA.Reset();
	Delay();
	m_gpioSCL.Set();
	m_gpioSDA.Set();
	Delay();
}


bool I2C::WaitAck()
{
	bool bAck=false;
	SetSdaMode(true);
	m_gpioSDA.Set();
	m_gpioSCL.Set();
	SetSdaMode(false);
	Delay();
	bAck=m_gpioSDA.IsReset();
	m_gpioSCL.Reset();
	return bAck;
}


void I2C::Ack()
{
	m_gpioSCL.Reset();
	SetSdaMode(true);
	m_gpioSDA.Reset();
	Delay();
	m_gpioSCL.Set();
	Delay();
	m_gpioSCL.Reset();
}


void I2C::NoAck()
{
	m_gpioSCL.Reset();
	SetSdaMode(true);
	m_gpioSDA.Set();
	Delay();
	m_gpioSCL.Set();
	Delay();
	m_gpioSCL.Reset();
}


void I2C::WriteByte(uint8_t u8Byte)
{
	SetSdaMode(true);
	m_gpioSCL.Reset();
	for(int i=0;i<8;++i)
	{
		(u8Byte&0x80)?m_gpioSDA.Set():m_gpioSDA.Reset();
		u8Byte<<=1;
		Delay();
		m_gpioSCL.Set();
		Delay();
		m_gpioSCL.Reset();
	}
}


uint8_t I2C::ReadByte()
{
	uint8_t u8Byte=0;
	SetSdaMode(false);
	for(int i=0;i<8;++i)
	{
		m_gpioSCL.Reset();
		Delay();
		m_gpioSCL.Set();
		u8Byte<<=1;
		if(m_gpioSDA.IsSet())
			u8Byte|=0x01;
		Delay();
	}
	return u8Byte;
}
