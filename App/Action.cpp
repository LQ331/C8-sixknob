#include "Action.h"
#include "LClassLib.h"


Action::Action()
{
}


Action::Action(const LGpio & gpioTrip)
	: m_gpioTrip(gpioTrip)
	, m_u32Warn(WARN_NONE)
	,	m_bIsActing(false)
{
}


Action::~Action()
{
}


void Action::Open()
{
	if(!m_bIsActing)
	{
		m_gpioTrip.Set();
		m_u32ActingCounter=0;
		m_bIsActing=true;
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

	