#include "ADC.h"
#include "Timer.h"


#define N_muestras 15872               // Igual a la longitud del array generado en Matlab

uint32_t canal_1, canal_2, canal_5;
extern uint32_t SLuz1,SLuz2,SPromedio;
uint8_t muestras[15872];

uint8_t audio;
int ALARMA = 1;                  
int GRABANDO=2;               
int GRABADO =3;
int FIN =4;

void init_ADC_LDRs(void)
{	 
   LPC_SC->PCONP|= (1<<12);                           // POwer ON
   LPC_PINCON->PINSEL1|= (1<<16);         					  // ( AD0.1)  P0.24) //LUZ1(izquierda)
	 LPC_PINCON->PINSEL3|= (3<<30);         					  // ( AD0.5)  P1.31) //LUZ2(derecha)
	 LPC_PINCON->PINMODE1|= (2<<16);         					  // Deshabilita pullup/pulldown
   LPC_PINCON->PINMODE3|= (2<<30); 				            // Deshabilita pullup/pulldown
   LPC_SC->PCLKSEL0|= (0<<8);                   		  // CCLK/4 (Fpclk después del reset) (100 Mhz/4 = 25Mhz)
   LPC_ADC->ADCR|=(1<<5)|(1<<1)                       // canales 0 y 1
                        | (1<<8)                		  // CLKDIV=1   (Fclk_ADC=25Mhz /(1+1)= 12.5Mhz)  MAXIMA FRECUENCIA 
                        | (1<<21);              	 	  // PDN=1  
   LPC_ADC->ADINTEN=(1<<5);					                  // Hab. interrupción fin de conversión del ÚLTIMO canal(canal 5)
   NVIC_EnableIRQ(ADC_IRQn);					 								//Habilita la interrupcion
   NVIC_SetPriority(ADC_IRQn,0);											//Prioridad
}	
void init_ADC_grabar(void)
{	
	LPC_SC->PCONP|= (1<<12);					// Power ON
	LPC_PINCON->PINSEL1|= (1<<18);  	// ADC input= P0.25 (AD0.2)
	LPC_PINCON->PINMODE1|= (2<<18); 	// Deshabilita pullup/pulldown*
	LPC_SC->PCLKSEL0|= (0<<8); 				// CCLK/4 (Fpclk después del reset) (100 Mhz/4 = 25Mhz)
	LPC_ADC->ADCR= (1<<2)|		  	    // Canal 2
								 (1<<8)|		  	    // CLKDIV=1  (Fclk_ADC=25Mhz /(1+1)= 12.5Mhz)   MAXIMA FRECUENCIA 
								 (1<<21)|					 	// PDN=1
								 (1<<24);				    // Inicio de conversión con el Match 1 del Timer 1
	LPC_ADC->ADINTEN =(1<<2);					// Hab. interrupción fin de conversión Canal 2 
	NVIC_EnableIRQ(ADC_IRQn);					 								//Habilita la interrupcion
  NVIC_SetPriority(ADC_IRQn,0);
}	
void ADC_IRQHandler(void)
{
	static uint16_t indice_muestra;
	if (audio == GRABANDO) {
		  canal_2=((LPC_ADC->ADDR2>>8)&0xFF);			// se borra automat. el flag DONE al leer ADCGDR
			muestras[indice_muestra++] = canal_2;;	//almacenamos la muestra tomada
			LPC_ADC->ADCR|=(1<<24); // START = ON
			NVIC_DisableIRQ(ADC_IRQn);
	  if(indice_muestra==N_muestras-1)
			{
				indice_muestra=0; 		// Reiniciamos el indice de muestras
				LPC_ADC->ADCR= (0<<24);// Paramos ADC
				init_ADC_LDRs();    	// Para iniciar la configuracion del timer para las LDRs
				LPC_TIM2->TCR=0x01;	  // Activa timer de LDRs
				audio = FIN;
			}
  }
	else{
	 LPC_ADC->ADCR&=~(1<<16);
   canal_1= ((LPC_ADC->ADDR1 >>4)&0xFFF);	// flag DONE se borra automat. al leer ADDR1
	 canal_5= ((LPC_ADC->ADDR5 >>4)&0xFFF);	// flag DONE se borra automat. al leer ADDR0
	 SLuz1 = ((canal_1/100)*2.5);  // 12 bits -> 4096
	 SLuz2 = ((canal_5/100)*2.5);  // 12 bits -> 4096
	
	 SPromedio = (SLuz1 + SLuz2)/2; 
	}  
}
/*******************************************************************************
* Function Name  : Audio
* Description    : En funcion de si queremos reproducir o grabar inicializa el timer 1 y el ADC
* Input          : audioElegido:   ALARMA   - Reproduce Alarma
*                                  GRABANDO - Graba audio
*																	 GRABADO - Reproduce audio grabado
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void Audio(int Opcion) {
	if (Opcion == ALARMA) {
		audio = ALARMA;
		init_TIMER1();
	  LPC_TIM1->TCR=0x01; 
	}
	if (Opcion == GRABANDO) {
		audio = GRABANDO;
		init_TIMER1(); 
		LPC_TIM1->TCR=0x01;
		init_ADC_grabar();;  
		LPC_TIM2->TCR = 0x02;        //Paramos el timer2 para que no haya modo burst y que solo se convierta el canal de grabacion		 
	}
	if (Opcion == GRABADO) {
		audio = GRABADO;
		init_TIMER1();
	  LPC_TIM1->TCR=0x01;  

	}
}
