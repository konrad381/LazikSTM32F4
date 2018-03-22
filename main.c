#include "main.h"
//Adas to byl
//===================================================================================================
int main(void) {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	initBootloader();
	if (SysTick_Config(SystemCoreClock / 1000)) {
		/* Capture error */
		while (1)
			;
	}
	initGPIO();
	initCan();
	initUart2();
	initUart1();
	initAdc();
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	lazikRuch = 1;
	while (1) {
		if (batteryError != 0) {
			GPIO_SetBits(GPIOC, GPIO_Pin_0);
			delay(100);
			GPIO_ResetBits(GPIOC, GPIO_Pin_0);
			delay(100);
		}
	}
}

//==================================================================================================
/**
 * @brief  Inicjalizacaj GPIO PC0 PC1 jako wyjœcia
 * @note  	Inicjalizacja pinów pod³¹czonych do diod LED jako wyjœcia
 * 			PC0 - czerwona dioda
 * 			PC1 - zolta dioda
 * @retval None
 */
void initGPIO() {

	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_StructInit(&gpio); // domyslna konfiguracja
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &gpio);
}

//==================================================================================================
/**
 * @brief  funkcja relizujaca opóŸnienie
 * @note   funkcja wykorzystuje systick do dok³adengo okreslenia czasu
 * @param  nTime wartosc okresljajaca opoznienie w ms (od 0 do 32bit)
 * @retval None
 */
void delay(__IO uint32_t nTime) {
	opoznienie = nTime;

	while (opoznienie != 0)
		;
}

//==================================================================================================
/**
 * @brief  funkcja resetujaca Timer
 * @note   funkcja wykorzystuje systick do dok³adengo okreslenia czasu
 * 		   funkcja uzywana do testowania komunikacji jesli wartosc timingDelay
 * 		   spadnie do 0 lazik zatrzyma sie
 * @retval None
 */
void ResetTimer() {
	lazikRuch = 1;
	timingDelay = wartoscOpoznienia;
}

//==================================================================================================
/**
 * @brief  przerwanie od Systick co 1ms
 * @note   synchronizuje czasowo inne funkcje
 * @retval None
 */
void SysTick_Handler(void) {
	if (timingDelay != 0) {
		timingDelay--;
	}
	if (opoznienie != 0) {
		opoznienie--;
	}
	if (timingDelay == 0 && lazikRuch != 0) {
		sendStop(STOP);
		lazikRuch = 0;
	}
	AdcBatteryStatusCheck();
	AdcBatteryStatusSend();
}

