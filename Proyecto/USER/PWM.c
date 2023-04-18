#include "PWM.h"
#include "tp_simple.h"


#define Fpclk 25e6	// Fcpu/4 (defecto después del reset)
#define Tpwm 15e-3	// Perido de la señal PWM (15ms)

int8_t VelDer = 0;
int8_t VelIzq = 0;
int8_t Posserv = 0;

/*******************************************************************************
* Function Name  : configPWM
* Description    : Configura una señal PWM de 15ms de periodo por P1.20
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None	
*******************************************************************************/
void configPWM(void) {
	
   LPC_PINCON->PINSEL7|=(3<<18);   // P3.25 salida PWM (PWM1.2) y LED 1
	 LPC_PINCON->PINSEL7|=(3<<20);   // P3.26 salida PWM (PWM1.3) y LEd 2
	 LPC_PINCON->PINSEL3|=(2<<14);   // P1.23 como salida PWM para servo (PWM1.4)
   LPC_SC->PCONP|=(1<<6);
   LPC_PWM1->MR0=Fpclk*Tpwm-1;
   LPC_PWM1->PCR|=(3<<10); //configurado el ENA2 (1.2),ENA3 (1.3),ENA4 (1.4)
	 LPC_PWM1->PCR|=(1<<12);         
   LPC_PWM1->MCR|=(1<<1);
   LPC_PWM1->TCR|=(1<<0)|(1<<3);
}

/*******************************************************************************
* Function Name  : MotorDerecha
* Description    : Genera el nivel alto con un PWC que representa la velocidad de las ruedas
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void MotorDerecha(int8_t VelDerecha)   // LED1 para la rueda Izquierda 
{
   LPC_PWM1->MR2=(((Fpclk*Tpwm-1)*abs(VelDerecha)/100)); // TH PWM1.2
   LPC_PWM1->LER|=(1<<2)|(1<<0); //le pasamos un 3 porque usamos el MR2 y MR0
	 Dato_motorDer();
	 if (VelDerecha > 0){
		LPC_GPIO1->FIOCLR |= (1<<20);   // Low el pin P1.20
	  LPC_GPIO1->FIOSET |= (1<<21);   // Hight el pin P1.21
	 }
	 else{
		LPC_GPIO1->FIOSET |= (1<<20);   // Hight el pin P1.20
	  LPC_GPIO1->FIOCLR |= (1<<21);   // Low el pin P1.21
	 }
}

/*******************************************************************************
* Function Name  : MotorIzquierda
* Description    : Genera el nivel alto con un PWC que representa la velocidad de las ruedas
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void MotorIzquierda(int8_t VelIzquierda)   // LED2 para la rueda izquierda 
{
   LPC_PWM1->MR3=(((Fpclk*Tpwm-1)*abs(VelIzquierda)/100)); // TH PWM1.3
   LPC_PWM1->LER|=(1<<3)|(1<<0); //le pasamos un 4 porque usamos el MR3 y MR0
	 Dato_motorIzq();
	 if (VelIzquierda > 0){
		 
		 LPC_GPIO1->FIOCLR |= (1<<25);   // Low el pin P1.24
	   LPC_GPIO1->FIOSET |= (1<<24);   // Hight el pin P1.25
	 }
	 else{
		 LPC_GPIO1->FIOSET |= (1<<25);   // Hight el pin P1.24
	   LPC_GPIO1->FIOCLR |= (1<<24);   // Low1¡ el pin P1.25
	 }
}

/*******************************************************************************
* Function Name  : PuenteH
* Description    : Configura Señales del puente en en H y los LED
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/ 
void PuenteH (void)
{	
	LPC_GPIO1->FIODIR |= (1<<20);   
	LPC_GPIO1->FIODIR |= (1<<21);  
	LPC_GPIO1->FIODIR |= (1<<24);  
	LPC_GPIO1->FIODIR |= (1<<25);  	
}

/*******************************************************************************
* Function Name  : setServo
* Description    : Actualiza el valor de la señal PWM
* Input          : grados - Debe tomar valores de -90 a 90
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void setServo(int8_t grados) {
   LPC_PWM1->MR4=(Fpclk*1.5e-3 + Fpclk*1e-3*grados/90); // TH
   LPC_PWM1->LER|=(1<<4)|(1<<0);
	 Dato_motorServo();
}
