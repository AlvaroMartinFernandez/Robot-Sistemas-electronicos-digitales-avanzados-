#include "tp_simple.h"
#include "PWM.h"
#include "StateChar.h"
#include "i2c_lpc17xx.h"
#include "Nunchuck.h"
#include "ADC.h"
#include "Timer.h"
#include "DAC.h"
#include "Interrupciones.h"
#include "uart.h"
#include "Bluetooth.h"
#include <RTL.h>
#include <Net_Config.h>
#include <serial.h>
#include "stdbool.h"
#include "WatchDog.h"

extern   U32 CheckMedia (void);



/*--------------------------- init ------------------------------------------*/

/*******************************************************************************
* Function Name  : init
* Description    : Initialize every subsystem
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
static void init () {
   
 	
  SER_Init ();   
  init_TcpNet ();   
  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
  


}



/*--------------------------- timer_poll ------------------------------------*/

/*******************************************************************************
* Function Name  : timer_poll
* Description    : Call timer_tick() if every 100ms (aprox) 
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
  }
}


extern uint8_t Estado;
extern struct t_screenZone Manual;
extern struct t_screenZone Automatico;
extern struct t_screenZone Depuracion;
extern int8_t VelDer;
extern int8_t VelIzq;
extern int8_t Posserv;
int main(void)
{
	LCD_Initializtion();
  LCD_Clear(Black);	
  TP_Init();
	Estado= 0;
//	TouchPanel_Calibrate();
	configPWM();
	PuenteH ();
	Nunchuck_Init();
	init_ADC_LDRs();
	init_TIMERS();
	init_DAC();
	init_EINT();
	NVIC_SetPriorityGrouping(2);
	uart0_init(9600);
  Nunchuck_Init();	
	WDT_init();
  init ();
	while(1)
	{
		MaquinaEstados();
		MaquinaEstadosBluetooth();
		MotorDerecha(VelDer);
		MotorIzquierda(VelIzq);
		setServo(Posserv);
	  timer_poll ();
    main_TcpNet ();			
		WDT_Feed();
	}
}
