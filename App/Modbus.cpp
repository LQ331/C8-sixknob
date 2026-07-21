#include "Modbus.h"
#include "LClassLib.h"


Modbus::Modbus()
	: m_u8RecvCount(0)
	, m_u8SendCount(0)
	, m_emAddrType(ADDR_ERROR)
	, m_bIsAck(true)
	, m_bIsBaudrateChanged(false)
{
}


Modbus::~Modbus()
{
}


Modbus::ADDR_TYPE Modbus::ProcessAddrType(uint8_t u8Addr)
{
	ADDR_TYPE emAddrType=ADDR_ERROR;
	if(u8Addr==0)
		emAddrType=ADDR_BROADCAST;
	else if(u8Addr==g_device.GetParam().m_u8Addr||u8Addr==0xAA)
		emAddrType=ADDR_MATCH;
	return emAddrType;
}


uint16_t Modbus::GetCRC16(uint8_t * p_u8Data,uint16_t u16Length)
{
	uint16_t u16CRC=0xFFFF;
	while(u16Length)
	{
		u16CRC^=*p_u8Data++;
		for(int i=0;i<8;++i)
		{
			if(u16CRC&1)
			{
				u16CRC>>=1;
				u16CRC^=0xA001;
			}
			else
				u16CRC>>=1;
		}
		--u16Length;
	}
	return u16CRC;
}


bool Modbus::Decode()
{
	m_bIsAck=true;
	m_emAddrType=ProcessAddrType(m_sz_u8Buff[INDEX_ADDRESS]);
	if(m_emAddrType==ADDR_BROADCAST)
		m_bIsAck=false;
	switch(m_sz_u8Buff[INDEX_FUNCCODE])
	{
		case 0x03:ReadHoldReg();break;
		case 0x04:ReadInputReg();break;
		case 0x05:WriteSingleCoil();break;
		case 0x06:WriteSingleHoldReg();break;
		case 0x10:WriteMultiHoldReg();break;
		default:Exception(EXCEPTION_01);break;
	}
	if(m_bIsAck)
		Encode();
	return m_bIsAck;
}


void Modbus::Encode()
{
	uint16_t u16CRC16=GetCRC16(m_sz_u8Buff,m_u8SendCount);
	m_sz_u8Buff[m_u8SendCount]=u16CRC16&0x00FF;
	m_u8SendCount++;
	m_sz_u8Buff[m_u8SendCount]=(u16CRC16>>8)&0x00FF;
	m_u8SendCount++;
}


void Modbus::Exception(EXCEPTION_TYPE emException)
{
	m_sz_u8Buff[INDEX_FUNCCODE]|=0x80;
	m_sz_u8Buff[INDEX_DATAAREA_START]=emException;
	m_u8SendCount=INDEX_DATAAREA_START+MODBUS_EXCEPTION_SIZE;
}



