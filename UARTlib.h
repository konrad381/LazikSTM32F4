//Nazwa: CANlib.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR

#ifndef UARTlib_H
#define UARTlib_H

#include "stm32f4xx.h"
#include "CANlib.h"
#include "main.h"
#include "ADClib.h"

volatile char polecenie[10];

uint8_t sendBuffor[10];
uint8_t sendDataLength;

void UART2wyslij(uint8_t dlugosc);

void wykonajPolecenie(void);

void USART3_IRQHandler(void);
void initUart3(void);
void USART2_IRQHandler(void);
void initUart2(void);
#endif

