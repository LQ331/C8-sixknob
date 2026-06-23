#include "Eeprom.h"


Eeprom::Eeprom()
	: m_p_i2c(nullptr)
	, m_p_gpioWP(nullptr)
	, m_u8DeviceAddr(0xA0)
	, m_u8PageSize(128)
	, m_u32TotalSize(512/8*1024)
{
}


Eeprom::Eeprom(I2C * p_i2c,LGpio * p_gpioWP,uint8_t u8DeviceAddr)
	: m_p_i2c(p_i2c)
	, m_p_gpioWP(p_gpioWP)
	, m_u8DeviceAddr(u8DeviceAddr)
	, m_u8PageSize(128)
	, m_u32TotalSize(512/8*1024)
{
}


Eeprom::~Eeprom()
{
}


bool Eeprom::WriteByte(uint16_t u16DataAddr,uint8_t u8Data)
{
	bool bSuccess=false;
	if(u16DataAddr>=m_u32TotalSize)
		return bSuccess;
	if(m_p_gpioWP)
		m_p_gpioWP->Reset();
	if(!m_p_i2c->IsLocked())
	{
		m_p_i2c->Lock();
		int nStartCounter=100;
		while(nStartCounter--)
		{
			m_p_i2c->Start();
			m_p_i2c->WriteByte(m_u8DeviceAddr|I2C_WRITE);
			if(m_p_i2c->WaitAck())
				break;
		}
		if(nStartCounter<=0)
			goto end;
		m_p_i2c->WriteByte((u16DataAddr>>8));
		if(!m_p_i2c->WaitAck())
			goto end;
		m_p_i2c->WriteByte((u16DataAddr%256));
		if(!m_p_i2c->WaitAck())
			goto end;
		m_p_i2c->WriteByte(u8Data);
		if(!m_p_i2c->WaitAck())
			goto end;
		bSuccess=true;
	end:
		m_p_i2c->Stop();
		m_p_i2c->Unlock();
		HAL_Delay(3);
	}
	if(m_p_gpioWP)
		m_p_gpioWP->Set();
	return bSuccess;
}


bool Eeprom::ReadByte(uint16_t u16DataAddr,uint8_t & u8Data)
{
	bool bSuccess=false;
	if(u16DataAddr>=m_u32TotalSize)
		return bSuccess;
	if(!m_p_i2c->IsLocked())
	{
		m_p_i2c->Lock();
		int nStartCounter=100;
		while(nStartCounter--)
		{
			m_p_i2c->Start();
			m_p_i2c->WriteByte(m_u8DeviceAddr|I2C_WRITE);
			if(m_p_i2c->WaitAck())
				break;
		}
		if(nStartCounter<=0)
			goto end;
		m_p_i2c->WriteByte((u16DataAddr>>8));
		if(!m_p_i2c->WaitAck())
			goto end;
		m_p_i2c->WriteByte((u16DataAddr%256));
		if(!m_p_i2c->WaitAck())
			goto end;
		m_p_i2c->Start();
		m_p_i2c->WriteByte(m_u8DeviceAddr|I2C_READ);
		if(!m_p_i2c->WaitAck())
			goto end;
		u8Data=m_p_i2c->ReadByte();
		m_p_i2c->NoAck();
		bSuccess=true;
	end:
		m_p_i2c->Stop();
		m_p_i2c->Unlock();
	}
	return bSuccess;
}


bool Eeprom::Write(uint16_t u16DataAddr,const uint8_t * p_u8Data,int nLength)
{
	bool bSuccess=false;
	if(((uint32_t)u16DataAddr+nLength)>m_u32TotalSize)
		return bSuccess;
	if(m_p_gpioWP)
		m_p_gpioWP->Reset();
	if(!m_p_i2c->IsLocked())
	{
		m_p_i2c->Lock();
		for(int i=0;i<nLength;++i)
		{
			if(i==0||!(u16DataAddr%m_u8PageSize))
			{
				if(i!=0)
				{
					m_p_i2c->Stop();
					HAL_Delay(3);
				}
				int nStartCounter=100;
				while(nStartCounter--)
				{
					m_p_i2c->Start();
					m_p_i2c->WriteByte(m_u8DeviceAddr|I2C_WRITE);
					if(m_p_i2c->WaitAck())
						break;
				}
				if(nStartCounter<=0)
					goto end;
				m_p_i2c->WriteByte((u16DataAddr>>8));
				if(!m_p_i2c->WaitAck())
					goto end;
				m_p_i2c->WriteByte((u16DataAddr%256));
				if(!m_p_i2c->WaitAck())
					goto end;
			}
			m_p_i2c->WriteByte(p_u8Data[i]);
			if(!m_p_i2c->WaitAck())
				goto end;
			u16DataAddr++;
		}
		bSuccess=true;
	end:
		m_p_i2c->Stop();
		m_p_i2c->Unlock();
		HAL_Delay(3);
	}
	if(m_p_gpioWP)
		m_p_gpioWP->Set();
	return bSuccess;
}


bool Eeprom::Read(uint16_t u16DataAddr,uint8_t * p_u8Data,int nLength)
{
	bool bSuccess=false;
	if(((uint32_t)u16DataAddr+nLength)>m_u32TotalSize)
		return bSuccess;
	if(!m_p_i2c->IsLocked())
	{
		m_p_i2c->Lock();
		int nStartCounter=100;
		while(nStartCounter--)
		{
			m_p_i2c->Start();
			m_p_i2c->WriteByte(m_u8DeviceAddr|I2C_WRITE);
			if(m_p_i2c->WaitAck())
				break;
		}
		if(nStartCounter<=0)
			goto end;
		m_p_i2c->WriteByte((u16DataAddr>>8));
		if(!m_p_i2c->WaitAck())
			goto end;
		m_p_i2c->WriteByte((u16DataAddr%256));
		if(!m_p_i2c->WaitAck())
			goto end;
		m_p_i2c->Start();
		m_p_i2c->WriteByte(m_u8DeviceAddr|I2C_READ);
		if(!m_p_i2c->WaitAck())
			goto end;
		for(int i=0;i<nLength;++i)
		{
			p_u8Data[i]=m_p_i2c->ReadByte();
			if(i<nLength-1)
				m_p_i2c->Ack();
			else
				m_p_i2c->NoAck();
		}
		bSuccess=true;
	end:
		m_p_i2c->Stop();
		m_p_i2c->Unlock();
	}
	return bSuccess;
}
