#include "ADClib.h"


//===============================================================================
//funkcja inicjalizuj¹ca przetwornik ADC w trybie ciaglego pomiaru
//aktywuje przerwanie od ADC w momencie przekroczenia wartoœci progowych napiêc wejsciowych
//na pinie PA0
void initAdc() {
	ADC_InitTypeDef adc;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef gpio;
	batteryError = 0;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_AN;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpio);

	ADC_StructInit(&adc);
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_Resolution = ADC_Resolution_12b;
	adc.ADC_ContinuousConvMode = ENABLE;
	adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	adc.ADC_NbrOfConversion = 1;
	adc.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &adc);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);

	ADC_ContinuousModeCmd(ADC1, ENABLE);
	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

	ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_0);
	//---------------------------------------------------------------
	//Ustawienie poziomu napiêcia powoduj¹cego przerwanie.
	//1861 = 9V
	ADC_AnalogWatchdogThresholdsConfig(ADC1, 4000, 1861);
	//--------------------------------------------------------------
	ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);

	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
}

//===============================================================================
//funkcja obs³ugi przerwania od przetwornika ADC
void ADC_IRQHandler(void) {
	if (ADC_GetITStatus(ADC1, ADC_IT_AWD)) {
		ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
		batteryError = 1;
	}

}
