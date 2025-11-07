#include "AutoComm.h"


AutoComm::AutoComm()
	: m_u16RecvCounter(0)
	, m_u16SendCounter(0)
	, m_emStatus(STATUS_IDLE)
	, m_u32TimeoutElapse(100)
	, m_u32TimeoutCounter(0)
	, m_bIsCommunicating(false)
	, m_bIsSendComplete(false)
	, m_p_prtocol(nullptr)
{
}


AutoComm::~AutoComm()
{
}


void AutoComm::Run(Communication & comm,LUsart & usart, LGpio & gpio,uint16_t u16Interval)
{
	switch(m_emStatus)
	{	
		case STATUS_IDLE:			
		{
			gpio.Reset();
			if(!comm.IsEmpty())
			{
				for(auto p_protocol:m_vec_p_protocol)
					p_protocol->OnRecvStart();
				m_u16RecvCounter=0;
				m_u32TimeoutCounter=0;
				m_emStatus=STATUS_RECVING_START;
				m_bIsCommunicating=true;
			}
		}
		break;
		case STATUS_RECVING_START:
		{
			if(m_u16RecvCounter<AUTOCOMM_BUFF_SIZE)
			{
				if(comm.Read(m_sz_u8Buff[m_u16RecvCounter]))
				{
					m_u16RecvCounter++;
					m_u32TimeoutCounter=0;
				}
				else
				{
					if(m_u32TimeoutCounter<m_u32TimeoutElapse)
						m_u32TimeoutCounter+=u16Interval;
					else
						m_emStatus=STATUS_RECVING_END;
				}
			}
			else
				m_emStatus=STATUS_RECVING_END;
		}
		break;
		case STATUS_RECVING_END:
		{
			bool bIsFind=false;
			for(auto p_protocol:m_vec_p_protocol)
			{
				if(p_protocol->Check(m_sz_u8Buff,m_u16RecvCounter))
				{
					bIsFind=true;
					m_p_prtocol=p_protocol;
					break;
				}
			}
			if(bIsFind)
			{
				if(m_p_prtocol->OnRecvEnd(m_sz_u8Buff,m_u16RecvCounter))
					m_emStatus=STATUS_SENDING_START;
				else
				{
					m_emStatus=STATUS_IDLE;
					m_bIsCommunicating=false;
				}
			}		
			else
			{
				m_emStatus=STATUS_IDLE;
				m_bIsCommunicating=false;
			}
		}
		break;
		case STATUS_SENDING_START:		
		{
			gpio.Set();
			m_bIsSendComplete=false;
			m_p_prtocol->OnSendStart(m_sz_u8Buff,m_u16SendCounter);
			comm.StartSend(usart,m_sz_u8Buff,m_u16SendCounter);
			m_emStatus=STATUS_SENDING_END;
		}
		break;
		case STATUS_SENDING_END:
		{
			if(m_bIsSendComplete)
			{
				m_p_prtocol->OnSendEnd();
				m_emStatus=STATUS_IDLE;
				m_bIsCommunicating=false;
			}
		}
		break;
		default:break;
	}
}

AutoComm::RUN_STATUS AutoComm::GetRunStatus()
{
	return m_emStatus;
}


void AutoComm::SetTimeoutElapse(uint32_t u32TimeoutElapse)
{
	m_u32TimeoutElapse=u32TimeoutElapse;
}


bool AutoComm::IsCommunicating()
{
	return m_bIsCommunicating;
}


void AutoComm::SendComplete()
{
	m_bIsSendComplete=true;
}

void AutoComm::AddProtocol(Protocol * p_protocol)
{
	m_vec_p_protocol.push_back(p_protocol);
}


void AutoComm::RemoveProtocol(Protocol * p_protocol)
{
	for(auto it=m_vec_p_protocol.begin();it<m_vec_p_protocol.end();++it)
	{
		if(*it==p_protocol)
			m_vec_p_protocol.erase(it);
	}
}


void AutoComm::ClearProtocol()
{
	m_vec_p_protocol.clear();
}