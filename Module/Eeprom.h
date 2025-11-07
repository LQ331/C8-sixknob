#ifndef __EEPROM_H
#define __EEPROM_H

#include "I2C.h"

class Eeprom
{
private:
	I2C * m_p_i2c;
	LGpio * m_p_gpioWP;
	uint8_t m_u8DeviceAddr;
	uint8_t m_u8PageSize;
	uint32_t m_u32TotalSize;
public:
	Eeprom();
	Eeprom(I2C * p_i2c,LGpio * p_gpioWP=nullptr,uint8_t u8DeviceAddr=0xA0);
	virtual ~Eeprom();
	bool WriteByte(uint16_t u16DataAddr,uint8_t u8Data);
	bool ReadByte(uint16_t u16DataAddr,uint8_t & u8Data);
	bool Write(uint16_t u16DataAddr,const uint8_t * p_u8Data,int nLength);
	bool Read(uint16_t u16DataAddr,uint8_t * p_u8Data,int nLength);
};

#endif