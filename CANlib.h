//Nazwa: CANlib.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR
/*
 * INSTRUKCJA OBS£UGI BIBLIOTEKI
 * 1)na poczatku programu musi zanlezc siê #include "CANlib.h"
 * 2)w funkcji main (poza petla glowna) musi znalezc sie CAN_Config();
 * W razie potrzeby wykorzystanie przerwañ nalezy wprowadzic odpowiednie zmiany w pliku CANlib.c
 *
 * Adresowanie:
 *  Wiadomosc do wszystkich				0x00
 *  Wiadomosc do sterowanika prawego 	0x123
 *  Wiadomosc do sterowanika lewego 	0x124
 *  Adres sterownika glownego			0x125
 */

#ifndef CANlib_H
#define CANlib_H

#include "stm32f4xx.h"
#include "UARTlib.h"

//definiowanie zmiennych zawieraj¹cych wiadmosci
CanTxMsg txMessage; //wiadomosc do wyslania
CanRxMsg rxMessage; //wiadomosc odebrana

//definiowanie typów wyliczeniowych dla funkcji
//kierunek obrotów silnika
typedef enum {
	PRZOD = 0, TYL = 1
} Silnik_kierunk;

//strona po której zanjduj¹ siê silniki
typedef enum {
	PRAWA = -1, OBA = 0, LEWA = 1
} Silniki_strona;

//stan silnika w³¹czyc START, wy³¹czyc STOP
typedef enum {
	STOP = 0, START = 1
} Silnik_enable;

//definiowanie funkcji
void initCan(void);   //funkcja inicjalizuj¹ca
void CAN1_RX0_IRQHandler(void);   //przerwanie po odebraniu wiadomoœci
void sendSpeed(Silniki_strona strona, int predkosc1, int predkosc2,
		int predkosc3);
void sendStop(Silnik_enable zezwolenie);
void sendPid(uint8_t P, uint8_t Ilow, uint8_t Ihigh, uint8_t K);
void sendUartParam(void);
void sendUartStartStop(void);

#endif
