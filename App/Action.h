#ifndef __ACTION_H
#define __ACTION_H

#include "LGpio.h"


#define ACTION_TRIP_TIMEOUT 30
#define ACTION_FLAG_TIMROUT 3000


class Action
{
public:
	enum WARN_REASON
		{
			WARN_NONE=0,
			WARN_IR1=0x00000001,			//过流
			WARN_IR2=0x00000002,			//短路短延时
			WARN_IR3=0x00000004,			//短路瞬时	
			WARN_IR0=0x00000008,
		};
private:
	LGpio m_gpioStatus;
	LGpio m_gpioTrip;
	bool m_bIsActing;
	bool m_bDispActionFlag;
	uint32_t m_u32IsActionCount;
	uint32_t m_u32ActingCounter;
	uint32_t m_u32StatusCounter;
	uint32_t m_u32LedRunCounter;
	uint32_t m_u32Warn;
public:
	Action();
	Action(const LGpio & gpioTrip);
	virtual ~Action();
	void Open();
	void Warn(bool bEnable);
	bool IsActing();
	uint32_t GetWarn();
	void TripReset(uint16_t u16Interval);
	void CheckStatus(uint16_t u16Interval);
	void LedRun(uint16_t u16Interval);
	void LedEarly();
	void LedStatus();
	void LedAlarm();
	void LedComm(bool bIsCommunicating);
	LGpio & GetGpioStatus();
	LGpio & GetGpioTrip();
};
#endif