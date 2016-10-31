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

//definiowanie zmiennych zawieraj�cych wiadmosci
 CanTxMsg TxMessage; //wiadomosc do wyslania
 CanRxMsg RxMessage; //wiadomosc odebrana

 //definiowanie typ�w wylizeniowych dla funkcji
 //kierunek obrot�w silnika
 typedef enum
 {
 	Przod = 0, Tyl = 1
 } Silnik_kierunk;

//strona po kt�rej zanjduj� si� silniki
 typedef enum
 {
 	Prawa = -1, Oba = 0, Lewa= 1
 } Silniki_strona;

//definiowanie funkcji
 void initCan(void);   //funkcja inicjalizuj�ca
 void CAN1_TX_IRQHandler(void);    //przerwanie po wys�aniu wiadomo�ci
 void CAN1_RX0_IRQHandler(void);   //przerwanie po odebraniu wiadomo�ci
 //funkcja do wysy�ania parametr�w dla 3 silnik�w znajduj�cych si� po jednej stronie
 //void WyslijPredkosc(Silniki_strona strona, Silnik_kierunk kierunek1, int predkosc1, Silnik_kierunk kierunek2, int predkosc2, Silnik_kierunk kierunek3, int predkosc3);
 void sendSpeed(Silniki_strona strona, int predkosc1, int predkosc2,  int predkosc3);


#endif

