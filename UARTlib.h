//Nazwa: CANlib.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR

#ifndef UARTlib_H
#define UARTlib_H

#include "stm32f4xx.h"
#include "CANlib.h"
#include "main.h"
#include "ADClib.h"

volatile char polecenie[20];
volatile char GPSdata[100];

typedef struct {
	volatile uint8_t *sendBuffor;
	volatile uint8_t dataLenght;
} sendData_TypeDef;

volatile sendData_TypeDef sendData[10];

void USART3_IRQHandler(void);
void initUart3(void);
void USART2_IRQHandler(void);
void initUart2(void);
void USART1_IRQHandler(void);
void initUart1(void);

void UART2wyslij(uint8_t* sendBuffor, uint8_t dlugosc);
void wykonajPolecenie(void);
void sendGpsData(uint8_t dlugoscRamki);

#endif
