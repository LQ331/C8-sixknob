#ifndef __LCLASSLIB_H__
#define __LCLASSLIB_H__

#include "string.h"
#include "LGpio.h"
#include "Led.h"
#include "LTim.h"
#include "Timer.h"
#include "Knob.h"
#include "Keyboard.h"
#include "LAdc.h"
#include "LDma.h"
#include "Coefficient.h"
#include "Sample.h"
#include "Flash.h"
#include "LSpi.h"
#include "ElecParam.h"
#include "Protect.h"
#include "Action.h"
#include "Device.h"
#include "LUsart.h"
#include "Modbus.h"
#include "LIwdg.h"
#include "AutoComm.h"
#include "communication.h"
#include "Protocol.h"

#define VERSION 7014



extern LGpio g_gpioDE485,g_ShuntTrip;
extern Led g_ledRun,g_ledEarly,g_ledStatus,g_ledAlarm,g_ledComm;
extern LTim g_tim26us,g_tim200us,g_tim10ms,g_tim500ms;
extern Timer g_timer1ms,g_timer10ms,g_timer20ms,g_timer100ms,g_timer200ms,g_timer500ms,g_timer1s;
extern Knob g_knob;
extern Keyboard g_keyboard;
extern LDma g_dmaADC;
extern LAdc g_adc;
extern Protect g_protect;
extern Sample g_sample;
extern Coefficient g_coef;
extern Flash g_flash;
extern FRAME_CURRENT g_emFrameCurrent;
extern ElecParam g_elecparam;
extern Action g_action;
extern Device g_device;
extern AutoComm g_autocommRS485;
extern LUsart g_usartRS485;
extern Communication g_commRS485;
extern Modbus g_modbusRS485;
extern LIwdg g_iwdg;
extern uint32_t g_u32Version;
extern bool g_bInitFinish;


#endif