bool Modbus::ReadReg(uint16_t u16RegAddr,uint16_t * p_u16Data)
{
	bool bSuccess=true;
	switch(u16RegAddr)
	{
		case 0x3012:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x3013:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Delay/1000;
		break;
		case 0x3014:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Threshold;
		break;
		case 0x3015:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Delay;
		break;
		case 0x3016:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR3].m_u32Threshold;
		break;
		case 0x3017:
		*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold;
		break;
		case 0x301A:
		case 0x3100:
			*p_u16Data=g_device.GetParam().m_u8Addr;
		break;
		case 0x3101:
			*p_u16Data=g_device.GetParam().m_u32Baudrate;
		break;
		case 0x4000:
		break;
		case 0x4001:
			*p_u16Data=0;
		break;
		case 0x4002:
		break;
		case 0x4003:
		case 0x4004:
		case 0x4006:
		case 0x4007:
		case 0x4008:
			*p_u16Data=g_elecparam.m_sz_u32I[u16RegAddr-0x4006]/RATIO_CUR;
		break;
		case 0x0000:
		case 0x0001:
		case 0x0002:
		case 0x0003:
			*p_u16Data=g_elecparam.m_sz_u32I[u16RegAddr-0x0000]/RATIO_CUR;
		break;
		case 0x0004:
			*p_u16Data=0;
		break;
		case 0x0005:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x0006:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Delay/1000;
		break;
		case 0x0007:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x0008:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Delay;
		break;
		case 0x0009:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR3].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x000A:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold/100;
		break;
		case 0x000B:
			*p_u16Data=0;
		break;
		case 0x000C:
			*p_u16Data=0;
		break;
		case 0x000D:
			*p_u16Data=g_sz[g_emFrameCurrent];
		break;
		case 0x000E:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold/100;
		break;
		case 0x000f:
			*p_u16Data=0;
		break;
		case 0x0010:
			if(g_device.GetParam().m_u32Baudrate==9600)
				*p_u16Data=0x0100|(g_device.GetParam().m_u8Addr);
			else
				*p_u16Data=0x0000|(g_device.GetParam().m_u8Addr);
		break;
		case 0x0011:
			//*p_u16Data=g_flash.read(EE_OpenReason,(uint16_t *)&m_u32OpenReason,sizeof(m_u32OpenReason)/2);
		*p_u16Data=g_action.GetOpenReason();
		break;
		case 0x0013:
			*p_u16Data=0;
		break;
		case 0x0014:
		case 0x0012:
		case 0x0015:
		case 0x0016:
		case 0x0017:
		case 0x0018:
		case 0x0019:
		case 0x001A:
		case 0x001B:
		case 0x001C:
		case 0x001D:
		case 0x001E:
		case 0x001F:
		case 0x0020:
		case 0x0021:
			*p_u16Data=0;
		break;
		case 0x0100:
		case 0x0101:
		case 0x0102:
		case 0x0103:
			*p_u16Data=g_elecparam.m_sz_u32I[u16RegAddr-0x0100]/RATIO_CUR;
		break;
		case 0x0104:
			*p_u16Data=0;
		break;
		case 0x0105:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x0106:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Delay/1000;
		break;
		case 0x0107:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x0108:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Delay;
		break;
		case 0x0109:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR3].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x010A:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold/100;
		case 0x010B:
			*p_u16Data=0;
		break;
		case 0x010C:
		break;
		case 0x010D:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold;
		break;
		case 0x010E:
			*p_u16Data=g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold/100;
		break;
		case 0x010f:
			*p_u16Data=0;
		break;
		case 0x0110:
			if(g_device.GetParam().m_u32Baudrate==9600)
				*p_u16Data=0x0100|(g_device.GetParam().m_u8Addr);
			else
				*p_u16Data=0x0000|(g_device.GetParam().m_u8Addr);
		break;
		case 0x0111:
			*p_u16Data=0;
		break;
		case 0x0114:
			*p_u16Data=0;
		break;
		case 0x0200:
		case 0x0201:
		case 0x0202:
		case 0x0203:
		case 0x0204:
		case 0x0205:
		case 0x0206:
		case 0x0207:
		case 0x0208:
		case 0x0209:
		case 0x020a:
		case 0x020b:
		case 0x020c:
		case 0x020d:
		case 0x020e:
			*p_u16Data=g_coef.m_sz2_u16PointHiIRMS[0][u16RegAddr-0x0200];
		break;
		case 0x0300:
		case 0x0301:
		case 0x0302:
		case 0x0303:
		case 0x0304:
		case 0x0305:
		case 0x0306:
		case 0x0307:
		case 0x0308:
		case 0x0309:
		case 0x030a:
		case 0x030b:
		case 0x030c:
		case 0x030d:
		case 0x030e:
			*p_u16Data=g_coef.m_sz2_u16PointHiIRMS[1][u16RegAddr-0x0300];
		break;
		case 0x0400:
		case 0x0401:
		case 0x0402:
		case 0x0403:
		case 0x0404:
		case 0x0405:
		case 0x0406:
		case 0x0407:
		case 0x0408:
		case 0x0409:
		case 0x040a:
		case 0x040b:
		case 0x040c:
		case 0x040d:
		case 0x040e:
			*p_u16Data=g_coef.m_sz2_u16PointHiIRMS[2][u16RegAddr-0x0400];
		break;
		case 0x0500:
		case 0x0501:
		case 0x0502:
		case 0x0503:
		case 0x0504:
		case 0x0505:
		case 0x0506:
		case 0x0507:
		case 0x0508:
		case 0x0509:
		case 0x050a:
		case 0x050b:
		case 0x050c:
		case 0x050d:
		case 0x050e:
			*p_u16Data=g_coef.m_sz2_u16PointHiIRMS[3][u16RegAddr-0x0500];
		break;
		default:bSuccess=false;break;
	}
	return bSuccess;
}


