#include <stdio.h>
#include <string.h>
#include <Math.h>
#include <LPC17xx.H>
#include "ADC.h"
#include "timer.h"
#include "Interrupciones.h"
#include "stdbool.h"
#include "tp_simple.h"

extern uint8_t Estado;
extern int contadorKey1;
extern int contadorKey2;
extern bool flagCambioMas2segPulsadores, flagCambioMenos2segPulsadores,flagContadorKey1,flagContadorKey2;


extern int ALARMA;                  
extern int GRABANDO;               
extern int GRABADO; 

void init_EINT(void)
{
  // Configuración interrupciones externas
  LPC_PINCON->PINSEL4|=(1<<22);			// P2.11 es entrada interrup. EXT 1 (pulsador key1 en mini-dk2)
  LPC_PINCON->PINSEL4|=(1<<24); 		// P2.12 es entrada interrup. EXT 2 (pulsador key2 en MIni-DK2)
  LPC_SC->EXTMODE|=(1<<1)|(1<<2);   // Por Flanco,	
  LPC_SC->EXTPOLAR=(1<<1)|(1<<2);	 //de subida
  NVIC_SetPriority(EINT2_IRQn, 6);		// Menor prioritaria!!! ;  sin CMSIS: NVIC->IP[18]=(4<<3); 
  NVIC_EnableIRQ(EINT2_IRQn); 				// sin CMSIS: NVIC->ISER[0]=(1<<18);
  NVIC_SetPriority(EINT1_IRQn,6);
  NVIC_EnableIRQ(EINT1_IRQn);
}

/*******************************************************************************
* Function Name  : EINT1 y EINT2 
* Description    : Para grabacion y  reproducion de audio
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void EINT1_IRQHandler()   // PARA GRABAR
{
	LPC_SC->EXTINT=(1<<1);	 // Borrar flag Externa 1
	if (Estado == 7 && flagCambioMas2segPulsadores== false && flagCambioMenos2segPulsadores== false && flagContadorKey2==false){
    Audio(GRABANDO);  			 //Para que el timer grabe  
	}
}
void EINT2_IRQHandler()   // PARA REPRODUCIR 
{
	LPC_SC->EXTINT=(1<<2); 	// Borrar flag Externa 2
	if (Estado == 7&& flagCambioMas2segPulsadores== false && flagCambioMenos2segPulsadores== false && flagContadorKey1==false){
		Audio(GRABADO); 							//Para que el timer reproduzca 
	}
}

