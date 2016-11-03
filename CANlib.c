#include "CANlib.h"

void konwersja(int adres, Silnik_kierunk kierunek, int predkosc);

//====================================================================================================
//Funkcja inicjaluzje CAN1 na pinach PB8 i PB9
void initCan(void) {
	//inicjalizacja struktur
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	//inicjalizacja przerwañ

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//uruchomienie zegarów
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	//konfiguracja pinów CAN  PB8 CAN_RX/ PB9 CAN_TX
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
	CAN_InitStructure.CAN_NART = ENABLE; //po wprowadzeniu wadomoœci do rejestru wiadomosc wysy³ana jest tylko jeden raz
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	//ustawienie predkosc tarnsmijsi
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 5;

	//konfiguracja Filtrów CAN
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x24A0;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

	//inicjalizacja CAN i filtrów
	CAN_DeInit(CAN1);
	//------------------------------------------------------------
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
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
//Obs³uga przerwania po odbierze
void CAN1_RX0_IRQHandler(void) {
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}
}
//====================================================================================================
//Funkcja wysy³a prêdkosc do sterownika silnikow
//Argumenty:
//Silnik_strona: Lewa, Prawa, Oba
//Predkosc: liczba w zakresie 0-255 gdzie 0==max predkosc do ty³u, 128==STOP, 255==max predkosc do przodu
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
	txMessage.Data[0] = predkosc1;
	txMessage.Data[1] = predkosc2;
	txMessage.Data[2] = predkosc3;
	CAN_Transmit(CAN1, &txMessage);
}

//====================================================================================================
//konwertuje 16bit wartosc predkosci i jedno bitowa wartosc kierunku na dwa bajty i wpisuje w dwa kolejne bajty wiadomoœci TX
//funkcja pomocnicza obecnie nie wykorzystywana
//Adres: numer bajtu w wiadomoœci wys³anej za pomoc¹ CAN od którego funkcja zacznie wpisywanie danych (wartoœc od 0 do 7)
//Kierunek: 	Przod, Tyl
//Predkosc: wartosc predkosci od 0 do 2^16
void konwersja(int adres, Silnik_kierunk kierunek, int predkosc) {
	txMessage.Data[adres] = ((predkosc & 0xFE) | (kierunek & 01));
	txMessage.Data[adres + 1] = predkosc >> 8;
}

