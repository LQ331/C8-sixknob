#ifndef __MODBUS_H
#define __MODBUS_H

#include "Protocol.h"
#include "Communication.h"

#define MODBUS_BUFF_SIZE 256
#define MODBUS_REGSTARTADDR_SIZE 2
#define MODBUS_REGNUM_SIZE 2
#define MODBUS_REGBYTENUM_SIZE 1
#define MODBUS_EXCEPTION_SIZE 1
#define MODBUS_HOLD_REG_START 0x0000
#define MODBUS_HOLD_PROTECT_START 0x3000
#define MODBUS_HOLD_PROTECT_END 0x30FF
#define MODBUS_HOLD_DEVICE_START 0x0000
#define MODBUS_HOLD_DEVICE_END 0x31FF
#define MODBUS_HOLD_REG_END 0x3FFF
#define MODBUS_INPUT_REG_START 0x4000
#define MODBUS_INPUT_REG_END 0x4FFF
#define MODBUS_COIL_START 0x5000
#define MODBUS_COIL_END 0x5000

class Modbus:public Protocol
{
public:
		enum FRAME_INDEX
	{
		INDEX_ADDRESS=0,
		INDEX_FUNCCODE,
		INDEX_DATAAREA_START
	};
		enum ADDR_TYPE
	{
		ADDR_ERROR=0,//错误地址
		ADDR_MATCH,//匹配地址
		ADDR_BROADCAST//广播地址
	};
	enum EXCEPTION_TYPE
	{
		EXCEPTION_NONE=0,
		EXCEPTION_01,
		EXCEPTION_02,
		EXCEPTION_03,
		EXCEPTION_04
	};
private:
	uint8_t m_sz_u8Buff[MODBUS_BUFF_SIZE];
	uint8_t m_u8RecvCount;
	uint8_t m_u8SendCount;
	ADDR_TYPE m_emAddrType;
	uint16_t m_u16RegStartAddr;
	uint16_t m_u16RegNum;
	uint16_t m_u16RegData;
	bool m_bIsAck;
	bool m_bIsBaudrateChanged;
private:
	ADDR_TYPE ProcessAddrType(uint8_t u8Addr);
	uint16_t GetCRC16(uint8_t * p_u8Data,uint16_t u16Length);
	bool Decode();
	void Encode();
	void Exception(EXCEPTION_TYPE emException);
	bool ReadReg(uint16_t u16RegAddr,uint16_t * p_u16Data);
	bool WriteReg(uint16_t u16RegAddr,uint16_t u16Data);
	bool WriteSingleReg();
	bool ReadMultiReg();
	bool WriteMultiReg();
	void ReadHoldReg();
	void ReadInputReg();
	void WriteSingleCoil();
	void WriteSingleHoldReg();
	void WriteMultiHoldReg();
public:
	Modbus();
	virtual ~Modbus();
	virtual bool Check(uint8_t * p_u8DataFrame,uint16_t u16Length);
	virtual void OnRecvStart();
	virtual bool OnRecvEnd(uint8_t * p_u8DataFrame,uint16_t u16Length);
	virtual void OnSendStart(uint8_t * p_u8DataFrame,uint16_t & u16Length);
	virtual bool OnSendEnd();
};

#endif