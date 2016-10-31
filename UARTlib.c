#include "UARTlib.h"
#include "CANlib.h"
#include "main.h"

volatile char c;
volatile uint8_t wysylane_buf[100];
volatile uint8_t j = 0, poczatek_buf = 0, koniec_buf = 0;
//volatile uint16_t adc_value;
/* Funkcja oblugi przerwan */
volatile int ramka = 0;

//====================================================================================================
//funkcja obs³ugi przerwania od UART3
void USART3_IRQHandler(void) {
	//ODBIERANIE ZNAKÓW
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		c = USART_ReceiveData(USART3);

		switch (c) {
		case 'w':
			ResetTimer();
			sendSpeed(Oba, 180, 180, 180);
			break;
		case 's':
			ResetTimer();
			sendSpeed(Oba, 74, 74, 74);
			break;
		case 'd':
			ResetTimer();
			sendSpeed(Prawa, 15, 15, 15);
			sendSpeed(Lewa, 240, 240, 240);
			break;
		case 'a':
			ResetTimer();
			sendSpeed(Lewa, 15, 15, 15);
			sendSpeed(Prawa, 240, 240, 240);
			break;
		case 'W':
			ResetTimer();
			sendSpeed(Oba, 255, 255, 255);
			break;
		case 'S':
			ResetTimer();
			sendSpeed(Oba, 1, 1, 1);
			break;
		case 'D':
			ResetTimer();
			sendSpeed(Prawa, 1, 1, 1);
			sendSpeed(Lewa, 255, 255, 255);
			break;
		case 'A':
			ResetTimer();
			sendSpeed(Lewa, 1, 1, 1);
			sendSpeed(Prawa, 255, 255, 255);

		}
	}
}

//====================================================================================================
//Funkcja inicjaluzuj¹ca UART3 na pinach PC10 PC11;
void initUart3(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //Enable clock for GPIOC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //Enable clock for USART2 peripheral

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Initialize NVIC*/
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	USART_InitStruct.USART_BaudRate = 9600; //baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStruct);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //Enable RX interrupt

	USART_Cmd(USART3, ENABLE);
}
