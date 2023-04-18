#include <LPC17xx.H>
#include <glcd.h>
#include <TouchPanel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Math.h>
#include "PWM.h"
#include "tp_simple.h"
#include "stdbool.h"
#include "ADC.h"
#include "Nunchuck.h"
#include "audio_muestras.h"
#include "uart.h"
//defenimos frecuencias
#define Th_trigger (10e-6*Fpclk_TIMx)	 
#define T_trigger (20e-3*Fpclk_TIMx)        
#define Fcclk 100e6							
#define Fpclk_TIMx (Fcclk/4) 		       // En los defines con operaciones: recomendable (...)	   
#define F_muestreo 10                  // Fs=10Hz (Cada 100 ms se toma una muestra del canal 0 y 2 )
#define F_out 8000
#define Fpclk 25e6
#define N_muestras 15872               // igual a la longitud del array generado en Matlab

//defenimos variables externas
extern int8_t VelDer;
extern int8_t VelIzq;
extern int8_t Posserv;


int N, distancia;
int tiempo=0;
extern bool modobluetooth;
bool barridoluz =false;

extern uint8_t Estado;
extern uint32_t SLuz1,SLuz2,SPromedio;
int32_t valorluz =40;
bool Luz= false;

//variables interruptores
bool flagContadorKey1=false,flagContadorKey2=false, flagCambioMas2segPulsadores=false, flagCambioMenos2segPulsadores=false; 
int	contadorKey1=0, contadorKey2=0;

//VAriables Botones mando wii
bool flagContadorC=false, flagContadorZ=false,flagCambioMas2segBotones=false, flagCambioMenos2segBotones=false;
int	contadorC=0, contadorZ=0;
bool flagCambioPulsandoC=false;
extern uint8_t muestras[];
extern uint8_t audio;
extern int ALARMA;                  
extern int GRABANDO;               
extern int GRABADO; 
extern int FIN;

//Variables para marcar lecturas y movimientos
int Lectura_nunchuck = 0;
int GiroDer =0;
int GiroIzq=0;
bool Barrido = false;
bool Barrido_Completo=false;
bool Giro90= false;
bool Giromenos90= false;
int aux_barrido = 0;
int aux_Giro=0;
int aux=0;
int grados=0; 
int umbralDistancia= 27; 