bool Modbus::WriteReg(uint16_t u16RegAddr,uint16_t u16Data)
{
	bool bSuccess=true;
	switch(u16RegAddr)
	{
		case 0x3100:
		{
			if(u16Data<=0x00FF)
			{
				DeviceParam dp=g_device.GetParam(false);
				dp.m_u8Addr=u16Data;
				g_device.SetParam(dp,false);				
			}
			else
				bSuccess=false;
		}
		break;
		case 0x3101:
		{
			if(u16Data==2400||u16Data==4800||u16Data==9600)
			{
				DeviceParam dp=g_device.GetParam(false);
				if(dp.m_u32Baudrate!=u16Data)
				{
					dp.m_u32Baudrate=u16Data;
					g_device.SetParam(dp,false);
					m_bIsBaudrateChanged=true;
				}
			}
			else
				bSuccess=false;
		}
		break;
		case 0x5000:
		{
			if(u16Data==0x0000)
				g_action.Open(Action::OPEN_COMM);
			else
				bSuccess=false;
		}
		break;
		case 0x0010:
		case 0x0011:
		{
			if((u16Data&0x00FF)<=0x00FF)
			{
				DeviceParam dp=g_device.GetParam(false);
				dp.m_u8Addr=(u16Data&0x00FF);
				g_device.SetParam(dp,false);				
			}
			else 
				bSuccess=false;
			if((u16Data&0xFF00)==0x0100)
			{
				DeviceParam dp=g_device.GetParam(false);
				if(dp.m_u32Baudrate!=9600)
				{
					dp.m_u32Baudrate=9600;
					g_device.SetParam(dp,false);
					m_bIsBaudrateChanged=true;
				}
			}
			else if((u16Data&0xFF00)!=0x0100)
			{
				DeviceParam dp=g_device.GetParam(false);
				if(dp.m_u32Baudrate!=4800)
				{
					dp.m_u32Baudrate=4800;
					g_device.SetParam(dp,false);
					m_bIsBaudrateChanged=true;
				}
			}
			else
				bSuccess=false;
		}
		break;
		case 0x0015:
		case 0x0115:
			{
			if(u16Data==0xAAAA)
				g_action.Open(Action::OPEN_COMM);
			else
				bSuccess=false;
		}
			
		default:bSuccess=false;break;
	}
	return bSuccess;
}


bool Modbus::WriteSingleReg()
{
	const uint8_t u8RegDataStart=INDEX_DATAAREA_START+MODBUS_REGSTARTADDR_SIZE;
	if(!WriteReg(m_u16RegStartAddr,m_u16RegData))
		return false;
	m_u8SendCount=u8RegDataStart+sizeof(m_u16RegData);
	return true;
}


bool Modbus::ReadMultiReg()
{
	const uint8_t u8RegByteNumStart=INDEX_DATAAREA_START;
	const uint8_t u8RegDataStart=u8RegByteNumStart+MODBUS_REGBYTENUM_SIZE;
	for(int i=0;i<m_u16RegNum;++i)
	{
		if(ReadReg(m_u16RegStartAddr+i,&m_u16RegData))
		{
			m_sz_u8Buff[u8RegDataStart+2*i]=(m_u16RegData>>8)&0x00FF;
			m_sz_u8Buff[u8RegDataStart+2*i+1]=m_u16RegData&0x00FF;
		}
		else
			return false;
	}
	m_sz_u8Buff[u8RegByteNumStart]=m_u16RegNum*2;
	m_u8SendCount=u8RegDataStart+m_sz_u8Buff[u8RegByteNumStart];
	return true;
}


bool Modbus::WriteMultiReg()
{
	const uint8_t u8RegByteNumStart=INDEX_DATAAREA_START+MODBUS_REGSTARTADDR_SIZE+MODBUS_REGNUM_SIZE;
	const uint8_t u8RegDataStart=u8RegByteNumStart+MODBUS_REGBYTENUM_SIZE;
	for(int i=0;i<m_u16RegNum;++i)
	{
		m_u16RegData=m_sz_u8Buff[u8RegDataStart+2*i]*256+m_sz_u8Buff[u8RegDataStart+2*i+1];
		if(!WriteReg(m_u16RegStartAddr+i,m_u16RegData))
			return false;
	}
	m_u8SendCount=u8RegByteNumStart;
	return true;
}


