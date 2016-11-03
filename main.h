//Nazwa: main.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx.h"
#include "CANlib.h"
#include "BootloaderLib.h"
#include "UARTlib.h"
#include "ADClib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
volatile uint8_t lazikRuch;
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void SysTick_Handler(void);
void ResetTimer(void);
void initGPIO(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
