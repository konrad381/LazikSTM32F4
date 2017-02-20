#include "BootloaderLib.h"

void (*SysMemBootJump)(void);

//====================================================================================================
/**
 * @brief 	Uruchamia bootloader (mozliwe programowanie za pomoca UART2)
 * @note   	funkcja ustawia flagê i resetuje procesor. Wymagana jest obecnosc
 * 			funkcji initBootloader(); w pierwszym wierszu funckji main() do
 * 			uruchomienia bootloadera po resecie
 * @retval None
 */
void startBootloader(void) {

	RTC_WriteBackupRegister(RTC_BKP_DR4, 0xAA);// Ustawienie flagi, która bêdzie widoczna po resecie
while(1)
	;
	// Reset procesora
	NVIC_SystemReset();
}


//====================================================================================================
/**
 * @brief 	Sprawdza czy nalezy uruchomic bootloader po resecie
 * @note   	Funkcja inicjalizuje bootloader (uruchamia go po resecie w momencie gdy
 * 			ustawiona jest flaga). Funkcja powinna znajdowac siê w pierwszym wierszu
 * 			funkcji main().
 * @retval None
 */
void initBootloader(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_AHB1PeriphClockLPModeCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	uint32_t bt;
	//sprawdzenie flagi (czy uruchomic bootloader)
	bt = RTC_ReadBackupRegister(RTC_BKP_DR4);
	if (bt == 0xAA) {
	RTC_WriteBackupRegister(RTC_BKP_DR4, 0x99); // Zresetowanie Flagi
	//uruchomienie bootloadera
		void (*SysMemBootJump)(void);
		SYSCFG->MEMRMP |= SYSCFG_MEMRMP_MEM_MODE_0;
		uint32_t p = (*((uint32_t *) 0x1fff0000)); /// debug p returns 0x20002d40  0x1fff0000 is the bootloader rom start address
		__set_MSP(p); //Set the main stack pointer to its defualt values
		SysMemBootJump = (void (*)(void)) (*((uint32_t *) 0x1FFF0004)); // Point the PC to the System Memory reset vector (+4)
		SysMemBootJump();
		while (1)
			;
		}
}

