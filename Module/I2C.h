#ifndef __I2C_H
#define __I2C_H

#include "LGpio.h"

#define I2C_WRITE 0x00
#define I2C_READ 0x01

class I2C
{
private:
	LGpio m_gpioSCL;
	LGpio m_gpioSDA;
	bool m_bIsLocked;
private:
	void SetSdaMode(bool bIsOutput);
	void Delay();
public:
	I2C();
	I2C(const LGpio & gpioSCL,const LGpio & gpioSDA);
	virtual ~I2C();
	bool IsLocked();
	void Lock();
	void Unlock();
	void Start();
	void Stop();
	bool WaitAck();
	void Ack();
	void NoAck();
	void WriteByte(uint8_t u8Byte);
	uint8_t ReadByte();
};

#endif