// TIMER 0 PARA CONTADORES Y FUNCIONES CON CUENTA DE TIEMPOS MANDO NUNCHUCK, KEY1 Y KEY2, BARRIDO SERVO, GIROS DE COCHE
void init_TIMER0(void)
{
  LPC_SC->PCONP|=(1<<1);             //  Activa Timer0
  LPC_TIM0->PR  = 24;               //   Para que cada tick sea de 1 Mhz (Fpclk/PR+1)
  LPC_TIM0->MCR = 0x3;               //  Interrupt on Match 0 and Reset TC
  LPC_TIM0->MR0 = 10000;             //  Periodo de interrupcion de TODOS los contadores 10ms
  LPC_TIM0->EMR = 1<<0 | 1<<5;       //  Match por MR0 y set con 10 en EMC0 (bits 4 y 5)      
  LPC_TIM0->TCR = 0x01;              //  Arranca timer            
  NVIC_SetPriority(TIMER0_IRQn,4);   //  Prioridad por encima del priority grouping 
  NVIC_EnableIRQ(TIMER0_IRQn);            
}	                                   
void TIMER0_IRQHandler(void)
{	
	LPC_TIM0->IR|=(1<<1);              // Borrar flag interrupción	 
	Lectura_nunchuck++;
	aux_barrido++;
	aux_Giro++;	
	tiempo++;
	//LECTURA DEL MANDO NUNCHUCK
	if(Lectura_nunchuck == 19 && modobluetooth==false)
	{                   // Para que lea el Nunchuck cada segundo
		Lectura_nunchuck = 0;
		Nunchuck_read();
	}
	else if(Lectura_nunchuck == 19 && modobluetooth==true)
	{
		Lectura_nunchuck = 0;
	}
	//BARRIDOS DEL SERVOMOTOR
	if(aux_barrido == 49 && Barrido== true)
	{  		
		Posserv += 10;
	  setServo(Posserv);
		aux_barrido=0;
		if (SPromedio>valorluz)
		{
			aux_barrido=0;
			grados=Posserv;
			Posserv=0;
			setServo(Posserv);
			Barrido=false;
			barridoluz =true;
		}
		if(Posserv==90&&aux<1)
		{
			Posserv=-90;
			setServo(Posserv);
			aux++;
		}
		else if(Posserv==90 && aux==1) //Los dos barridos han sido completados
		{
			Posserv=0;
			setServo(Posserv);
			Barrido_Completo= true;
			Barrido=false;
		}
	}
	else if(Barrido==false)
	{
		aux_barrido=0;
	}
	//FUNCION DE GIROS 
	if(Giro90==true && Giromenos90==false )
	{
		VelDer=70;
	  VelIzq= 0;
	}
	else if(Giromenos90==true && Giro90==false)
	{
		VelDer= 0;
	  VelIzq= 70;
	}
	else if(Giromenos90==false && Giro90==false)
	{
		aux_Giro=0;
	}
	if(aux_Giro == 60 &&(Giro90==true ||Giromenos90 ==true))
	{
		VelDer=0;
		VelIzq=0;
		Giro90=false;
		Giromenos90= false;
	}
	if(aux_Giro>60)
	{
		aux_Giro=0;
	}
	//ESTADO DE LOS PULSADORES Y BOTONES
	if (flagContadorKey1)
	{
		contadorKey1++;
	}
	else
	{
		if (contadorKey1>199 && contadorKey2>199)
		{
		flagCambioMas2segPulsadores= true;
		contadorKey1=0;
		}
		else if(contadorKey1>0 && contadorKey2>0)
		{
			flagCambioMenos2segPulsadores= true;
			contadorKey1=0;
		}
 		else
			contadorKey1=0;
	}
	
	if (flagContadorKey2) 
		contadorKey2++;
		else
	{
		if (contadorKey1>199 && contadorKey2>199)
		{
		flagCambioMas2segPulsadores= true;
		contadorKey2=0;
		}
		else if(contadorKey1>0 && contadorKey2>0)
		{
			flagCambioMenos2segPulsadores= true;
			contadorKey2=0;
		}
 		else
			contadorKey2=0;
	}
	if (flagContadorC) 
		contadorC++;
	else
	{
		if (contadorC>199 && contadorZ>199)
		{
			flagCambioMas2segBotones= true;
			contadorC=0;
			contadorZ=0;;
		}
		else if(contadorC>0 && contadorZ>0)
		{
			flagCambioMenos2segBotones= true;
			contadorC=0;
			contadorZ=0;
		}
		else if(contadorC>199 && contadorZ==0)
		{
			flagCambioPulsandoC= true;
			contadorC=0;
		}
 		else if(contadorC>0 &&contadorZ==0)
		{
			contadorC=0;
			if(Estado==3)
			Audio(GRABANDO);
		}
	}
	
	if (flagContadorZ) 
		contadorZ++;
		else
	{
		if (contadorC>199 && contadorZ>199)
		{
			flagCambioMas2segBotones= true;
			contadorZ=0;
			contadorC=0;
		}
		else if(contadorC>0 && contadorZ>0)
		{
			flagCambioMenos2segBotones= true;
			contadorZ=0;
			contadorC=0;
		}
 		else if(contadorZ>0 && contadorC==0)
		{
			contadorZ=0;
			if (Estado==3)
			Audio(GRABADO);
		}
	}
}
// TIMER 1 PARA TODO AUDIO.
/*  Timer 1 en modo Output Compare (reset T0TC on Match 0)
	Counter clk: 25 MHz 	MAT1.0 : On match, salida de una muestra hacia el DAC */                                  

void init_TIMER1(void)            
{
	LPC_SC->PCONP|= (1<<2);						 // Activa timer 1
  LPC_TIM1->PR  = 0;     	 			   // Para que cada tick sea de 25 Mhz (Fpclk/PR+1)
  LPC_TIM1->MCR = 0x03;							 // Reset TC on Match, and Interrupt!  
  LPC_TIM1->MR0 = Fpclk_TIMx/F_out-1;     // MR1=25e6/(8KHz)-1 ------> Interrumpe cada 125 us 
	LPC_TIM1->TCR = 0x02;							 // Timer STOP y RESET
  NVIC_EnableIRQ(TIMER1_IRQn);			
	NVIC_SetPriority(TIMER1_IRQn,4);   // Prioridad por encima del priority grouping 
}                                    // No hace falta EMR ni asignar un pin porque no queremos ningun match, solo reseteo