void Modbus::ReadHoldReg()
{
	if(m_u8RecvCount==8)
	{
		EXCEPTION_TYPE emException=EXCEPTION_NONE;
		const uint8_t u8RegStartAddrStart=INDEX_DATAAREA_START;
		const uint8_t u8RegNumStart=u8RegStartAddrStart+MODBUS_REGSTARTADDR_SIZE;
		m_u16RegStartAddr=m_sz_u8Buff[u8RegStartAddrStart]*256+m_sz_u8Buff[u8RegStartAddrStart+1];
		m_u16RegNum=m_sz_u8Buff[u8RegNumStart]*256+m_sz_u8Buff[u8RegNumStart+1];
		if(m_u16RegStartAddr>=MODBUS_HOLD_REG_START&&m_u16RegStartAddr<=MODBUS_HOLD_REG_END&&m_u16RegStartAddr+m_u16RegNum>=MODBUS_HOLD_REG_START&&m_u16RegStartAddr+m_u16RegNum<=MODBUS_HOLD_REG_END)
		{
			if(m_u16RegNum>=0x0001&&m_u16RegNum<=0x007D)
			{
				if(!ReadMultiReg())
					emException=EXCEPTION_04;
			}
			else
				emException=EXCEPTION_03;
		}
		else
			emException=EXCEPTION_02;
		if(emException)
			Exception(emException);
	}
	else
		m_bIsAck=false;
}


void Modbus::ReadInputReg()
{
	if(m_u8RecvCount==8)
	{
		EXCEPTION_TYPE emException=EXCEPTION_NONE;
		const uint8_t u8RegStartAddrStart=INDEX_DATAAREA_START;
		const uint8_t u8RegNumStart=u8RegStartAddrStart+MODBUS_REGSTARTADDR_SIZE;
		m_u16RegStartAddr=m_sz_u8Buff[u8RegStartAddrStart]*256+m_sz_u8Buff[u8RegStartAddrStart+1];
		m_u16RegNum=m_sz_u8Buff[u8RegNumStart]*256+m_sz_u8Buff[u8RegNumStart+1];
		if(m_u16RegStartAddr>=MODBUS_INPUT_REG_START&&m_u16RegStartAddr<=MODBUS_INPUT_REG_END&&m_u16RegStartAddr+m_u16RegNum>=MODBUS_INPUT_REG_START&&m_u16RegStartAddr+m_u16RegNum<=MODBUS_INPUT_REG_END)
		{
			if(m_u16RegNum>=0x0001&&m_u16RegNum<=0x007D)
			{
				if(!ReadMultiReg())
					emException=EXCEPTION_04;
			}
			else
				emException=EXCEPTION_03;
		}
		else
			emException=EXCEPTION_02;
		if(emException)
			Exception(emException);
	}
	else
		m_bIsAck=false;
}


void Modbus::WriteSingleCoil()
{
	if(m_u8RecvCount==8)
	{
		EXCEPTION_TYPE emException=EXCEPTION_NONE;
		const uint8_t u8RegStartAddrStart=INDEX_DATAAREA_START;
		const uint8_t u8RegDataStart=u8RegStartAddrStart+MODBUS_REGSTARTADDR_SIZE;
		m_u16RegStartAddr=m_sz_u8Buff[u8RegStartAddrStart]*256+m_sz_u8Buff[u8RegStartAddrStart+1];
		m_u16RegData=m_sz_u8Buff[u8RegDataStart]*256+m_sz_u8Buff[u8RegDataStart+1];
		if(m_u16RegStartAddr>=MODBUS_COIL_START&&m_u16RegStartAddr<=MODBUS_COIL_END)
		{
			if(m_u16RegData==0x0000||m_u16RegData==0xFF00)
			{
				if(!WriteSingleReg())
					emException=EXCEPTION_04;
			}
			else
				emException=EXCEPTION_03;
		}
		else
			emException=EXCEPTION_02;
		if(emException)
			Exception(emException);
	}
	else
		m_bIsAck=false;
}


void Modbus::WriteSingleHoldReg()
{
	if(m_u8RecvCount==8)
	{
		EXCEPTION_TYPE emException=EXCEPTION_NONE;
		const uint8_t u8RegStartAddrStart=INDEX_DATAAREA_START;
		const uint8_t u8RegDataStart=u8RegStartAddrStart+MODBUS_REGSTARTADDR_SIZE;
		m_u16RegStartAddr=m_sz_u8Buff[u8RegStartAddrStart]*256+m_sz_u8Buff[u8RegStartAddrStart+1];
		m_u16RegData=m_sz_u8Buff[u8RegDataStart]*256+m_sz_u8Buff[u8RegDataStart+1];
		if(m_u16RegStartAddr>=MODBUS_HOLD_REG_START&&m_u16RegStartAddr<=MODBUS_HOLD_REG_END)
		{
			if(m_u16RegData>=0x0000&&m_u16RegData<=0xFFFF)
			{
				if(WriteSingleReg())
				{
					if(m_u16RegStartAddr>=MODBUS_HOLD_DEVICE_START&&m_u16RegStartAddr<=MODBUS_HOLD_DEVICE_END)
					{
						g_device.ParamRestore();
						g_device.EraseWriteParam();
					}
				}
				else
				{
					emException=EXCEPTION_04;
				}
			}
			else
				emException=EXCEPTION_03;
		}
		else
			emException=EXCEPTION_02;
		if(emException)
			Exception(emException);
	}
	else
		m_bIsAck=false;
}


