#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "LUsart.h"

#define COMM_BUFF_SIZE 32

class Communication
{
private:
	uint8_t m_sz_u8Buff[COMM_BUFF_SIZE];
	uint8_t m_u8WriteIndex;
	uint8_t m_u8ReadIndex;
	uint32_t m_u32WriteCounter;
	uint32_t m_u32ReadCounter;	
	bool m_bLoopWrite;	
	uint8_t m_u8Data;
public:
	Communication();
	virtual ~Communication();
	bool IsEmpty();
	bool IsLoopWrite();
	void StartRecv(LUsart & usart);
	void StartSend(LUsart & usart,uint8_t *p_u8Data, uint16_t u16Size);
	void Write();
	bool Read(uint8_t & u8Data);
};

#endif