void TIMER1_IRQHandler(void)
{
	//Relativas al audio

	static uint16_t indice_muestra;
	LPC_TIM1->IR|= (1<<0);           // Borrar flag
	if (audio == ALARMA) {
		LPC_DAC->DACR= muestra[indice_muestra++]<<8; // 8 bits!!!!
	}
	else if(audio == GRABADO)
	{
		LPC_DAC->DACR= muestras[indice_muestra++]<<8; // 8 bits!!!!
	}
	else if(audio==GRABANDO)
	{
		LPC_ADC->ADINTEN=(1<<2);// Hab. interrupción fin de conversión del canal 0
		NVIC_EnableIRQ(ADC_IRQn);;
	}
	if(indice_muestra==(N_muestras-1))
	{
		indice_muestra=0;
		LPC_TIM1->TCR=0x02;	//Stop Timer and reset, DAC= 0V.
		LPC_DAC->DACR=0;	 // 0 V		/33	
		init_ADC_LDRs();
		audio= FIN;		
  }
	else if( indice_muestra==(N_muestras/2)-1 && audio ==ALARMA)
	{
		indice_muestra=0;
		LPC_TIM1->TCR=0x02;	//Stop Timer and reset, DAC= 0V.
		LPC_DAC->DACR=0;	 // 0 V		/33	
		init_ADC_LDRs(); 
		audio =FIN;
	}
}

// TIMER 2 PARA LDRs Y MOVIMIENTO MODO AUTOMATICO EN FUNCION DE LOS LDRS
void init_TIMER2(void)
{
   LPC_SC->PCONP|=(1<<22);                // Alimento el Timer2 , está desactivado de forma predeterminada 
   LPC_PINCON->PINSEL9|=(1<<25);          // MAT2.0 para PWM servo (P4.28)	
   LPC_TIM2->MCR = 0x18;                  // Interrupt on Match 1 and Reset TC (bit 4 y 5 activos)
   LPC_TIM2->MR1 = (Fpclk/F_muestreo)-1;  // Periodo de muestreo de TODAS las entradas!!!!   
   LPC_TIM2->EMR = 0x00C2;                // MODO TOGGLE en EMC1
   LPC_TIM2->TCR = 0x01;                  // Arranca timer
   NVIC_EnableIRQ(TIMER2_IRQn); 
   NVIC_SetPriority(TIMER2_IRQn,4);       // Prioridad por encima del priority grouping   	
}	                                        // No hace falta asignar un pin porque no queremos ningun match, solo interrumpe

void TIMER2_IRQHandler(void)
{	
	 LPC_TIM2->IR|=(1<<1);   // Borrar flag interrupción
   LPC_ADC->ADCR|=(1<<16); // BURST=1 --> Cada 65TclkADC se toma una muestra de cada canal comenzando 
                           //             desde el más bajo (bit LSB de CR[0..7])
  Dato_sensorLuz1();
	Dato_sensorLuz2();
	Dato_promedioSensores();
	
	//Movimiento automatico modo automatico
	 if (Estado==5 && distancia>umbralDistancia && Giro90== false && Giromenos90== false)
	 {
		 if(SPromedio>=valorluz && barridoluz==false)					//Si el coche detecta LUZ
		 {
			 Posserv=0;
			 setServo(Posserv);
			 grados=0;
			 Barrido_Completo=false;
			 Barrido= false;
			 if(SLuz1>SLuz2) 								//Si la el el sensor IZq recibe mas luz nos movemos a la izquierda
			 {
				 VelIzq= 60;
				 VelDer = 75;
			 }
			 else if(SLuz1<SLuz2)					 //Si la el el sensor Der recibe mas luz nos movemos a la Derecha
			 {
				 VelIzq = 75;
				 VelDer = 60;
			 }
			 else													//Si reciben la misma Luz nos movemos a recto
			 {
				 VelDer =75;
				 VelIzq=75;
			 }
		 }
		else														//Si no detecta luz
		{
			if(Barrido_Completo==false&& Barrido== false && grados ==0)
			{
				Posserv=-90;
		  	setServo(Posserv);
				Barrido = true;
			}
			else if(Barrido_Completo== true)
			{
				VelDer=70;
				VelIzq=70;
			}
			else if(grados<0)
			{
				 VelIzq= 60;
				 VelDer = 75;
				 barridoluz=false;
			}
			else if(grados>0)
			{
				 VelIzq= 75;
				 VelDer = 60;
			   barridoluz=false;
			}
		}
	}
	else if(distancia<umbralDistancia&& Estado ==5)
	{
		Barrido =false;
		Barrido_Completo= false;
		
		if(SLuz1>SLuz2) 								//Si la el el sensor IZq recibe mas luz nos movemos a la izquierda
		 {
				Giro90= true;							 //Giro Izquierda 90º
		 }
		else if(SLuz2>SLuz1) 								//Si la el el sensor IZq recibe mas luz nos movemos a la izquierda
		 {
				Giromenos90= true;					 //Giro Derecha 90º
		 }
		 else
		 {
				Giro90= true;		
		 }
	}
	
}

