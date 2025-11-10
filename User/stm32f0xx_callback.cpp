#include "stm32f0xx_callback.h"


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc==&g_adc.m_handle)
	{
		g_sample.CalcRMS();
	}
}


void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc==&g_adc.m_handle)
	{
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==(&g_usartRS485.m_handle))
	{
		g_commRS485.Write();
		g_commRS485.StartRecv(g_usartRS485);
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==(&g_usartRS485.m_handle))
		g_autocommRS485.SendComplete();
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart==(&g_usartRS485.m_handle))
		g_commRS485.StartRecv(g_usartRS485);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)                                                                                                                                                                                                                                                                                                            
{
	if(htim==(&g_tim26us.m_handle))
	{
//		g_ledStatus.On();		
	}
	else if(htim==(&g_tim200us.m_handle))
	{
		g_timer1ms.Increase();
		g_sample.StartConvert(g_adc);
		g_keyboard.Scan();
	} 
	else if(htim==(&g_tim10ms.m_handle))
	{

		g_timer10ms.Increase();
		g_timer20ms.Increase();
		g_timer100ms.Increase();
		g_timer200ms.Increase();
//		g_keyboard.Scan();
		g_sample.CalcReal(g_coef);
		g_elecparam.GetParam();
		g_protect.ParamSet();
		g_action.TripReset(10);
		if(g_protect.IsEnableScan())
		{
			g_protect.ProcessIR1(10);
			g_protect.ProcessIR3();
			g_protect.ProcessIR2(10);			
			g_protect.ProcessIR0(10);
			g_protect.ShuntTrip(10);
		}
	}
	else if(htim==(&g_tim500ms.m_handle))
	{
		g_timer500ms.Increase();
		g_timer1s.Increase();
	}
}


void TimerElapsedCallback(Timer * p_timer)
{
	if(p_timer==&g_timer1ms)
	{
		g_autocommRS485.Run(g_commRS485,g_usartRS485,g_gpioDE485,1);
	}
	else if(p_timer==&g_timer10ms)
	{
//		g_keyboard.Process();

//		g_action.CheckStatus(10);

	}
	else if(p_timer==&g_timer20ms)
	{

	}
	else if(p_timer==&g_timer100ms)
	{
	}
	else if(p_timer==&g_timer200ms)
	{
		g_elecparam.GetAverageParam();
	}
	else if(p_timer==&g_timer500ms)
	{
//		g_protect.ProcessLightWarn();
//		g_action.LedRun(500);

////		g_action.LedStatus();
	}
	else if(p_timer==&g_timer1s)
	{
		
	}
}
