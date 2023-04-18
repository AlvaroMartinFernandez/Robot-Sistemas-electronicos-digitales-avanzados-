#include <LPC17xx.H>
#include "WatchDog.h"

#define F_cpu 100e6					// Defecto Keil (xtal=12Mhz)
#define F_pclk F_cpu/4 			// Defecto despues del reset
#define F_wdclk F_pclk/4 		// WDT clk


/*Funcion que ejecuta la secuencia de recarga del WDT*/
void WDT_Feed(void)
{
	
	LPC_WDT->WDFEED=0xAA;
	LPC_WDT->WDFEED=0x55;

}

void WDT_init(void)
{
	LPC_WDT->WDTC= F_wdclk*8; // Timeout=8seg. //valor de recarga
	LPC_WDT->WDCLKSEL=0x01;  // Clock=PCLK
	LPC_WDT->WDMOD=0x03;		// Enable y Reset	si vence el tiempo 
  LPC_WDT->WDFEED=0xAA;
  LPC_WDT->WDFEED=0x55;
}
