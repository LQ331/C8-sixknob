#ifndef __AUTOCOMM_H
#define __AUTOCOMM_H

#include "Protocol.h"
#include "Communication.h"
#include <vector>

#define AUTOCOMM_BUFF_SIZE 256

class AutoComm
{
public:
	enum RUN_STATUS
	{
		STATUS_IDLE=0,
		STATUS_RECVING_START,
		STATUS_RECVING_END,
		STATUS_SENDING_START,
		STATUS_SENDING_END
	};
private:
	uint8_t m_sz_u8Buff[AUTOCOMM_BUFF_SIZE];
	uint16_t m_u16RecvCounter;
	uint16_t m_u16SendCounter;
	RUN_STATUS m_emStatus;
	uint32_t m_u32TimeoutElapse;
	uint32_t m_u32TimeoutCounter;
	bool m_bIsCommunicating;
	bool m_bIsSendComplete;
	Protocol * m_p_prtocol;
	std::vector<Protocol *> m_vec_p_protocol;
public:
	AutoComm();
	virtual ~AutoComm();
	void Run(Communication & comm,LUsart & usart,LGpio & gpio,uint16_t u16Interval);
	RUN_STATUS GetRunStatus();
	void SetTimeoutElapse(uint32_t u32TimeoutElapse);
	bool IsCommunicating();
	void SendComplete();
	void AddProtocol(Protocol * p_protocol);
	void RemoveProtocol(Protocol * p_protocol);
	void ClearProtocol();
};

#endif