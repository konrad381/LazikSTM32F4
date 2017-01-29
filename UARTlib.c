#include "UARTlib.h"

//====================================================================================================
//funkcja obs³ugi przerwania od UART2
void USART2_IRQHandler(void) {
	//ODBIERANIE ZNAKÓW
	char inputChar;
	volatile static uint8_t znakiDoOdebrania = 0;
	volatile static uint8_t odbiorRamki = 0;
	volatile static uint8_t licznik = 0;
	volatile static uint8_t licznikNadawania = 1;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		inputChar = USART_ReceiveData(USART2);
		if (odbiorRamki > 0) {
			if (znakiDoOdebrania == 0) {
				switch (inputChar) {
				case 'v':
					znakiDoOdebrania = 2;
					licznik = 1;
					break;
				case 'S':
					znakiDoOdebrania = 1;
					licznik = 1;
					break;
				case 'p':
					znakiDoOdebrania = 3;
					licznik = 1;
				default:
					odbiorRamki = 0;
					break;
				}
				polecenie[0] = inputChar;
			} else {
				polecenie[licznik] = inputChar;
				licznik++;
				znakiDoOdebrania--;
			}
			if (znakiDoOdebrania == 0) {
				odbiorRamki = 0;
				wykonajPolecenie();
			}
		}

		else {
			if (inputChar == '#') {
				odbiorRamki = 1;
			}
		}
	}
	//wysylanie znaków
	if (USART_GetITStatus(USART2, USART_IT_TXE)) {
		USART_ClearITPendingBit(USART2, USART_IT_TXE);
		USART_SendData(USART2, sendBuffor[licznikNadawania]);
		licznikNadawania++;
		if (licznikNadawania == sendDataLength) {
			licznikNadawania = 1;
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	}

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

void ustawPredkosc(void) {
	static int8_t wskazanieX;
	static int8_t wskazanieY;
	wskazanieY = -polecenie[1];
	wskazanieX = -polecenie[2];
	int predkoscPrawa;
	int predkoscLewa;
	predkoscPrawa = (int) (wskazanieX) + wskazanieY;
	predkoscLewa = (int) (wskazanieX) - wskazanieY;

	if (predkoscPrawa > 100) {
		predkoscPrawa = 100;
	} else if (predkoscPrawa < -100) {
		predkoscPrawa = -100;
	}
	if (predkoscLewa > 100) {
		predkoscLewa = 100;
	} else if (predkoscLewa < -100) {
		predkoscLewa = -100;
	}
	sendSpeed(LEWA, predkoscLewa, predkoscLewa, predkoscLewa);
	sendSpeed(PRAWA, predkoscPrawa, predkoscPrawa, predkoscPrawa);
}

void wykonajPolecenie(void) {
	switch (polecenie[0]) {
	case 'v':
		ustawPredkosc();
		ResetTimer();
		break;
	case 'S': {
		ResetTimer();
		if (polecenie[1] == '1') {
			sendStop(START);
		} else {
			sendStop(STOP);
		}
		break;
		case 'p':
		ResetTimer();
		sendPid(polecenie[1], polecenie[2], polecenie[3]);
		break;
	}
	}
}


void UART2wyslij(uint8_t dlugosc) {
	sendDataLength = dlugosc;
	USART_SendData(USART2, sendBuffor[0]);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}