// TIMER 3 PARA ULTRASONIDO
void init_TIMER3(void)
{
	LPC_SC->PCONP|= 1<<23;         // Alimento el Timer 3, que esta desactivado de forma predeterminada. 
	LPC_PINCON->PINSEL0|=(3<<22);  // MAT3.1 para el Trigger (P0.11)
	LPC_PINCON->PINSEL1|=(3<<14);  // CAP3.0 para el echo    (P0.23)
	
	LPC_TIM3->PR = 24; 
  LPC_TIM3->CCR = 0x6;           // Interrupcion en flanco de bajada
	
	LPC_TIM3->MCR |= (0x3<<0);     //  GENERAR INTERRUPCIÓN  Y RESETEAR CUANDO LLEGUE MR0 
	
	LPC_TIM3->MR0 =  T_trigger;  	
	LPC_TIM3->MR1 =  Th_trigger; 
	LPC_TIM3->EMR = 1<<1 | 1<<6 ;  // mas rápido que EMR |= 1<<1;

	NVIC_SetPriority(TIMER3_IRQn,3);  
	NVIC_EnableIRQ(TIMER3_IRQn);
	
	LPC_TIM3->TCR = 1<<1;         // RESET TIM3
	LPC_TIM3->TCR = 1<<0;         // START TIM3.
}

void TIMER3_IRQHandler(void) {
	
 static uint32_t temp; 
 if(LPC_TIM3->IR & (1 << 0)){
	LPC_TIM3->IR |= 1<<0;
	LPC_TIM3->EMR = 1<<6 | 1<<1;  // mas rápido que EMR |= 1<<1;
 }
 
  if (LPC_TIM3->IR & (1 << 4)){ // si hay evento de CR0
	 LPC_TIM3->IR |= 1<<4;
		
   if(LPC_TIM3->CCR == 5){
		temp = LPC_TIM3->CR0;
    LPC_TIM3->CCR = 0x6; 
	 } 		
	 else{
		N = LPC_TIM3->CR0 - temp;		// Tiempo en alto
		LPC_TIM3->CCR = 0x5; 
	 }
	 if(N>58){
	 distancia = N/58;
	 Dato_Distanciacm();
	if (distancia < umbralDistancia && Barrido==false){
		if(Estado==3){
		VelDer= 0;
		VelIzq = 0;}
		if(Estado==3 ||Estado==5)
		Audio(ALARMA);
	}
	}
}
}
//INICIA TODOS LOS TIMERS MENOS EL TIMER1 QUE UTILIZAMOS PARA EL AUDIO Y SOLO LO INICIAREMOS CUANDO LO NECESITEMOS
void init_TIMERS(void)
{
	init_TIMER0();
	init_TIMER2();
	init_TIMER3();
}
//FUNCION QUE CONTROLA QUE LOS PULSADORES SEAN PULSADOS
void controladorPulsadores(void) {
	if ((LPC_GPIO2->FIOPIN & 1<<11) == 0x0000) {  //Key1
		flagContadorKey1 = true;
	}
	else {
		flagContadorKey1 = false;
	}
	if ((LPC_GPIO2->FIOPIN & 1<<12) == 0x0000) {  //Key2
		flagContadorKey2 = true;
	}
	else {
		flagContadorKey2 = false;
	}
}

//FUNCION QUE REINICIA CONTADORES
void contadoresreset(void)
{
			contadorKey1=0;
		  contadorKey2=0;
		  contadorC = 0;
	   	contadorZ = 0;
		  flagCambioMas2segPulsadores=false;
			flagCambioMenos2segPulsadores=false; 
	  	flagCambioMas2segBotones=false;
			flagCambioMenos2segBotones=false;
		  flagCambioPulsandoC = false;
}
