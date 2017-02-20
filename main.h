//Nazwa: main.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "stm32f4xx.h"
#include "CANlib.h"
#include "BootloaderLib.h"
#include "UARTlib.h"
#include "ADClib.h"

#define wartoscOpoznienia 200

volatile uint8_t lazikRuch;
static volatile uint32_t timingDelay;
static volatile uint32_t opoznienie;


void delay(uint32_t nTime);
void SysTick_Handler(void);
void ResetTimer(void);
void initGPIO(void);

#endif
