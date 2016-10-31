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

//definiowanie zmiennych zawieraj¹cych wiadmosci
 CanTxMsg TxMessage; //wiadomosc do wyslania
 CanRxMsg RxMessage; //wiadomosc odebrana

 //definiowanie typów wylizeniowych dla funkcji
 //kierunek obrotów silnika
 typedef enum
 {
 	Przod = 0, Tyl = 1
 } Silnik_kierunk;

//strona po której zanjduj¹ siê silniki
 typedef enum
 {
 	Prawa = -1, Oba = 0, Lewa= 1
 } Silniki_strona;

//definiowanie funkcji
 void initCan(void);   //funkcja inicjalizuj¹ca
 void CAN1_TX_IRQHandler(void);    //przerwanie po wys³aniu wiadomoœci
 void CAN1_RX0_IRQHandler(void);   //przerwanie po odebraniu wiadomoœci
 //funkcja do wysy³ania parametrów dla 3 silników znajduj¹cych siê po jednej stronie
 //void WyslijPredkosc(Silniki_strona strona, Silnik_kierunk kierunek1, int predkosc1, Silnik_kierunk kierunek2, int predkosc2, Silnik_kierunk kierunek3, int predkosc3);
 void sendSpeed(Silniki_strona strona, int predkosc1, int predkosc2,  int predkosc3);


#endif

