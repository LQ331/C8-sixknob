#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "LGpio.h"

class Protocol
{
public:
	Protocol();
	virtual ~Protocol();
	virtual bool Check(uint8_t * p_u8DataFrame,uint16_t u16Length)=0;
	virtual void OnRecvStart()=0;
	virtual bool OnRecvEnd(uint8_t * p_u8DataFrame,uint16_t u16Length)=0;
	virtual void OnSendStart(uint8_t * p_u8DataFrame,uint16_t & u16Length)=0;
	virtual bool OnSendEnd()=0;
};

#endif