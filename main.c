/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "CANlib.h"
#include "BootloaderLib.h"
#include "UARTlib.h"

static __IO uint32_t TimingDelay;
volatile int wartoscDomyslna = 200;

void GPIO_init();
void Delay(__IO uint32_t nTime);
int i = 0;

//===================================================================================================
int main(void) {
// sprawdzenie taktowania zegara
	initBootloader();
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	if (SysTick_Config(SystemCoreClock / 1000)) {
		/* Capture error */
		while (1)
			;
	}
	GPIO_init();
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	initCan();
	initUart3();
	while (1) {
		GPIO_SetBits(GPIOC, GPIO_Pin_0);
		Delay(100);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		Delay(100);
	}
}

//==================================================================================================
/** Inicjalizacaj GPIO PC0 PC1 jako wyjœcia
 * PC0 - czerwona dioda
 * PC1 - zolta dioda
 */
void GPIO_init() {

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
void Delay(__IO uint32_t nTime) {
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;
}

//==================================================================================================
void TimingDelay_Decrement(void) {
	if (TimingDelay != 0) {
		TimingDelay--;
	}
}

//==================================================================================================
void ResetTimer() {
	TimingDelay = wartoscDomyslna;
}

//==================================================================================================
void SysTick_Handler(void) {
	TimingDelay_Decrement();
	if (TimingDelay == 0) {
		sendSpeed(Oba, 128, 128, 128);
	}
}

