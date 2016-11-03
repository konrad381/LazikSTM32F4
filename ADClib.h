//Nazwa: ADClib.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR
/*
 * Bibliotek do obs³ugi przetwornika ADC i zabezpieczenia baterii przed nadmiernym roza³dowaniem
 *
 * W momencie spadku napiêcia ponizej wartosci progowej (domyœlnie 9V) zmienna batteryError = 1
 * Gdy bateria jest gotowa do pracy batteryError = 0
 * Aby aktywowac biblioteke wewn¹trz funkcji main nalezy umiescic funckjê initAdc();
 */
#ifndef ADClib_H
#define ADClib_H

#include "stm32f4xx.h"

volatile uint8_t batteryError;

void initAdc(void);
void ADC_IRQHandler(void);

#endif

