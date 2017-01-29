//Nazwa: ADClib.h
//Autor: Konrad Aleksiejuk
//Projekt: �azik marsja�ski KNR
/*
 * Bibliotek do obs�ugi przetwornika ADC i zabezpieczenia baterii przed nadmiernym roza�dowaniem
 *
 * W momencie spadku napi�cia ponizej wartosci progowej (domy�lnie 10.5V) zmienna batteryError = 1
 * Gdy bateria jest gotowa do pracy batteryError = 0
 * Aby aktywowac biblioteke wewn�trz funkcji main nalezy umiescic funckj� initAdc();
 */
#ifndef ADClib_H
#define ADClib_H

#include "stm32f4xx.h"
#include "CANlib.h"
#include "UARTlib.h"

volatile uint8_t batteryError;
volatile uint8_t batteryAlert;
volatile uint16_t batteryAlertTime;

void initAdc(void);
void ADC_IRQHandler(void);
void AdcBatteryStatusCheck(void);
void AdcBatteryStatusSend(void);

#endif

