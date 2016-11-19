#include "UARTlib.h"

//====================================================================================================
//funkcja obs³ugi przerwania od UART3
void USART2_IRQHandler(void) {
	//ODBIERANIE ZNAKÓW
	char inputChar;
	uint8_t static blokujJoystick = 0;
	volatile static uint8_t sterowanieJoystick = 0;
	volatile static uint8_t licznik = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		inputChar = USART_ReceiveData(USART2);
		if (batteryError == 0) {
			if (sterowanieJoystick == 1) {
				static int8_t wskazanieX;
				static int8_t wskazanieY;
				if (licznik == 0) {
					wskazanieY = -inputChar;
					licznik++;
				} else if (licznik == 1) {
					wskazanieX = -inputChar;
					licznik = 0;
					sterowanieJoystick = 0;
					int predkoscPrawa;
					int predkoscLewa;
					predkoscPrawa = (int) (wskazanieX * 1.28 + 127)
							+ wskazanieY * 1.27;
					predkoscLewa = (int) (wskazanieX * 1.28 + 127)
							- wskazanieY * 1.27;

					if (predkoscPrawa >= 255) {
						predkoscPrawa = 254;
					} else if (predkoscPrawa <= 0) {
						predkoscPrawa = 1;
					}
					if (predkoscLewa >= 255) {
						predkoscLewa = 254;
					} else if (predkoscLewa <= 0) {
						predkoscLewa = 1;
					}
					if (blokujJoystick == 0) {
						sendSpeed(LEWA, predkoscLewa, predkoscLewa,
								predkoscLewa);
						sendSpeed(PRAWA, predkoscPrawa, predkoscPrawa,
								predkoscPrawa);
					}
					blokujJoystick = 0;
				}
			} else {
				switch (inputChar) {
				case 'w':
					ResetTimer();
					sendSpeed(OBA, 180, 180, 180);
					lazikRuch = 1;
					break;
				case 's':
					ResetTimer();
					sendSpeed(OBA, 74, 74, 74);
					lazikRuch = 1;
					break;
				case 'd':
					ResetTimer();
					sendSpeed(PRAWA, 15, 15, 15);
					sendSpeed(LEWA, 240, 240, 240);
					lazikRuch = 1;
					break;
				case 'a':
					ResetTimer();
					sendSpeed(LEWA, 15, 15, 15);
					sendSpeed(PRAWA, 240, 240, 240);
					lazikRuch = 1;
					break;
				case 'W':
					ResetTimer();
					sendSpeed(OBA, 255, 255, 255);
					lazikRuch = 1;
					break;
				case 'S':
					ResetTimer();
					sendSpeed(OBA, 1, 1, 1);
					lazikRuch = 1;
					break;
				case 'D':
					ResetTimer();
					sendSpeed(PRAWA, 1, 1, 1);
					sendSpeed(LEWA, 255, 255, 255);
					lazikRuch = 1;
					break;
				case 'A':
					ResetTimer();
					sendSpeed(LEWA, 1, 1, 1);
					sendSpeed(PRAWA, 255, 255, 255);
					lazikRuch = 1;
					break;
				case 'v':
					ResetTimer();
					sterowanieJoystick = 1;
					lazikRuch = 1;
					break;
				case ' ':
					sendSpeed(OBA, 128, 128, 128);
					lazikRuch = 0;
					break;
				case 'e':
					ResetTimer();
					sendSpeed(PRAWA, 15, 15, 15);
					sendSpeed(LEWA, 240, 240, 240);
					lazikRuch = 1;
					blokujJoystick = 1;
					break;
				case 'q':
					ResetTimer();
					sendSpeed(LEWA, 15, 15, 15);
					sendSpeed(PRAWA, 240, 240, 240);
					lazikRuch = 1;
					blokujJoystick = 1;
					break;
				}
			}
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

	USART_InitStruct.USART_BaudRate = 115200; //baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStruct);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //Enable RX interrupt

	USART_Cmd(USART3, ENABLE);
}

void initUart2(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Enable clock for GPIOC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //Enable clock for USART2 peripheral

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Initialize NVIC*/
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	USART_InitStruct.USART_BaudRate = 115200; //baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Enable RX interrupt

	USART_Cmd(USART2, ENABLE);
}

void USART3_IRQHandler(void) {
	//ODBIERANIE ZNAKÓW
	char inputChar;
	volatile static uint8_t sterowanieJoystick = 0;
	volatile static uint8_t licznik = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		inputChar = USART_ReceiveData(USART3);
		if (batteryError == 0) {
			if (sterowanieJoystick == 1) {
				static int8_t wskazanieX;
				static int8_t wskazanieY;
				if (licznik == 0) {
					wskazanieY = -inputChar;
					licznik++;
				} else if (licznik == 1) {
					wskazanieX = -inputChar;
					licznik = 0;
					sterowanieJoystick = 0;
					int predkoscPrawa;
					int predkoscLewa;
					if (wskazanieX <= 0) {
						wskazanieY = -wskazanieY;
					}
					predkoscPrawa = (int) (wskazanieX * 1.28 + 127)
							+ wskazanieY * 1.27;
					predkoscLewa = (int) (wskazanieX * 1.28 + 127)
							- wskazanieY * 1.27;

					if (predkoscPrawa >= 255) {
						predkoscPrawa = 254;
					} else if (predkoscPrawa <= 0) {
						predkoscPrawa = 1;
					}
					if (predkoscLewa >= 255) {
						predkoscLewa = 254;
					} else if (predkoscLewa <= 0) {
						predkoscLewa = 1;
					}
					sendSpeed(LEWA, predkoscLewa, predkoscLewa, predkoscLewa);
					sendSpeed(PRAWA, predkoscPrawa, predkoscPrawa,
							predkoscPrawa);
				}
			} else {
				switch (inputChar) {
				case 'w':
					ResetTimer();
					sendSpeed(OBA, 180, 180, 180);
					lazikRuch = 1;
					break;
				case 's':
					ResetTimer();
					sendSpeed(OBA, 74, 74, 74);
					lazikRuch = 1;
					break;
				case 'd':
					ResetTimer();
					sendSpeed(PRAWA, 15, 15, 15);
					sendSpeed(LEWA, 240, 240, 240);
					lazikRuch = 1;
					break;
				case 'a':
					ResetTimer();
					sendSpeed(LEWA, 15, 15, 15);
					sendSpeed(PRAWA, 240, 240, 240);
					lazikRuch = 1;
					break;
				case 'W':
					ResetTimer();
					sendSpeed(OBA, 255, 255, 255);
					lazikRuch = 1;
					break;
				case 'S':
					ResetTimer();
					sendSpeed(OBA, 1, 1, 1);
					lazikRuch = 1;
					break;
				case 'D':
					ResetTimer();
					sendSpeed(PRAWA, 1, 1, 1);
					sendSpeed(LEWA, 255, 255, 255);
					lazikRuch = 1;
					break;
				case 'A':
					ResetTimer();
					sendSpeed(LEWA, 1, 1, 1);
					sendSpeed(PRAWA, 255, 255, 255);
					lazikRuch = 1;
					break;
				case 'v':
					ResetTimer();
					sterowanieJoystick = 1;
					lazikRuch = 1;
					break;
				case ' ':
					sendSpeed(OBA, 128, 128, 128);
					lazikRuch = 0;
					break;
				case 'e':
					batteryError = 0;
					break;
				}
			}
		}
	}
}
