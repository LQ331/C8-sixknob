#ifndef __ACTION_H
#define __ACTION_H

#include "LGpio.h"


#define ACTION_TRIP_TIMEOUT 30
#define ACTION_FLAG_TIMROUT 3000


class Action
{
public:
	enum OPEN_REASON
	{
		OPEN_NONE=0,
		OPEN_IR1,
		OPEN_IR2,
		OPEN_IR3,
		OPEN_IR0,
		OPEN_COMM,
		OPEN_TEST,
	};
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
	LGpio m_gpioReset;
	LGpio m_gpioBuzzer;
	LGpio m_gpioAlarm;
	bool m_bIsActing;
	bool m_bDispActionFlag;
	uint32_t m_u32IsActionCount;
	uint32_t m_u32ActingCounter;
	uint32_t m_u32StatusCounter;
	uint32_t m_u32LedRunCounter;
	uint32_t m_u32Warn;
	uint32_t m_u32OpenReason;
	uint32_t m_u32WarnCounter;
public:
	Action();
	Action(const LGpio & gpioTrip,const LGpio & gpioReset,const LGpio & gpioBuzzer,const LGpio & gpioAlarm);
	virtual ~Action();
	void Open(OPEN_REASON emReason);
	void Warn(bool bEnable);
	void WarnNoTrip(uint16_t u16Interval,bool bEnable);
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
	uint16_t GetOpenReason();
};
#endif