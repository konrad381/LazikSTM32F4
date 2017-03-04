#include "UARTlib.h"

//========================================================================================
/**
 * @brief  inicjalizacja UART2 na pinach TX PA2, RX PA3
 * @note   Uart wykorzystywany do komunikacji z jednostk¹ steruj¹ca
 * 		Baudrate 115200, 8bit, 1 stop, parity no
 * 		Inicjalizacja przerwania odbiorczego USART2_IRQHandler
 * @retval None
 */
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
/**
 * @brief  funkcja obs³ugi przerwania od UART2
 * @note   zarówno przerwania odbiorcze i nadawcze
 * 		odbieranie ramki rozpoczyna znak "#", wiadomosc jest rozpoznawana na
 * 		podstawie drugiego znaku i okreslana jest liczba znakow do odebrania.
 * 		Po odebraniu calej ramki polecenie jest wykonywane.
 * @retval None
 */
void USART2_IRQHandler(void) {
	//ODBIERANIE ZNAKÓW
	char inputChar;
	volatile static uint8_t znakiDoOdebrania = 0;
	volatile static uint8_t odbiorRamki = 0;
	volatile static uint8_t licznik = 0;
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
					znakiDoOdebrania = 4;
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

		static volatile  int k=0;
		static volatile  uint8_t trwaWysylanie=0;

		if (trwaWysylanie == 1) {
			USART_SendData(USART2, *sendData[k].sendBuffor);
			sendData[k].sendBuffor++;
			sendData[k].dataLenght--;
			if (sendData[k].dataLenght == 0) {
				trwaWysylanie = 0;
			}
		}
		if (trwaWysylanie == 0) {
			for (k = 0; k < 9 && trwaWysylanie == 0; ) {
				if (sendData[k].dataLenght > 0) {
					USART_SendData(USART2, *sendData[k].sendBuffor);
					sendData[k].sendBuffor++;
					sendData[k].dataLenght--;
					trwaWysylanie = 1;
				}else{
					k++;
				}
			}
			if (k >= 9) {
				k = 0;
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			}
		}
	}
}

//====================================================================================================
/**
 * @brief  Funkcja inicjaluzuj¹ca UART3 na pinach TX PC10, RX PC11;
 * @note   Uart niewykorzystywany (zapasowy)
 * 		Baudrate 115200, 8bit, 1 stop, parity no
 * 		Inicjalizacja przerwania odbiorczego USART3_IRQHandler
 * @retval None
 */
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

//====================================================================================================
/**
 * @brief  funkcja obs³ugi przerwania od UART3
 * @note   obenie nie uzywane
 * @retval None
 */
void USART3_IRQHandler(void) {
//ODBIERANIE ZNAKÓW
	//char inputChar;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		//	inputChar = USART_ReceiveData(USART3);
	}
}

//==============================================================================
/**
 * @brief  inicjalizacja UART1 na pinach TX PA9, RX PA10
 * @note   Uart wykorzystywany do komunikacji z modu³em GPS
 * 		Baudrate 4800, 8bit, 1 stop, parity no
 * 		Inicjalizacja przerwania odbiorczego USART1_IRQHandler
 * @retval None
 */
void initUart1(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Enable clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Enable clock for USART2 peripheral

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Initialize NVIC*/
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	USART_InitStruct.USART_BaudRate = 4800; //baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Enable RX interrupt

	USART_Cmd(USART1, ENABLE);
}

//==============================================================================
/**
 * @brief  funkcja obs³ugi przerwania od UART1
 * @note   zarówno przerwania odbiorcze i nadawcze (wykorzystywane tylko odbiorcze)
 * 		odbieranie ramki rozpoczyna znak "$", a koñcz¹cej znakiem "*"
 * 		i wpisanie do GPSdata[]
 * @retval None
 */
void USART1_IRQHandler(void) {
	char inputChar;
	volatile static uint8_t odbiorRamki = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		inputChar = USART_ReceiveData(USART1);
		if (odbiorRamki > 0) {
			GPSdata[odbiorRamki] = inputChar;
			odbiorRamki++;
			if (inputChar == '*') {
				sendGpsData(odbiorRamki);
				odbiorRamki = 0;

			}
		} else if (inputChar == '$') {
			GPSdata[0] = '$';
			odbiorRamki = 1;
		}
	}
}

//===================================================================================
/**
 * @brief  funkcja oblicza i wysyla zadana predkosc do silników po obu stronach
 * 		na podstawie wartosci otrzymanych ze jednostki sterujacej
 * @note   wartosc obliczana na podstawie wartosci wychylenia joysticka
 * 		w obu osiach X i Y wartosc od -100 do 100
 * @retval None
 */
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

//===============================================================================
/**
 * @brief  funkcja okresla na podstawie pierwszego znaku w linii jak interpretowac dane polecenie.
 * @note   rozpoznawane wartosci: v - ustawienie predkosci
 * 							   S - start/stop silników
 * 							   p - nastawy regulatorów PID
 * @retval None
 */
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
		sendPid(polecenie[1], polecenie[2], polecenie[3], polecenie[4]);
		break;
	}
	}
}

//===============================================================================
/**
 * @brief  funkcja wysyla dane ze wskazanego adresu tablicy do jednostki sterujacej za pomoca UART2
 * @note   funkcja wpisuje adres pierwszego znaku w tablicy i d³ugosc tabicy do tablicy struktur,
 * 		   która przechowuje dane i wysyla je pokolei( dane nie sa nadpisywane ani tracone)
 * @param 	dataToSend - wskaŸnik do pierwszego elementu tablicy zawieraj¹cej dane do wyslania
 * @param 	dlugosc - liczba znaków które maj¹ zostac wyslane od(1 do 99)
 * @retval None
 */
void UART2wyslij(uint8_t* dataToSend, uint8_t dlugosc) {
	uint8_t koniec = 0;
	for (int l = 0; l < 10 && koniec == 0; l++) {
		if (sendData[l].dataLenght == 0) {
			sendData[l].dataLenght = dlugosc;
			sendData[l].sendBuffor = dataToSend;
			koniec = 1;
		}
	}
	if (USART_GetITStatus(USART2, USART_IT_TXE) == RESET) {
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	}
}

//===============================================================================
/**
 * @brief  funkcja interpretuje dane z GPS i przesy³a je do jednostki sterujacej
 * @note   dane GPS s¹ odczytywane z GPSdata[]
 * @param 	dlugoscRamki - dlugosc odebranej ramki danych GPS która ma zostac zinterpretowana
 * @retval None
 */
void sendGpsData(uint8_t dlugoscRamki) {
	static uint8_t sendBuffor[52];
	if (GPSdata[3] == 'G' && GPSdata[4] == 'G' && GPSdata[5] == 'A'
			&& dlugoscRamki >= 45) {
		sendBuffor[0]='#';
		sendBuffor[1]='G';
		for (int i = 0; i < 49; i++) {
					sendBuffor[i+2] = GPSdata[i];
		}
			UART2wyslij(&sendBuffor[0],50);
	}
}