void Modbus::WriteMultiHoldReg()
{
	if(m_u8RecvCount>8&&m_u8RecvCount==m_sz_u8Buff[6]+9)
	{
		EXCEPTION_TYPE emException=EXCEPTION_NONE;
		const uint8_t u8RegStartAddrStart=INDEX_DATAAREA_START;
		const uint8_t u8RegNumStart=u8RegStartAddrStart+MODBUS_REGSTARTADDR_SIZE;
		const uint8_t u8RegByteNumStart=u8RegNumStart+MODBUS_REGNUM_SIZE;
		m_u16RegStartAddr=m_sz_u8Buff[u8RegStartAddrStart]*256+m_sz_u8Buff[u8RegStartAddrStart+1];
		m_u16RegNum=m_sz_u8Buff[u8RegNumStart]*256+m_sz_u8Buff[u8RegNumStart+1];
		if(m_u16RegStartAddr>=MODBUS_HOLD_REG_START&&m_u16RegStartAddr<=MODBUS_HOLD_REG_END&&m_u16RegStartAddr+m_u16RegNum>=MODBUS_HOLD_REG_START&&m_u16RegStartAddr+m_u16RegNum<=MODBUS_HOLD_REG_END)
		{
			if(m_u16RegNum>=0x0001&&m_u16RegNum<=0x007B&&m_u16RegNum*2==m_sz_u8Buff[u8RegByteNumStart])
			{
				if(WriteMultiReg())
				{
					if((m_u16RegStartAddr>=MODBUS_HOLD_PROTECT_START&&m_u16RegStartAddr<=MODBUS_HOLD_PROTECT_END)||(m_u16RegStartAddr+m_u16RegNum>=MODBUS_HOLD_PROTECT_START&&m_u16RegStartAddr+m_u16RegNum<=MODBUS_HOLD_PROTECT_END))
					{
						
					}
					else if((m_u16RegStartAddr>=MODBUS_HOLD_DEVICE_START&&m_u16RegStartAddr<=MODBUS_HOLD_DEVICE_END)||(m_u16RegStartAddr+m_u16RegNum>=MODBUS_HOLD_DEVICE_START&&m_u16RegStartAddr+m_u16RegNum<=MODBUS_HOLD_DEVICE_END))
					{
						g_device.ParamRestore();
						g_device.EraseWriteParam();
					}
				}
				else
				{
					g_device.ParamBackup();
					emException=EXCEPTION_04;
				}
			}
			else
				emException=EXCEPTION_03;
		}
		else
			emException=EXCEPTION_02;
		if(emException)
			Exception(emException);
	}
	else
		m_bIsAck=false;
}


bool Modbus::Check(uint8_t * p_u8DataFrame,uint16_t u16Length)
{
	if(u16Length<INDEX_DATAAREA_START+2||u16Length>MODBUS_BUFF_SIZE)
		return false;
	if(!ProcessAddrType(p_u8DataFrame[INDEX_ADDRESS]))
		return false;
	uint16_t u16CRC16ByData,u16CRC16ByCalc;
	u16CRC16ByData=p_u8DataFrame[u16Length-2]+p_u8DataFrame[u16Length-1]*256;
	u16CRC16ByCalc=GetCRC16(p_u8DataFrame,u16Length-2);
	return (u16CRC16ByData==u16CRC16ByCalc);
}


void Modbus::OnRecvStart()
{
}


bool Modbus::OnRecvEnd(uint8_t * p_u8DataFrame,uint16_t u16Length)
{
	memcpy(m_sz_u8Buff,p_u8DataFrame,u16Length);
	m_u8RecvCount=u16Length;
	return Decode();
}


void Modbus::OnSendStart(uint8_t * p_u8DataFrame,uint16_t & u16Length)
{
	memcpy(p_u8DataFrame,m_sz_u8Buff,m_u8SendCount);
	u16Length=m_u8SendCount;
}


bool Modbus::OnSendEnd()
{
	if(m_bIsBaudrateChanged)
	{
		g_usartRS485.m_handle.Init.BaudRate=g_device.GetParam().m_u32Baudrate;
		g_usartRS485.Init();
		g_commRS485.StartRecv(g_usartRS485);
		m_bIsBaudrateChanged=false;
	}
	return true;
}