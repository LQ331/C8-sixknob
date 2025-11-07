#include "Communication.h"


Communication::Communication()
	: m_u8WriteIndex(0)
	, m_u8ReadIndex(0)
	, m_u32WriteCounter(0)
	, m_u32ReadCounter(0)
	, m_bLoopWrite(false)
{
}


Communication::~Communication()
{
}


bool Communication::IsEmpty()
{
	return (m_u32WriteCounter==m_u32ReadCounter);
}


bool Communication::IsLoopWrite()
{
	return m_bLoopWrite;
}

void Communication::StartRecv(LUsart & usart)
{
	usart.Receive_IT(&m_u8Data,1);
}


void Communication::StartSend(LUsart & usart,uint8_t *p_u8Data, uint16_t u16Size)
{
	usart.Transmit_IT(p_u8Data,u16Size);
}


void Communication::Write()
{
	m_sz_u8Buff[m_u8WriteIndex]=m_u8Data;
	m_u8WriteIndex=(m_u8WriteIndex+1)<COMM_BUFF_SIZE?(m_u8WriteIndex+1):0;
	m_u32WriteCounter++;
	if(m_u32WriteCounter-m_u32ReadCounter>COMM_BUFF_SIZE)
		m_bLoopWrite=true;
}


bool Communication::Read(uint8_t & u8Data)
{
	bool bRtn=!IsEmpty();
	if(bRtn)
	{
		u8Data=m_sz_u8Buff[m_u8ReadIndex];
		m_u8ReadIndex=(m_u8ReadIndex+1)<COMM_BUFF_SIZE?(m_u8ReadIndex+1):0;
		m_u32ReadCounter++;
	}
	return bRtn;
}
