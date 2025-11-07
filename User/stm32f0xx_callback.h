#ifndef __STM32F0xx_CALLBACK_H
#define __STM32F0xx_CALLBACK_H

#include "LClassLib.h"

#ifdef __cplusplus
 extern "C" {
#endif

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void TimerElapsedCallback(Timer * p_timer);
#ifdef __cplusplus
}
#endif

#endif
