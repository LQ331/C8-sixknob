#include "Timer.h"


Timer * Timer::m_sz_p_timerSchedule[TIMER_SCHEDULE_SIZE]={nullptr};


Timer::Timer()
	: m_u32Elapse(0)
	, m_callback(nullptr)
	, m_emMode(MODE_IDLE)
	, m_u32Count(0)
	, m_bRun(false)
{
}


Timer::Timer(uint32_t u32Elapse,TimerCallback callback,TIMER_MODE emMode)
	: m_u32Elapse(u32Elapse)
	, m_callback(callback)
	, m_emMode(emMode)
	, m_u32Count(0)
	, m_bRun(false)
{
}


Timer::~Timer()
{
}


void Timer::Increase()
{
	if(m_bRun)
		m_u32Count++;
}


uint32_t Timer::CheckCallbackTimes()
{
	return (m_bRun?m_u32Count/m_u32Elapse:0);
}


void Timer::Run()
{
	if(m_bRun&&m_u32Count>=m_u32Elapse)
	{
		if(m_callback)
			m_callback(this);
		switch(m_emMode)
		{
			case MODE_IDLE:m_u32Count=0;break;
			case MODE_PERIOD:m_u32Count-=m_u32Elapse;break;
			default:break;
		}
	}
}


void Timer::Start()
{
	m_bRun=true;
}


void Timer::Stop()
{
	m_bRun=false;
}


bool Timer::AddToSchedule(int nIndex)
{
	if(nIndex>=0&&nIndex<TIMER_SCHEDULE_SIZE)
	{
		m_sz_p_timerSchedule[nIndex]=this;
		return true;
	}
	else
		return false;
}


void Timer::RemoveFromSchedule()
{
	for(Timer * & p_timer:m_sz_p_timerSchedule)
	{
		if(p_timer==this)
			p_timer=nullptr;
	}
}


void Timer::Schedule()
{
	int nIndexOfMaxTimes=0;
	uint32_t u32MaxTimes=0;
	uint32_t sz_u32Times[TIMER_SCHEDULE_SIZE];
	for(int i=0;i<TIMER_SCHEDULE_SIZE;++i)
		sz_u32Times[i]=m_sz_p_timerSchedule[i]?m_sz_p_timerSchedule[i]->CheckCallbackTimes():0;
	for(int i=0;i<TIMER_SCHEDULE_SIZE;++i)
	{
		if(u32MaxTimes<sz_u32Times[i]&&m_sz_p_timerSchedule[i])
		{
			u32MaxTimes=sz_u32Times[i];
			nIndexOfMaxTimes=i;
		}
	}
	if(m_sz_p_timerSchedule[nIndexOfMaxTimes])
		m_sz_p_timerSchedule[nIndexOfMaxTimes]->Run();
}
