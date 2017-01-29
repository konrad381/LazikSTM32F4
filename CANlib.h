//Nazwa: CANlib.h
//Autor: Konrad Aleksiejuk
//Projekt: �azik marsja�ski KNR
/*
 * INSTRUKCJA OBS�UGI BIBLIOTEKI
 * 1)na poczatku programu musi zanlezc si� #include "CANlib.h"
 * 2)w funkcji main (poza petla glowna) musi znalezc sie CAN_Config();
 * W razie potrzeby wykorzystanie przerwa� nalezy wprowadzic odpowiednie zmiany w pliku CANlib.c
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

//definiowanie zmiennych zawieraj�cych wiadmosci
 CanTxMsg txMessage; //wiadomosc do wyslania
 CanRxMsg rxMessage; //wiadomosc odebrana

 //definiowanie typ�w wylizeniowych dla funkcji
 //kierunek obrot�w silnika
 typedef enum
 {
 	PRZOD = 0, TYL = 1
 } Silnik_kierunk;

//strona po kt�rej zanjduj� si� silniki
 typedef enum
 {
 	PRAWA = -1, OBA = 0, LEWA= 1
 } Silniki_strona;

 typedef enum{
	 STOP = 0, START = 1
 } Silnik_enable;

//definiowanie funkcji
 void initCan(void);   //funkcja inicjalizuj�ca
 void CAN1_RX0_IRQHandler(void);   //przerwanie po odebraniu wiadomo�ci
 //funkcja do wysy�ania parametr�w dla 3 silnik�w znajduj�cych si� po jednej stronie
 //void WyslijPredkosc(Silniki_strona strona, Silnik_kierunk kierunek1, int predkosc1, Silnik_kierunk kierunek2, int predkosc2, Silnik_kierunk kierunek3, int predkosc3);
 void sendSpeed(Silniki_strona strona, int predkosc1, int predkosc2,  int predkosc3);
void sendStop(Silnik_enable zezwolenie);
void sendPid(uint8_t P, uint8_t I, uint8_t K);
void sendUartParam(void);
void sendUartStartStop(void);


#endif

