//Nazwa: BootloaderLib.h
//Autor: Konrad Aleksiejuk
//Projekt: £azik marsjañski KNR
/*
 * INSTRUKCJA OBS£UGI BIBLIOTEKI
 * 1)dolacz biblioteke do pliku z funkcja main za pomoc¹ polecenia #include "BootLoaderLib.h"
 * 2)na samym poczatku petli glownej umiesc funkcje BootLoaderInit()
 * !!!UWAGA initBootloader() musi byc pierwsza linijka funkcji main!!!!!
 * 3)W miejscu w którym chcesz uruchomic Bootloader umiesc funkcje startBootloader();
 * Pamietaj, ze po uruchomieniu bootloadera nastapi reset
 * ponowne uruchomienie programu glownego bedzie mozliwe dopiero po wgraniu nowego programu lub recznym wcisnieciu przycisku RESET
 *
 * Do wgrania nowego programu wykorzystaj "STMFlashLoader Demo" - darmowy program dla windows
 * Wgranie bootloadera za pomoaca UART jest mozliwe na pinach UART3 (PC10 - Tx; PC11 - Rx)
 * Teoretycznie istnieje mozliwosc wgrania programu za pomoca UART1 (PA9 - Tx;  PA10 - RX)
 * jednak piny te s¹ pod³¹czone do programatora i przy w³¹czonym zasilaniu programatora jest to niemozliwe
 * przy wylaczony nie bylo testowane
 */

#ifndef BootloaderLib_H
#define BootloaderLib_H

#include "stm32f4xx.h"
void initBootloader(void);
void startBootloader(void);

#endif
