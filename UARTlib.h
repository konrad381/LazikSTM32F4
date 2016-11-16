//Nazwa: CANlib.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR

#ifndef UARTlib_H
#define UARTlib_H

#include "stm32f4xx.h"
#include "CANlib.h"
#include "main.h"
#include "ADClib.h"

void USART3_IRQHandler(void);
void initUart3(void);
void initUart2(void);
void USART2_IRQHandler(void);

#endif

