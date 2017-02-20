#include "CANlib.h"

//====================================================================================================
/**
  * @brief  Funkcja inicjaluzje CAN1 na pinach PB8 i PB9
  * @note   Adresowanie:
 *  		Wiadomosc do wszystkich				0x00
 * 			Wiadomosc do sterowanika prawego 	0x123
 *  		Wiadomosc do sterowanika lewego 	0x124
 *  		Adres sterownika glownego			0x125
 *  		Adresowanie w inicjalizacji filtru
 *  		Wysyłanie wiadomosci bez przerwania
 *  		Odbiór za pomoca przerwania CAN1_RX0_IRQHandler
  * @retval None
  */
void initCan(void) {
	//inicjalizacja struktur
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	//inicjalizacja przerwa�
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//uruchomienie zegar�w
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	//konfiguracja pin�w CAN  PB8 CAN_RX/ PB9 CAN_TX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//konfiguracja CAN
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE; //po wprowadzeniu wadomo�ci do rejestru wiadomosc wysy�ana jest tylko jeden raz
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	//ustawienie predkosc tarnsmijsi
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 5;

	//konfiguracja Filtr�w CAN
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x24A0;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

	//inicjalizacja CAN i filtr�w
	CAN_DeInit(CAN1);
	//------------------------------------------------------------
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_Init(CAN1, &CAN_InitStructure);
	CAN_FilterInit(&CAN_FilterInitStructure);
	/* Transmit Structure preparation */
	txMessage.StdId = 0x00;
	txMessage.ExtId = 0x00;
	txMessage.RTR = CAN_RTR_DATA;
	txMessage.IDE = CAN_ID_STD;
	txMessage.DLC = 3;

}

//====================================================================================================
/**
  * @brief  Obsługa przerwanie oddbiorczego
  * @note   Okreslenie typu wiadomosci za pomoca dlugosci wiadomosci (rxMessage.DLC)
  * @retval None
  */
void CAN1_RX0_IRQHandler(void) {
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rxMessage);
		switch (rxMessage.DLC) {
		case 7:
			sendUartParam();
			break;
		case 2:
			sendUartStartStop();
			break;
		}
	}
}
//====================================================================================================
/**
  * @brief  Funkcja wysy�a pr�dkosc do sterownika silnikow
  * @note   wysyla zadane predkosci do wszystkich trzech silników
  * 		sterowanych przez wybrany sterownik
  * @param  strona : Lewa - wyslanie do lewego sterownika
  * 				 Prawa - wyslanie do prawego sterowanika
  * 				 Oba - wyslanie do obu sterownikow
  * @param predkosc(1/2/3): watrosc predkosci kolejnych silnikow
  * 						liczba w zakresie -100, 100
  * @retval None
  */
void sendSpeed(Silniki_strona strona, int predkosc1, int predkosc2,
		int predkosc3) {
	switch (strona) {
	case PRAWA:
		txMessage.StdId = 0x123;
		break;
	case OBA:
		txMessage.StdId = 0x00;
		break;
	case LEWA:
		txMessage.StdId = 0x124;
		break;
	}
	txMessage.DLC = 4;
	txMessage.Data[0] = 'v';
	txMessage.Data[1] = predkosc1;
	txMessage.Data[2] = predkosc2;
	txMessage.Data[3] = predkosc3;
	CAN_Transmit(CAN1, &txMessage);
}

//====================================================================================================
/**
  * @brief  Funkcja uruchamia lub zatrzymuje silniki (generowanie PWM w sterowniku)
  * @note   zatrzymanie silinków oznacza ich wyłączenie, pojazd moze zaczac się poruszc
  * 		pod wlacnym ciezarem. Funkcja wysyla polecenie zatrzymania po magistrali CAN
  * 		w razie uszkodzenia magistrali funkcja nie zatrzyma silników.
  * @param  zezwolenie: STOP - silnik zostaje zatrzymany
  * 					START - silnik zostaje uruchomiony
  * @retval None
  */
void sendStop(Silnik_enable zezwolenie) {
	txMessage.StdId = 0x00;
	txMessage.DLC = 2;
	txMessage.Data[0] = 's';
	txMessage.Data[1] = zezwolenie;
	CAN_Transmit(CAN1, &txMessage);
}

//====================================================================================================
/**
  * @brief  Funkcja ustawia nastawy regulatora PI oraz warosc K wzmocnienia antywindup'owego
  * @note  	po zmianie nastawów pojazd zostaje automatycznie zatrzymany, trzeba ponownie uruchomic napedy
  * @param  P - wartosc wzmocnienia czlonu P (od 0 do 255)
  * @param 	Ilow - stala czasowa członu calkujacego(czesc niska wartosci 16 bitowej)(od 0 do 65535)
  * @param 	Ihigh - stala czasowa członu calkujacego(czesc wysok wartosci 16 bitowej)(od 0 do 65535)
  * @param  K - wartosc wzmocnienia roznicy sygnalu przed i po nasyceniu (antywindup)(od 0 do 255)
  * @retval None
  */
void sendPid(uint8_t P, uint8_t Ilow, uint8_t Ihigh, uint8_t K) {
	txMessage.StdId = 0x00;
	txMessage.DLC = 5;
	txMessage.Data[0] = 'p';
	txMessage.Data[1] = P;
	txMessage.Data[2] = Ilow;
	txMessage.Data[3] = Ihigh;
	txMessage.Data[4] = K;
	CAN_Transmit(CAN1, &txMessage);
}

//====================================================================================================
/**
  * @brief  Przesyla otrzymane parametry predkosci i pradu z silnika (przez CAN) do sterowanika (przez UART2)
  * @note  	funkcja w nie modyfikuje otrzymanych danych a jedynie przesyła je dalej
  * 		dane są odczytywane bezpośrednioa z rxMessage.Data[]
  * @retval None
  */
void sendUartParam(void) {
	sendBuffor[0] = '#';
	for (int i = 0; i <= 7; i++) {
		sendBuffor[i + 1] = rxMessage.Data[i];
	}
	UART2wyslij(8);
}

//====================================================================================================
/**
  * @brief  Przesyla potwierdzenie wykonania polecenia sendStop() do jednostki sterujacej
  * @note  	na chwile obecna funkcja nie aktywna
  * @retval None
  */
void sendUartStartStop(void) {

}

