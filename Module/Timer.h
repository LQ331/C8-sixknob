#ifndef __TIMER_H
#define __TIMER_H

#include "LGpio.h"
class Timer;

#ifdef __cplusplus
 extern "C" {
#endif
typedef void (*TimerCallback)(Timer * p_timer);
#ifdef __cplusplus
}
#endif

#define TIMER_SCHEDULE_SIZE 10

class Timer
{
public:
	enum TIMER_MODE
	{
		MODE_IDLE=0,
		MODE_PERIOD
	};
private:
	uint32_t m_u32Elapse;
	TimerCallback m_callback; 
	TIMER_MODE m_emMode;
	uint32_t m_u32Count;
	bool m_bRun;
	static Timer * m_sz_p_timerSchedule[TIMER_SCHEDULE_SIZE];
public:
	Timer();
	Timer(uint32_t u32Elapse,TimerCallback callback=nullptr,TIMER_MODE emMode=MODE_IDLE);
	virtual ~Timer();
	void Increase();
	uint32_t CheckCallbackTimes();
	void Run();
	void Start();
	void Stop();
	bool AddToSchedule(int nIndex);
	void RemoveFromSchedule();
	static void Schedule();
};

#endif