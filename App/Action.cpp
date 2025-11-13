#include "Action.h"
#include "LClassLib.h"


Action::Action()
{
}


Action::Action(const LGpio & gpioTrip,const LGpio & gpioReset,const LGpio & gpioBuzzer,const LGpio & gpioAlarm)
	: m_gpioTrip(gpioTrip)
	, m_u32Warn(WARN_NONE)
	, m_gpioReset(gpioReset)
	, m_gpioBuzzer(gpioBuzzer)
	, m_gpioAlarm(gpioAlarm)
	, m_bIsActing(false)
	, m_u32WarnCounter(ACTION_WARN_TIME)
{
	m_gpioAlarm.Reset();
	m_gpioBuzzer.Reset();
}


Action::~Action()
{
}


void Action::Open(OPEN_REASON emReason)
{
	if(!m_bIsActing)
	{
		m_gpioTrip.Set();
		m_u32ActingCounter=0;
		m_bIsActing=true;
		g_u32OpenReason=emReason;
		g_device.EraseWriteParam();
	}
}


void Action::TripReset(uint16_t u16Interval)
{
	if(m_bIsActing)
	{
		m_u32ActingCounter+=u16Interval;
		m_u32IsActionCount+=u16Interval;
		if(m_u32ActingCounter>=ACTION_TRIP_TIMEOUT)			
			{
				m_gpioTrip.Reset();
			}
		if(m_u32IsActionCount>=ACTION_FLAG_TIMROUT)
			{
				m_bIsActing=false;									
				m_u32IsActionCount=0;
			}
	}					
}


void Action::Warn(bool bEnable)
{
	if(bEnable)
		g_ledAlarm.On();
	else
		g_ledAlarm.Off();
}


void Action::WarnNoTrip(uint16_t u16Interval,bool bEnable)
{
	if(bEnable)
	{
		m_gpioAlarm.Set();
		m_u32WarnCounter+=u16Interval;
		if(m_u32WarnCounter>=ACTION_WARN_TIME)
		{
			m_gpioBuzzer.Toggle();
			m_u32WarnCounter=0;
		}
	}
	else
	{
		m_gpioAlarm.Reset();
		m_gpioBuzzer.Reset();
		m_u32WarnCounter=ACTION_WARN_TIME;
	}
}


uint16_t Action::GetOpenReason()
{
	uint32_t OpenReason;
	g_flash.read(EE_OpenReason,(uint16_t *)&OpenReason,sizeof(OpenReason)/2);
	return OpenReason;
}