/* Includes ------------------------------------------------------------------*/
#include "main.h"

static __IO uint32_t timingDelay;
volatile int wartoscOpoznienia = 200;
volatile uint8_t lazikRuch = 1;

void delay(__IO uint32_t nTime);
void initGPIO(void);

//===================================================================================================
int main(void) {
	initBootloader();
	if (SysTick_Config(SystemCoreClock / 1000)) {
		/* Capture error */
		while (1)
			;
	}
	initGPIO();
	initCan();
	initUart2();
	initAdc();
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	while (1) {
		if(batteryError!=0){
		GPIO_SetBits(GPIOC, GPIO_Pin_0);
		delay(100);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		delay(100);
		}
	}
}

//==================================================================================================
/** Inicjalizacaj GPIO PC0 PC1 jako wyjœcia
 * PC0 - czerwona dioda
 * PC1 - zolta dioda
 */
void initGPIO() {

	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_StructInit(&gpio); // domyslna konfiguracja
	gpio.GPIO_Pin = GPIO_Pin_0; // konfigurujemy pin 5
	gpio.GPIO_Mode = GPIO_Mode_OUT;  //ko wyjscie
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_1; // konfigurujemy pin 5
	gpio.GPIO_Mode = GPIO_Mode_OUT;  //ko wyjscie
	GPIO_Init(GPIOC, &gpio);
}

//==================================================================================================
void delay(__IO uint32_t nTime) {
	timingDelay = nTime;

	while (timingDelay != 0)
		;
}

//==================================================================================================
void TimingDelay_Decrement(void) {
	if (timingDelay != 0) {
		timingDelay--;
	}
}

//==================================================================================================
void ResetTimer() {
	timingDelay = wartoscOpoznienia;
}

//==================================================================================================
void SysTick_Handler(void) {
	TimingDelay_Decrement();
	if (timingDelay == 0 && lazikRuch != 0) {
		sendSpeed(OBA, 128, 128, 128);
		lazikRuch = 0;
	}
}

